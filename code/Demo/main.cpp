#define eCryModule eCryM_Game

#define USE_CRY_ASSERT
#include <CryCommon/CryAssert.h>
#include "CryCommon/CryModuleDefs.h"
#include "CryCommon/MSVCspecific.h"
#include "CryCommon/ISystem.h"
#include <CryCommon/CryLibrary.h>
#include <CryCommon/IGameStartup.h>
#include <CryCommon/IEditorGame.h>
#include <CryCommon/IGameFramework.h>
#include <CryCommon/IConsole.h>

#include "yasli/Archive.h"
#include "yasli/BinArchive.h"

#include "CrySystem/System.h"
#include "CrySystem/DebugCallStack.h"
#include "CrySystem/CryPak.h"

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
    SSystemInitParams startupParams;
    string gameDLLName;

    startupParams.bMoonEnable = false;
    startupParams.bSkipFont = true;                      //!< Don't load CryFont.dll.
    startupParams.bSkipRenderer = false;                 //!< Don't load Renderer.
    //startupParams.bSkipConsole = false;                //!< Don't create console.
    //startupParams.bSkipNetwork = false;                //!< Don't create Network.
    //startupParams.bSkipLiveCreate = false;             //!< Don't create LiveCreate.
    //startupParams.bSkipWebsocketServer = false;        //!< Don't create the WebSocket server.
    startupParams.bDedicatedServer = false;
    //restart parameters
    static const char logFileName[] = "moon.log";


    startupParams.hInstance = GetModuleHandle(0);
    startupParams.sLogFileName = logFileName;

    ISystem* pSys = CreateSystemInterface(startupParams);

    pSys->GetRootFolder();
    ICryPak* pPak = gEnv->pSystem->GetIPak();

    printf("Hello World!\n");

    //gEnv->pSystem->CreateAVIReader();
    //CCryPak* pak = nullptr;
    //pak->GetPakInfo();

    yasli::BinOArchive ar;
    ar.clear();

    lua_State* ls2 = luaL_newstate();

    return 0;
}
