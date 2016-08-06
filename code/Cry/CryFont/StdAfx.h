// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryModuleDefs.h>
#define eCryModule eCryM_Font

#define CRYFONT_EXPORTS

#include <platform.h>

#include <IFont.h>

#include <ILog.h>
#include <IConsole.h>
#include <IRenderer.h>
#include <CrySizer.h>

// USE_NULLFONT should be defined for all platforms running as a pure dedicated server
#if CRY_PLATFORM_DESKTOP
	#ifndef USE_NULLFONT
		#define USE_NULLFONT
	#endif
#endif
