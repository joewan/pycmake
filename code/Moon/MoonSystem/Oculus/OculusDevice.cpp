// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.
//
// Description: Oculus Hmd Device functionality updated to Oculus SDK 0.7.0
// - 03/06/2015 Created by Dario Sancho

#include "StdAfx.h"

#if defined(INCLUDE_OCULUS_SDK)

	#include "OculusDevice.h"
	#include "OculusResources.h"

	#include <IStereoRenderer.h>

namespace
{
// Note: This symmetric fov is still used for frustum culling
ovrFovPort ComputeSymmetricalFov(EHmdType hmdType, const ovrFovPort &fovLeftEye, const ovrFovPort &fovRightEye)
{
	// CB aspect ratio = 1.8 (2160x1200), DK2 aspect ratio = 1.7777 (1920x1080)
	const float stereoDeviceAR =
	  hmdType == eHmdType_CB ? 1.8f :
	  hmdType == eHmdType_EVT ? 1.8f :
	  hmdType == eHmdType_DK2 ? 1.7777f :
	  1.7777f;   // not clear that we should support anything under DK2

	CryLog("[OculusDevice] Aspect Ratio selected: %f", stereoDeviceAR);
	ovrFovPort fovMax;
	fovMax.UpTan    = max(fovLeftEye.UpTan, fovRightEye.UpTan);
	fovMax.DownTan  = max(fovLeftEye.DownTan, fovRightEye.DownTan);
	fovMax.LeftTan  = max(fovLeftEye.LeftTan, fovRightEye.LeftTan);
	fovMax.RightTan = max(fovLeftEye.RightTan, fovRightEye.RightTan);

	const float combinedTanHalfFovHorizontal = max(fovMax.LeftTan, fovMax.RightTan);
	const float combinedTanHalfFovVertical   = max(fovMax.UpTan, fovMax.DownTan);

	ovrFovPort fovSym;
	fovSym.UpTan = fovSym.DownTan = combinedTanHalfFovVertical * 1.f;
	// DARIO_NOTE: This adapts the Horiz. Fov to the proper AR of Oculus. The 2.f factor is because the width of the eye textures is half the width of the backbuffer one
	fovSym.LeftTan = fovSym.RightTan = fovSym.UpTan / (2.f / stereoDeviceAR);

	CryLog("[OculusDevice] Fov: Up/Down tans [%f] Left/Right tans [%f]", fovSym.UpTan, fovSym.LeftTan);
	return fovSym;
}

Quat HmdQuatToWorldQuat(const Quat &quat)
{
	Matrix33 m33(quat);
	Vec3     column1 = -quat.GetColumn2();
	m33.SetColumn2(m33.GetColumn1());
	m33.SetColumn1(column1);
	return Quat::CreateRotationX(gf_PI * 0.5f) * Quat(m33);
}

Vec3 HmdVec3ToWorldVec3(const Vec3 &vec)
{
	return Vec3(vec.x, -vec.z, vec.y);
}

void CopyPoseState(HmdPoseState &dst, HmdPoseState &worldDst, const ovrPoseStatef &src)
{
	dst.orientation         = Quat(src.ThePose.Orientation.w, src.ThePose.Orientation.x, src.ThePose.Orientation.y, src.ThePose.Orientation.z);
	dst.position            = Vec3(src.ThePose.Position.x, src.ThePose.Position.y, src.ThePose.Position.z);
	dst.angularVelocity     = Vec3(src.AngularVelocity.x, src.AngularVelocity.y, src.AngularVelocity.z);
	dst.linearVelocity      = Vec3(src.LinearVelocity.x, src.LinearVelocity.y, src.LinearVelocity.z);
	dst.angularAcceleration = Vec3(src.AngularAcceleration.x, src.AngularAcceleration.y, src.AngularAcceleration.z);
	dst.linearAcceleration  = Vec3(src.LinearAcceleration.x, src.LinearAcceleration.y, src.LinearAcceleration.z);

	worldDst.orientation         = HmdQuatToWorldQuat(dst.orientation);
	worldDst.position            = HmdVec3ToWorldVec3(dst.position);
	worldDst.angularVelocity     = HmdVec3ToWorldVec3(dst.angularVelocity);
	worldDst.linearVelocity      = HmdVec3ToWorldVec3(dst.linearVelocity);
	worldDst.angularAcceleration = HmdVec3ToWorldVec3(dst.angularAcceleration);
	worldDst.linearAcceleration  = HmdVec3ToWorldVec3(dst.linearAcceleration);
}

void LogHmdCharacteristics(const ovrHmdDesc &hmdDesc, const HmdDeviceInfo &devInfo, const CryVR::Oculus::COculusController &controller)
{
	CryLogAlways("[HMD][Oculus] - Device: %s - Type:%u", hmdDesc.ProductName ? hmdDesc.ProductName : "unknown", hmdDesc.Type);
	CryLogAlways("[HMD][Oculus] --- Manufacturer: %s", hmdDesc.Manufacturer ? hmdDesc.Manufacturer : "unknown");
	CryLogAlways("[HMD][Oculus] --- VendorId: %d", hmdDesc.VendorId);
	CryLogAlways("[HMD][Oculus] --- ProductId: %d", hmdDesc.ProductId);
	CryLogAlways("[HMD][Oculus] --- Firmware: %d.%d", hmdDesc.FirmwareMajor, hmdDesc.FirmwareMinor);
	CryLogAlways("[HMD][Oculus] --- SerialNumber: %s", hmdDesc.SerialNumber);
	CryLogAlways("[HMD][Oculus] - Resolution: %dx%d", hmdDesc.Resolution.w, hmdDesc.Resolution.h);
	CryLogAlways("[HMD][Oculus] - Horizontal FOV: %.2f degrees", devInfo.fovH * 180.0f / gf_PI);
	CryLogAlways("[HMD][Oculus] - Vertical FOV: %.2f degrees", devInfo.fovV * 180.0f / gf_PI);

	CryLogAlways("-[HMD][Oculus] Low persistence mode: Enabled by default");
	CryLogAlways("-[HMD][Oculus] Dynamic prediction: Enabled by default (for DK2 and later)");

	CryLogAlways("-[HMD][Oculus] Sensor orientation tracking: %s", (hmdDesc.AvailableTrackingCaps & ovrTrackingCap_Orientation) ? "supported" : "unsupported");
	CryLogAlways("-[HMD][Oculus] Sensor mag yaw correction: %s", (hmdDesc.AvailableTrackingCaps & ovrTrackingCap_MagYawCorrection) ? "supported" : "unsupported");
	CryLogAlways("-[HMD][Oculus] Sensor position tracking: %s", (hmdDesc.AvailableTrackingCaps & ovrTrackingCap_Position) ? "supported" : "unsupported");

	CryLogAlways("-[HMD][Oculus] Controller - Left Hand : %s", controller.IsConnected(eHmdController_OculusLeftHand) ? "Connected" : "Disconnected");
	CryLogAlways("-[HMD][Oculus] Controller - Right Hand: %s", controller.IsConnected(eHmdController_OculusRightHand) ? "Connected" : "Disconnected");
}

} // anonymous namespace

