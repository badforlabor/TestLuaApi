#include <assert.h>
#include <iostream>
#include <string.h>
#include <vector>

#include "LuaUtils.h"
#include "AutoRegTestFunc.h"
using namespace std;

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static void Test2Impl(lua_State* L)
{
    const char* luaCode = R"delimiter(
str = "I am x-man."
tbl = {name = "DC-TBL", id = 1111}
tbl2 = {name = "DC2-TBL2", id = 2222}
        )delimiter";

    // 加载lua代码
    int bRet = luaL_loadstring(L, luaCode);
    if (bRet)
    {
        cout << "load file error" << endl;
        return;
    }

    // 执行lua代码
    bRet = lua_pcall(L, 0, 0, 0);
    if (bRet)
    {
        cout << "pcall error" << endl;
        return;
    }

    int stackCount = lua_gettop(L);
    cout << stackCount << endl;

    lua_getglobal(L, "tbl");
    lua_pushstring(L, "name");
    LuaUtils::DumpStack(L);
    lua_gettable(L, -2);
    LuaUtils::DumpStack(L);
    {
        assert(lua_isstring(L, -1));
        auto str = lua_tostring(L, -1);
        assert(_stricmp(str, "DC-TBL") == 0);
    }

    LuaUtils::DumpStack(L);
    lua_getfield(L, -2, "name");
    {
        assert(lua_isstring(L, -1));
        auto str = lua_tostring(L, -1);
        assert(_stricmp(str, "DC-TBL") == 0);
    }
    LuaUtils::DumpStack(L);
}

static void Test2()
{
    lua_State* L = luaL_newstate();
    Test2Impl(L);
    lua_close(L);
}


static AutoRegTestFunc autoTest(Test2);
