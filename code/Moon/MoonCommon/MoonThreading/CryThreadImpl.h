// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryThread.h>

// Include architecture specific code.
#if CRY_PLATFORM_WINAPI
	#include <CryAtomics_impl_win32.h>
	#include <CryThreadImpl_win32.h>
#elif CRY_PLATFORM_POSIX
	#include <CryThreadImpl_posix.h>
#else
// Put other platform specific includes here!
#endif

// vim:ts=2