// -------------------------------------------------------------------------
namespace CryVR {
namespace Oculus {

// -------------------------------------------------------------------------
Device::Device()
	: m_refCount(1)
	, m_lastFrameID_UpdateTrackingState(-1)
	, m_queueAhead(CryVR::CVars::hmd_queue_ahead > 0)
	, m_bLoadingScreenActive(false)
	, m_pSession(0)
	, m_hmdDesc()
	, m_devInfo()
	, m_disableHeadTracking(false)
{
	memset(&m_eyeFovSym, 0, sizeof(m_eyeFovSym));

	CreateDevice();
	UpdateCurrentIPD();

	if (m_pSession && GetISystem()->GetISystemEventDispatcher())
		GetISystem()->GetISystemEventDispatcher()->RegisterListener(this);

	CryVR::CVars::RegisterListener(this);
}

// -------------------------------------------------------------------------
Device::~Device()
{
	CryVR::CVars::UnregisterListener(this);

	if (m_pSession)
	{
		if (GetISystem()->GetISystemEventDispatcher())
			GetISystem()->GetISystemEventDispatcher()->RemoveListener(this);

		ovr_ConfigureTracking(m_pSession, 0, 0);
		ovr_Destroy(m_pSession);

		m_pSession = 0;
	}
}

// -------------------------------------------------------------------------
void Device::AddRef()
{
	CryInterlockedIncrement(&m_refCount);
}

// -------------------------------------------------------------------------
void Device::Release()
{
	long refCount = CryInterlockedDecrement(&m_refCount);
	#if !defined(_RELEASE)
	IF(refCount < 0, 0)
	__debugbreak();
	#endif
	IF(refCount == 0, 0)
	{
		delete this;
	}
}

// -------------------------------------------------------------------------
Device* Device::CreateInstance()
{
	Device* pDevice = new Device();
	if (pDevice && !pDevice->HasSession())
	{
		SAFE_RELEASE(pDevice);
	}
	return pDevice;
}

// -------------------------------------------------------------------------
void Device::CreateDevice()
{
	ovrGraphicsLuid luid;
	ovrResult       result = ovr_Create(&m_pSession, &luid);
	if (m_pSession && OVR_SUCCESS(result))
	{
		m_hmdDesc      = ovr_GetHmdDesc(m_pSession);
		m_devInfo.type = eHmdType_Unknown;
		if (m_hmdDesc.Type == ovrHmd_DK1)
			m_devInfo.type = eHmdType_DK1;
		else if (m_hmdDesc.Type == ovrHmd_DKHD)
			m_devInfo.type = eHmdType_DKHD;
		else if (m_hmdDesc.Type == ovrHmd_DK2)
			m_devInfo.type = eHmdType_DK2;
		else if (m_hmdDesc.Type == ovrHmd_CB)
			m_devInfo.type = eHmdType_CB;
		else if (m_hmdDesc.Type >= ovrHmd_ES06 || m_hmdDesc.Type == ovrHmd_Other)     // TODO: need to properly address EVTs once they stabilize the models
			m_devInfo.type = eHmdType_EVT;

		if (m_devInfo.type == eHmdType_EVT)
		{
			m_controller.Init(m_pSession);
			CryLogAlways("[HMD][Oculus] Touch Controllers initialized.");
		}
		else
		{
			CryLogAlways("[HMD][Oculus] Touch Controllers were not initialized because the detected HMD does not support them. Device [(Oculus)%d / (CryEngine)%d]", m_hmdDesc.Type, m_devInfo.type);
		}

		m_eyeFovSym = ComputeSymmetricalFov(m_devInfo.type, m_hmdDesc.DefaultEyeFov[ovrEye_Left], m_hmdDesc.DefaultEyeFov[ovrEye_Right]);

		m_devInfo.productName  = m_hmdDesc.ProductName;
		m_devInfo.manufacturer = m_hmdDesc.Manufacturer;
		m_devInfo.screenWidth  = m_hmdDesc.Resolution.w;
		m_devInfo.screenHeight = m_hmdDesc.Resolution.h;
		m_devInfo.fovH         = 2.0f * atanf(m_eyeFovSym.LeftTan);
		m_devInfo.fovV         = 2.0f * atanf(m_eyeFovSym.UpTan);

		LogHmdCharacteristics(m_hmdDesc, m_devInfo, m_controller);

		ovrSizei leftEyeSize  = ovr_GetFovTextureSize(m_pSession, ovrEye_Left, m_hmdDesc.DefaultEyeFov[ovrEye_Left], 1.f);
		ovrSizei rightEyeSize = ovr_GetFovTextureSize(m_pSession, ovrEye_Right, m_hmdDesc.DefaultEyeFov[ovrEye_Right], 1.f);

		m_preferredSize.w = max(leftEyeSize.w, rightEyeSize.w);
		m_preferredSize.h = max(leftEyeSize.h, rightEyeSize.h);
	}
	else
	{
		ovrErrorInfo errorInfo;
		ovr_GetLastErrorInfo(&errorInfo);
		CryLogAlways("[HMD][Oculus] Failed to initialize HMD device! [%s]", *errorInfo.ErrorString ? errorInfo.ErrorString : "unspecified error");
	}
}

// -------------------------------------------------------------------------
void Device::GetPreferredRenderResolution(unsigned int &width, unsigned int &height)
{
	width  = m_preferredSize.w;
	height = m_preferredSize.h;
}

// -------------------------------------------------------------------------
void Device::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_LEVEL_LOAD_START_LOADINGSCREEN:
		m_bLoadingScreenActive = true;
		break;

