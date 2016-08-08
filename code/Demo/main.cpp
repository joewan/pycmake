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
    SSystemInitParams startupParams;
    string gameDLLName;

    //startupParams.bShaderCacheGen = true;
    startupParams.bSkipFont = true;                    //!< Don't load CryFont.dll.
    startupParams.bSkipRenderer = true;                //!< Don't load Renderer.
    //startupParams.bSkipConsole = false;                //!< Don't create console.
    //startupParams.bSkipNetwork = false;                //!< Don't create Network.
    //startupParams.bSkipLiveCreate = false;             //!< Don't create LiveCreate.
    //startupParams.bSkipWebsocketServer = false;        //!< Don't create the WebSocket server.
    startupParams.bDedicatedServer = true;
    //restart parameters
    static const char logFileName[] = "Server.log";


    startupParams.hInstance = GetModuleHandle(0);
    startupParams.sLogFileName = logFileName;

    ISystem* pSys = CreateSystemInterface(startupParams);

    pSys->GetRootFolder();
    ICryPak* pPak = gEnv->pSystem->GetIPak();

    printf("Hello World!\n");

    //gEnv->pSystem->CreateAVIReader();
    //CCryPak* pak = nullptr;
    //pak->GetPakInfo();

    //yasli::BinOArchive ar;

    lua_State* ls2 = luaL_newstate();

    return 0;
}
