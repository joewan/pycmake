#define _LAUNCHER
#define eCryModule eCryM_Game

#include "CryCommon/CryModuleDefs.h"

#include <platform.h>
#include <platform_impl.inl>

#include "CryCommon/ISystem.h"
#include "yasli/Archive.h"
#include "yasli/BinArchive.h"

#include <MacSpecific.h>
#include <CryAssert.h>
#include <CryLibrary.h>
#include <IGameStartup.h>
#include <IGameFramework.h>
#include <IConsole.h>
#include "CrySystem/System.h"
#include "CrySystem/DebugCallStack.h"
#include "CrySystem/CryPak.h"

extern "C"
{
    #include "lua.h"
    #include "lstate.h"
    #include "lauxlib.h"
}

#include <stdio.h>

int main()
{
    printf("Hello World!\n");

    ISystem* ptr2 = nullptr;
    ptr2->CreateAVIReader();
    CCryPak* pak = nullptr;
    pak->GetPakInfo();

    yasli::BinOArchive ar;

    lua_State* ls2 = luaL_newstate();

    return 0;
}