	// Intentional fall through
	case ESYSTEM_EVENT_LEVEL_LOAD_END:
	case ESYSTEM_EVENT_LEVEL_LOAD_ERROR:
		m_bLoadingScreenActive = false;
		m_controller.ClearControllerState(COculusController::eCV_FullController);
		break;

	case ESYSTEM_EVENT_LEVEL_GAMEPLAY_START:
		m_controller.ClearControllerState(COculusController::eCV_FullController);
		break;

	default:
		break;
	}
}

// -------------------------------------------------------------------------
// DARIO_NOTE: This is called to get the FOV that will be passed to the camera for frustum culling
// Later (D3DStereo) the camera frustum and asymmetry is overwritten before rendering the scene
void Device::GetCameraSetupInfo(float &fov, float &aspectRatioFactor) const
{
	// DARIO_NOTE: We are forcing symmetric FOV. This needs to be reworked at some point.
	if (m_pSession)
	{
		fov               = m_devInfo.fovV;
		aspectRatioFactor = 2.0f * m_eyeFovSym.LeftTan;
	}
	else
	{
		CryLog("[HMD][Oculus] GetCameraSetupInfo returns invalid information since the HMD was not created");
		// Just return something valid
		fov               = 0.5f * gf_PI;
		aspectRatioFactor = 2.0f;
	}
}

