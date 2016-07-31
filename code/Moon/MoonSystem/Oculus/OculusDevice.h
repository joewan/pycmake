// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.
//
// Description: Oculus Hmd Device functionality updated to Oculus SDK 0.7.0
// - 03/06/2015 Created by Dario Sancho

#pragma once

#if defined(INCLUDE_OCULUS_SDK)

	#include <IHMDDevice.h>
	#include <ISystem.h>
	#include "OculusConfig.h"
	#include "OculusTouchController.h"
	#include "HMDCVars.h"

struct IConsoleCmdArgs;
struct IRenderer;

namespace CryVR {
namespace Oculus {

class Device:public IOculusDevice, public CryVR::IVRCmdListener, public ISystemEventListener
{
	public:
	// IHmdDevice interface
	virtual void AddRef() override;
	virtual void Release() override;

	virtual EHmdClass GetClass() const override                         { return eHmdClass_Oculus; }
	virtual void      GetDeviceInfo(HmdDeviceInfo &info) const override { info = m_devInfo; }

	virtual void                    GetCameraSetupInfo(float &fov, float &aspectRatioFactor) const override;
	virtual void                    GetAsymmetricCameraSetupInfo(int nEye, float &fov, float &aspectRatio, float &asymH, float &asymV, float &eyeDist) const;
	virtual void                    UpdateInternal(EInternalUpdate type) override;
	virtual void                    RecenterPose() override;
	virtual void                    UpdateTrackingState(EVRComponent type) override;
	virtual const HmdTrackingState& GetNativeTrackingState() const override;
	virtual const HmdTrackingState& GetLocalTrackingState() const override;
	virtual const IHmdController*   GetController() const override { return &m_controller; }
	virtual const EHmdSocialScreen  GetSocialScreenType() const override;
	virtual void                    DisableHMDTracking(bool disable) override;
	virtual void                    GetPreferredRenderResolution(unsigned int &width, unsigned int &height) override;
	virtual int                     GetControllerCount() const override;
	// ~IHmdDevice interface

	// IOculusDevice interface
	virtual bool CreateSwapTextureSetD3D11(void* d3d11Device, TextureDesc desc, TextureSet* set) override;
	virtual bool CreateMirrorTextureD3D11(void* d3d11Device, TextureDesc desc, Texture* texture) override;
	virtual void DestroySwapTextureSet(TextureSet* set) override;
	virtual void DestroyMirrorTexture(Texture* texture) override;
	virtual void PrepareTexture(TextureSet* set, uint32 frameIndex) override;
	virtual void SubmitFrame(EyeTarget leftEye, EyeTarget rightEye) override;
	// ~IOculusDevice interface

	// IVRCmdListener
	virtual void OnCommand(EVRCmd cmd, IConsoleCmdArgs* pArgs);
	// ~IVRCmdListener

	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam);
	// ~ISystemEventListener

	public:
	int  GetRefCount() const { return m_refCount; }
	bool HasSession() const  { return m_pSession != 0; }

	public:
	static Device* CreateInstance();

	private:
	Device();
	virtual ~Device();

	void CreateDevice();
	void PrintHmdInfo();

	private:
	static void OnHmdRecenterPose(IConsoleCmdArgs* pArgs);
	static void OnHmdSocialScreenChange(IConsoleCmdArgs* pArgs);

	private:
	struct RenderParameters
	{
		ovrPosef         eyePoses[ovrEye_Count];
		ovrFovPort       eyeFovs[ovrEye_Count];
		ovrViewScaleDesc viewScaleDesc;
	};

	private:
	RenderParameters& GetFrameRenderParameters();
	float             UpdateCurrentIPD();
	void              DebugDraw(float &xPosLabel, float &yPosLabel) const;

	private:
	volatile int m_refCount;
	int          m_lastFrameID_UpdateTrackingState; // we could remove this. at some point we may want to sample more than once the tracking state per frame.
	bool         m_queueAhead;
	bool         m_bLoadingScreenActive;

	ovrSession    m_pSession;                       // opaque pointer to an OVR session
	ovrHmdDesc    m_hmdDesc;                        // complete descriptor of the HMD
	HmdDeviceInfo m_devInfo;

	HmdTrackingState m_nativeTrackingState;
	HmdTrackingState m_localTrackingState;
	HmdTrackingState m_disabledTrackingState;

	RenderParameters m_frameRenderParams[2];        // double buffered frame render parameters to avoid race condition between caching it (main thread) and reading it (render thread)

	ovrFovPort m_eyeFovSym;

	COculusController m_controller;

	ovrSizei m_preferredSize;                       //query & cache preferred texture size to map the texture/screen resolution 1:1

	bool m_disableHeadTracking;
};

}                                                 // namespace Oculus
}                                                 // namespace CryVR

#endif                                            // #if defined(INCLUDE_OCULUS_SDK)
