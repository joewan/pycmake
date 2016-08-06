// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

// -------------------------------------------------------------------------
//  File name:   stdafx.h
//  Version:     v1.00
//  Created:     30/9/2002 by Timur.
//  Compilers:   Visual Studio.NET
//  Description: Precompiled Header.
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#ifndef __stdafx_h__
#define __stdafx_h__

#if _MSC_VER > 1000
	#pragma once
#endif

//#define DEFINE_MODULE_NAME "CrySystem"

#include <CryModuleDefs.h>
#define eCryModule eCryM_System

#define CRYSYSTEM_EXPORTS

#include <platform.h>

//////////////////////////////////////////////////////////////////////////
// CRT
//////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#if !CRY_PLATFORM_LINUX && !CRY_PLATFORM_ANDROID && !CRY_PLATFORM_APPLE && !CRY_PLATFORM_ORBIS
	#include <memory.h>
	#include <malloc.h>
#endif
#include <fcntl.h>

#if !CRY_PLATFORM_ORBIS && !CRY_PLATFORM_APPLE && !CRY_PLATFORM_ANDROID
	#if CRY_PLATFORM_LINUX
		#include <sys/io.h>
	#else
		#include <io.h>
	#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CRY Stuff ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include "Cry_Math.h"
#include <Cry_Camera.h>
#include <smartptr.h>
#include <Range.h>
#include <CrySizer.h>
#include <StlUtils.h>


static inline int RoundToClosestMB( size_t memSize )
{
	// add half a MB and shift down to get closest MB
	return((int) ((memSize + (1 << 19)) >> 20));
}


//////////////////////////////////////////////////////////////////////////
// For faster compilation
//////////////////////////////////////////////////////////////////////////
#include <IRenderer.h>
#include <CryFile.h>
#include <ISystem.h>
#include <IScriptSystem.h>
#include <IEntitySystem.h>
#include <I3DEngine.h>
#include <ITimer.h>
#include <IAudioSystem.h>
#include <IPhysics.h>
#include <IAISystem.h>
#include <IXml.h>
#include <ICmdLine.h>
#include <IInput.h>
#include <IConsole.h>
#include <ILog.h>

#include "FrameProfiler/FrameProfiler_JobSystem.h"  // to be removed

/////////////////////////////////////////////////////////////////////////////
//forward declarations for common Interfaces.
/////////////////////////////////////////////////////////////////////////////
class ITexture;
struct IRenderer;
struct ISystem;
struct IScriptSystem;
struct ITimer;
struct IFFont;
struct IInput;
struct IKeyboard;
struct ICVar;
struct IConsole;
struct IGame;
struct IEntitySystem;
struct IProcess;
struct ICryPak;
struct ICryFont;
struct I3DEngine;
struct IMovieSystem;
struct IAudioSystem;
struct IPhysicalWorld;

/////////////////////////////////////////////////////////////////////////////
// HMD libraries
/////////////////////////////////////////////////////////////////////////////
#include "IHMDDevice.h"

#endif // __stdafx_h__