// -------------------------------------------------------------------------
void Device::GetAsymmetricCameraSetupInfo(int nEye, float &fov, float &aspectRatio, float &asymH, float &asymV, float &eyeDist) const
{
	// calculate from projection matrix
	const float zNear = 0.25f;      // @theo: why these values?
	const float zFar  = 8000.f;
	ovrMatrix4f proj  = ovrMatrix4f_Projection(m_hmdDesc.DefaultEyeFov[nEye], zNear, zFar, ovrProjection_RightHanded);
	fov         = 2.0f * atan(1.0f / proj.M[1][1]);
	aspectRatio = proj.M[1][1] / proj.M[0][0];
	asymH       = proj.M[0][2] / proj.M[1][1] * aspectRatio;
	asymV       = proj.M[1][2] / proj.M[1][1];

	eyeDist = CryVR::CVars::hmd_ipd;

	static bool _bPrinted = false;
	if (!_bPrinted)
	{
		ovrMatrix4f projLeft  = ovrMatrix4f_Projection(m_hmdDesc.DefaultEyeFov[ovrEye_Left], zNear, zFar, ovrProjection_RightHanded);
		ovrMatrix4f projRight = ovrMatrix4f_Projection(m_hmdDesc.DefaultEyeFov[ovrEye_Right], zNear, zFar, ovrProjection_RightHanded);

		CryLogAlways("Left FOV: %f\t%f\t%f\t%f",
		             m_hmdDesc.DefaultEyeFov[ovrEye_Left].LeftTan, m_hmdDesc.DefaultEyeFov[ovrEye_Left].RightTan,
		             m_hmdDesc.DefaultEyeFov[ovrEye_Left].DownTan, m_hmdDesc.DefaultEyeFov[ovrEye_Left].UpTan);

		CryLogAlways("%f\t%f\t%f\t%f", projLeft.M[0][0], projLeft.M[0][1], projLeft.M[0][2], projLeft.M[0][3]);
		CryLogAlways("%f\t%f\t%f\t%f", projLeft.M[1][0], projLeft.M[1][1], projLeft.M[1][2], projLeft.M[1][3]);
		CryLogAlways("%f\t%f\t%f\t%f", projLeft.M[2][0], projLeft.M[2][1], projLeft.M[2][2], projLeft.M[2][3]);
		CryLogAlways("%f\t%f\t%f\t%f", projLeft.M[3][0], projLeft.M[3][1], projLeft.M[3][2], projLeft.M[3][3]);

		CryLogAlways("Right FOV: %f\t%f\t%f\t%f",
		             m_hmdDesc.DefaultEyeFov[ovrEye_Right].LeftTan, m_hmdDesc.DefaultEyeFov[ovrEye_Right].RightTan,
		             m_hmdDesc.DefaultEyeFov[ovrEye_Right].DownTan, m_hmdDesc.DefaultEyeFov[ovrEye_Right].UpTan);

		CryLogAlways("%f\t%f\t%f\t%f", projRight.M[0][0], projRight.M[0][1], projRight.M[0][2], projRight.M[0][3]);
		CryLogAlways("%f\t%f\t%f\t%f", projRight.M[1][0], projRight.M[1][1], projRight.M[1][2], projRight.M[1][3]);
		CryLogAlways("%f\t%f\t%f\t%f", projRight.M[2][0], projRight.M[2][1], projRight.M[2][2], projRight.M[2][3]);
		CryLogAlways("%f\t%f\t%f\t%f", projRight.M[3][0], projRight.M[3][1], projRight.M[3][2], projRight.M[3][3]);

		_bPrinted = true;
	}
}

// -------------------------------------------------------------------------
void Device::RecenterPose()
{
	if (m_pSession)
	{
		ovr_RecenterPose(m_pSession);
	}
}

// -------------------------------------------------------------------------
void Device::DisableHMDTracking(bool disable)
{
	m_disableHeadTracking = disable;
}

