#include "CryCommon/CryModuleDefs.h"
#define eCryModule eCryM_Game
#include <platform.h>
#include <platform_impl.inl>
#include <CryLibrary.h>
#include <IHardwareMouse.h>
#include "CryCommon/ISystem.h"
#include "CryCommon/CrySizer.h"
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

    lua_State* ls2 = luaL_newstate();

    return 0;
}
