/**
 * Auth :   liubo
 * Date :   
 * Comment: lua的一些工具函数！
 */

#pragma once

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


class LuaUtils
{
public:
    static void DumpStack(lua_State* L);
    static void RunCode(lua_State* L, const char* luaCode);
};