// -------------------------------------------------------------------------
void Device::UpdateTrackingState(EVRComponent type)
{
	if (m_pSession)
	{
		// Allow update Queue-Ahead at run-time
		const bool cvarQueueAhead = (CryVR::CVars::hmd_queue_ahead > 0);
		if (m_queueAhead != cvarQueueAhead)
		{
			m_queueAhead = cvarQueueAhead;
			if (OVR_SUCCESS(ovr_SetBool(m_pSession, "QueueAheadEnabled", m_queueAhead)))
			{
				CryLog("[HMD][Oculus] QueueAheadEnabled changed to %s", m_queueAhead ? "TRUE" : "FALSE");
			}
			else
			{
				CryLog("[HMD][Oculus] QueueAheadEnabled changed failed trying to set the value to %s", m_queueAhead ? "TRUE" : "FALSE");
			}
		}

		IRenderer* pRenderer = gEnv->pRenderer;
		const int  frameID   = pRenderer->GetFrameID(false);

	#if !defined(_RELEASE)
		// Check we update things in the right thread at not too often

		if (!gEnv->IsEditor())    // we currently assume one system update per frame rendered, which is not always the case in editor (i.e. no level)
		{
			if (((type & eVRComponent_Hmd) != 0) && (CryGetCurrentThreadId() != gEnv->mMainThreadId) && (m_bLoadingScreenActive == false))
			{
				gEnv->pLog->LogError("[HMD][Oculus] Device::UpdateTrackingState() should be called from main thread only!");
			}

			if (frameID == m_lastFrameID_UpdateTrackingState)
			{
				gEnv->pLog->LogError("[HMD][Oculus] Device::UpdateTrackingState() should be called only once per frame!");
			}
		}
		m_lastFrameID_UpdateTrackingState = frameID;
	#endif

		// Update tracking state for HMD and controllers
		const float halfIPD = UpdateCurrentIPD() * 0.5f;

		//Get eye poses, feeding in correct IPD offset
		// Note: /*halfIPD*/ this is a hack to fix badly projected health & safety text that happens in Oculus runtime 0.7 for EVT devices
		// Oculus suggested this fix and said that this value is only used for a very specific layer that it is not used at the moment
		ovrVector3f ViewOffset[2];
		ViewOffset[0].x = /*halfIPD*/ 0.f;
		ViewOffset[0].y = 0.f;
		ViewOffset[0].z = 0.f;
		ViewOffset[1].x = /*-halfIPD*/ 0.f;
		ViewOffset[1].y = 0.f;
		ViewOffset[1].z = 0.f;

		// get the current tracking state
		ovrPosef         eyePose[2];
		ovrTrackingState trackingState;
		ovr_GetEyePoses(m_pSession, frameID, false, ViewOffset, eyePose, &trackingState);

		const ICVar* pStereoScaleCoefficient = gEnv->pConsole ? gEnv->pConsole->GetCVar("r_stereoScaleCoefficient") : 0;
		const float  stereoScaleCoefficient  = pStereoScaleCoefficient ? pStereoScaleCoefficient->GetFVal() : 1.f;

		CopyPoseState(m_nativeTrackingState.pose, m_localTrackingState.pose, trackingState.HeadPose);

		const int statusFlags =
		  ((trackingState.StatusFlags & ovrStatus_OrientationTracked) ? eHmdStatus_OrientationTracked : 0) |
		  ((trackingState.StatusFlags & ovrStatus_PositionTracked) ? eHmdStatus_PositionTracked : 0) |
		  ((trackingState.StatusFlags & ovrStatus_CameraPoseTracked) ? eHmdStatus_CameraPoseTracked : 0) |
		  ((trackingState.StatusFlags & ovrStatus_PositionConnected) ? eHmdStatus_PositionConnected : 0) |
		  ((trackingState.StatusFlags & ovrStatus_HmdConnected) ? eHmdStatus_HmdConnected : 0);

		// HMD-specific tracking update
		if ((type & eVRComponent_Hmd) != 0)
		{
			// scale pose based on r_stereoScaleCoefficient (this cvar also affects the eye distance)
			m_nativeTrackingState.pose.position *= stereoScaleCoefficient;
			m_localTrackingState.pose.position  *= stereoScaleCoefficient;
			m_localTrackingState.statusFlags     = m_nativeTrackingState.statusFlags = statusFlags;

			// store the parameters required for distortion rendering
			RenderParameters &frameParams = GetFrameRenderParameters();
			for (uint32 eye = 0; eye < 2; ++eye)
			{
				frameParams.eyeFovs[eye]                          = m_hmdDesc.DefaultEyeFov[eye];
				frameParams.eyePoses[eye]                         = eyePose[eye];
				frameParams.viewScaleDesc.HmdToEyeViewOffset[eye] = ViewOffset[eye];
			}
			frameParams.viewScaleDesc.HmdSpaceToWorldScaleInMeters = stereoScaleCoefficient;
		}

		// Controller-specific tracking update
		if ((type & eVRComponent_Controller) != 0)
		{
			CopyPoseState(m_controller.m_state.m_handNativeTrackingState[eHmdController_OculusLeftHand].pose, m_controller.m_state.m_handLocalTrackingState[eHmdController_OculusLeftHand].pose, trackingState.HandPoses[ovrHand_Left]);
			CopyPoseState(m_controller.m_state.m_handNativeTrackingState[eHmdController_OculusRightHand].pose, m_controller.m_state.m_handLocalTrackingState[eHmdController_OculusRightHand].pose, trackingState.HandPoses[ovrHand_Right]);

			for (size_t i = 0; i < eHmdController_MaxNumOculusControllers; ++i)
			{
				m_controller.m_state.m_handNativeTrackingState[i].pose.position *= stereoScaleCoefficient;
				m_controller.m_state.m_handLocalTrackingState[i].pose.position  *= stereoScaleCoefficient;

				m_controller.m_state.m_handNativeTrackingState[i].statusFlags = statusFlags;
				m_controller.m_state.m_handLocalTrackingState[i].statusFlags  = statusFlags;
			}

			m_controller.Update();
			if (CryVR::CVars::hmd_info)
			{
				float xPos = 10.f, yPos = 10.f;
				DebugDraw(xPos, yPos);
				yPos += 25.f;
				m_controller.DebugDraw(xPos, yPos);
			}
		}
	}
}

