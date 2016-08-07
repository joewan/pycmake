#include "lua.h"
#include "lstate.h"
#include "lauxlib.h"
#include "stdio.h"

//#include "CryCommon/CryModuleDefs.h"
//#define eCryModule eCryM_Game

//#include <platform.h>
//#include "CryCommon/ISystem.h"

int main()
{
    //IScriptSystem* ptr = nullptr;
    //ptr->DebugContinue();

    //ISystem* ptr2 = nullptr;
    //ptr2->CreateAVIReader();
    //lua_State ls;
    //ls.size_ci = 254;
    //std::cout << ls.size_ci << std::endl;
    printf("hello world!\n");
    lua_State* ls2 = luaL_newstate();
    ls2 = 0;
    return 0;
}
