#include "CryCommon/CryModuleDefs.h"
#define eCryModule eCryM_Game
#include <platform.h>
#include <platform_impl.inl>
#include "CryCommon/ISystem.h"
#include "CrySystem/CryPak.h"

extern "C"
{
    #include "lua.h"
    #include "lstate.h"
    #include "lauxlib.h"
}
#include <stdio.h>

class A
{
public:
    A()
    {
    }
};

A a;

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