// -------------------------------------------------------------------------
const EHmdSocialScreen Device::GetSocialScreenType() const
{
	const int kFirstInvalidIndex = static_cast<int>(EHmdSocialScreen::eHmdSocialScreen_FirstInvalidIndex);

	if (CryVR::CVars::hmd_social_screen >= -1 && CryVR::CVars::hmd_social_screen < kFirstInvalidIndex)
	{
		const EHmdSocialScreen socialScreenType = static_cast<EHmdSocialScreen>(CryVR::CVars::hmd_social_screen);
		return socialScreenType;
	}

	return EHmdSocialScreen::eHmdSocialScreen_DistortedDualImage;     // default to dual distorted image
}

// -------------------------------------------------------------------------
void Device::UpdateInternal(EInternalUpdate type)
{
	switch (type)
	{
	case IHmdDevice::eInternalUpdate_DebugInfo:
	{
		if (CryVR::CVars::hmd_info)
		{
			if (Device* pDevice = Resources::GetAssociatedDevice())
				pDevice->PrintHmdInfo();
		}
	}
	break;
	default:
		break;
	}
}

// -------------------------------------------------------------------------
void Device::OnCommand(EVRCmd cmd, IConsoleCmdArgs* pArgs)
{
	switch (cmd)
	{
	case CryVR::eVRC_Recenter:
		if (Device* pDevice = Resources::GetAssociatedDevice())
			pDevice->RecenterPose();
		break;
	default:
		break;
	}
}

// -------------------------------------------------------------------------
const HmdTrackingState & Device::GetNativeTrackingState() const
{
	return !m_disableHeadTracking ? m_nativeTrackingState : m_disabledTrackingState;
}

// -------------------------------------------------------------------------
const HmdTrackingState & Device::GetLocalTrackingState() const
{
	return !m_disableHeadTracking ? m_localTrackingState : m_disabledTrackingState;
}

// -------------------------------------------------------------------------
Device::RenderParameters & Device::GetFrameRenderParameters()
{
	IRenderer* pRenderer = gEnv->pRenderer;
	const int  frameID   = pRenderer->GetFrameID(false);
	return m_frameRenderParams[frameID % 2];
}

// -------------------------------------------------------------------------
float Device::UpdateCurrentIPD()
{
	if (m_pSession && CVars::hmd_ipd < 0)
	{
		const float defaultIPD = 1.f;     // clearly wrong value [1m] to make sure we detect the problem
		CVars::hmd_ipd = ovr_GetFloat(m_pSession, OVR_KEY_IPD, defaultIPD);

		CryLogAlways("[HMD][Oculus] The current IPD (hmd_ipd) was < 0. Setting it to the value read from Oculus SDK [%f]", CryVR::CVars::hmd_ipd);
	}

	return CryVR::CVars::hmd_ipd;
}

// -------------------------------------------------------------------------
void Device::PrintHmdInfo()
{
	// TODO
}

