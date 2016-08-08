#define eCryModule eCryM_Game

#define USE_CRY_ASSERT
#include <CryAssert.h>

#include "CryCommon/CryModuleDefs.h"
#include "MSVCspecific.h"
#include "CryCommon/ISystem.h"
#include "yasli/Archive.h"
#include "yasli/BinArchive.h"


//#include <CryLibrary.h>
//#include <IGameStartup.h>
//#include <IGameFramework.h>
//#include <IConsole.h>
#include "CrySystem/System.h"
//#include "CrySystem/DebugCallStack.h"
//#include "CrySystem/CryPak.h"

extern ISystem* CreateSystemInterface(const SSystemInitParams &startupParams);

extern "C"
{
    #include "lua.h"
    #include "lstate.h"
    #include "lauxlib.h"
}

#include <stdio.h>

int main()
{
    SSystemInitParams params;
    CreateSystemInterface(params);

    printf("Hello World!\n");

    gEnv->pSystem->CreateAVIReader();
    //CCryPak* pak = nullptr;
    //pak->GetPakInfo();

    //yasli::BinOArchive ar;

    lua_State* ls2 = luaL_newstate();

    return 0;
}