// -------------------------------------------------------------------------
void Device::DebugDraw(float &xPosLabel, float &yPosLabel) const
{
	IRenderer* pRenderer = gEnv->pRenderer;

	if (!pRenderer)
		return;

	// Basic info
	const float  yPos = yPosLabel, xPosData = xPosLabel, yDelta = 20.f;
	float        y    = yPos;
	const ColorF fColorLabel(1.0f, 1.0f, 1.0f, 1.0f);
	const ColorF fColorIdInfo(1.0f, 1.0f, 0.0f, 1.0f);
	const ColorF fColorInfo(0.0f, 1.0f, 0.0f, 1.0f);
	const ColorF fColorDataPose(0.0f, 1.0f, 1.0f, 1.0f);

	pRenderer->Draw2dLabel(xPosLabel, y, 1.3f, fColorLabel, false, "Oculus Hmd Info:");
	y += yDelta;

	pRenderer->Draw2dLabel(xPosData, y, 1.3f, fColorIdInfo, false, "Device:%s (%u)-Manufacturer:%s", m_hmdDesc.ProductName ? m_hmdDesc.ProductName : "unknown", m_hmdDesc.Type, m_hmdDesc.Manufacturer ? m_hmdDesc.Manufacturer : "unknown");
	y += yDelta;
	pRenderer->Draw2dLabel(xPosData, y, 1.3f, fColorIdInfo, false, "VendorId:%d-ProductId:%d-Firmware: %d.%d-SerialNumber: %s", m_hmdDesc.VendorId, m_hmdDesc.ProductId, m_hmdDesc.FirmwareMajor, m_hmdDesc.FirmwareMinor, m_hmdDesc.SerialNumber);
	y += yDelta;
	pRenderer->Draw2dLabel(xPosData, y, 1.3f, fColorInfo, false, "Resolution: %dx%d", m_hmdDesc.Resolution.w, m_hmdDesc.Resolution.h);
	y += yDelta;
	pRenderer->Draw2dLabel(xPosData, y, 1.3f, fColorInfo, false, "FOV (degrees): H:%.2f - V:%.2f", RAD2DEG(m_devInfo.fovH), RAD2DEG(m_devInfo.fovV));
	y += yDelta;
	pRenderer->Draw2dLabel(xPosData, y, 1.3f, fColorInfo, false, "Caps: Rot tracking [%s], Mag Yaw correction [%s], Pos tracking [%s]",
	                       (m_hmdDesc.AvailableTrackingCaps & ovrTrackingCap_Orientation) ? "supported" : "unsupported",
	                       (m_hmdDesc.AvailableTrackingCaps & ovrTrackingCap_MagYawCorrection) ? "supported" : "unsupported",
	                       (m_hmdDesc.AvailableTrackingCaps & ovrTrackingCap_Position) ? "supported" : "unsupported"
	                       );
	y += yDelta;

	const Vec3 hmdPos = m_localTrackingState.pose.position;
	const Ang3 hmdRotAng(m_localTrackingState.pose.orientation);
	const Vec3 hmdRot(RAD2DEG(hmdRotAng));
	pRenderer->Draw2dLabel(xPosData, y, 1.3f, fColorDataPose, false, "HmdPos(LS):[%.f,%f,%f]", hmdPos.x, hmdPos.y, hmdPos.z);
	y += yDelta;
	pRenderer->Draw2dLabel(xPosData, y, 1.3f, fColorDataPose, false, "HmdRot(LS):[%.f,%f,%f] (PRY) degrees", hmdRot.x, hmdRot.y, hmdRot.z);
	y += yDelta;

	yPosLabel = y;
}


// -------------------------------------------------------------------------
// From SDK 0.6.0 we can only use Oculus SDK Rendering
// Hence the code below
// -------------------------------------------------------------------------
bool Device::CreateSwapTextureSetD3D11(void* d3d11Device, TextureDesc desc, TextureSet* set)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width              = desc.width;
	textureDesc.Height             = desc.height;
	textureDesc.MipLevels          = 1;
	textureDesc.ArraySize          = 1;
	textureDesc.Format             = (DXGI_FORMAT)desc.format;
	textureDesc.SampleDesc.Count   = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage              = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags     = 0;
	textureDesc.MiscFlags          = 0;

	ovrSwapTextureSet* textureSet = NULL;
	ovrResult          result     = ovr_CreateSwapTextureSetD3D11(
	  m_pSession,
	  static_cast<ID3D11Device*>(d3d11Device),
	  &textureDesc, ovrSwapTextureSetD3D11_Typeless, &textureSet);

	if (!OVR_SUCCESS(result) || textureSet == NULL)
	{
		ovrErrorInfo errorInfo;
		ovr_GetLastErrorInfo(&errorInfo);
		gEnv->pLog->Log("[HMD][Oculus] %s [%s]", "Unable to create D3D11 texture swap chain!", *errorInfo.ErrorString ? errorInfo.ErrorString : "unspecified error");
		return false;
	}

	uint32 numTextures = textureSet->TextureCount;

	set->deviceSwapTextureSet = static_cast<void*>(textureSet);
	set->textures             = new void*[numTextures];
	for (uint32 t = 0; t < numTextures; ++t)
	{
		set->textures[t] = reinterpret_cast<ovrD3D11Texture &>(textureSet->Textures[t]).D3D11.pTexture;
	}
	set->numTextures = numTextures;
	return true;
}

// -------------------------------------------------------------------------
bool Device::CreateMirrorTextureD3D11(void* d3d11Device, TextureDesc desc, Texture* texture)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width              = desc.width;
	textureDesc.Height             = desc.height;
	textureDesc.MipLevels          = 1;
	textureDesc.ArraySize          = 1;
	textureDesc.Format             = (DXGI_FORMAT)desc.format;
	textureDesc.SampleDesc.Count   = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage              = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags     = 0;
	textureDesc.MiscFlags          = 0;

	ovrTexture* deviceTexture = NULL;
	ovrResult   result        = ovr_CreateMirrorTextureD3D11(
	  m_pSession,
	  static_cast<ID3D11Device*>(d3d11Device),
	  &textureDesc, ovrSwapTextureSetD3D11_Typeless, &deviceTexture);

	if (!OVR_SUCCESS(result) || deviceTexture == NULL)
	{
		ovrErrorInfo errorInfo;
		ovr_GetLastErrorInfo(&errorInfo);
		gEnv->pLog->Log("[HMD][Oculus] %s [%s]", "Unable to create D3D11 mirror texture!", *errorInfo.ErrorString ? errorInfo.ErrorString : "unspecified error");
		return false;
	}

	texture->deviceTexture = static_cast<void*>(deviceTexture);
	texture->texture       = reinterpret_cast<ovrD3D11Texture*>(deviceTexture)->D3D11.pTexture;

	return true;
}

// -------------------------------------------------------------------------
void Device::DestroySwapTextureSet(TextureSet* set)
{
	ovr_DestroySwapTextureSet(m_pSession, static_cast<ovrSwapTextureSet*>(set->deviceSwapTextureSet));
	SAFE_DELETE_ARRAY(set->textures);
	set->textures = NULL;
}

// -------------------------------------------------------------------------
void Device::DestroyMirrorTexture(Texture* texture)
{
	ovr_DestroyMirrorTexture(m_pSession, static_cast<ovrTexture*>(texture->deviceTexture));
	texture->deviceTexture = NULL;
}

// -------------------------------------------------------------------------
void Device::PrepareTexture(TextureSet* set, uint32 frameIndex)
{
	ovrSwapTextureSet* textureSwapSet = static_cast<ovrSwapTextureSet*>(set->deviceSwapTextureSet);
	assert(frameIndex < textureSwapSet->TextureCount);
	textureSwapSet->CurrentIndex = frameIndex;
}

// -------------------------------------------------------------------------
void Device::SubmitFrame(EyeTarget leftEye, EyeTarget rightEye)
{
	FRAME_PROFILER("OculusDevice::SubmitFrame", gEnv->pSystem, PROFILE_SYSTEM);

	const EyeTarget*  eyes[]      = { &leftEye, &rightEye };
	RenderParameters &frameParams = GetFrameRenderParameters();

	ovrLayerEyeFov layer;
	layer.Header.Type  = ovrLayerType_EyeFov;
	layer.Header.Flags = 0;
	for (uint32 eye = 0; eye < 2; ++eye)
	{
		layer.ColorTexture[eye]    = static_cast<ovrSwapTextureSet*>(eyes[eye]->deviceSwapTextureSet);
		layer.Viewport[eye].Pos.x  = eyes[eye]->viewportPosition.x;
		layer.Viewport[eye].Pos.y  = eyes[eye]->viewportPosition.y;
		layer.Viewport[eye].Size.w = eyes[eye]->viewportSize.x;
		layer.Viewport[eye].Size.h = eyes[eye]->viewportSize.y;
		layer.Fov[eye]             = frameParams.eyeFovs[eye];
		layer.RenderPose[eye]      = frameParams.eyePoses[eye];
	}

	ovrLayerHeader* layers = &layer.Header;
	ovrResult       result = ovr_SubmitFrame(m_pSession, 0, &frameParams.viewScaleDesc, &layers, 1);
	if (!OVR_SUCCESS(result))
	{
		ovrErrorInfo errorInfo;
		ovr_GetLastErrorInfo(&errorInfo);
		gEnv->pLog->Log("[HMD][Oculus] %s [%s]", "Unable to submit frame to the Oculus device!", *errorInfo.ErrorString ? errorInfo.ErrorString : "unspecified error");
	}
}

// -------------------------------------------------------------------------
int Device::GetControllerCount() const
{
	const uint32 cnt = m_controller.IsConnected(eHmdController_OculusLeftHand) ? 1 : 0;
	return m_controller.IsConnected(eHmdController_OculusRightHand) ? cnt + 1 : cnt;
}
}      // namespace Oculus
}      // namespace CryVR


#endif // #if defined(INCLUDE_OCULUS_SDK)
