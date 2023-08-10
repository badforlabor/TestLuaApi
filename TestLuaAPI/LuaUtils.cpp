#include "LuaUtils.h"

#include <assert.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>

template<typename ...Args>
static std::string Format(const char* format, Args ...args)
{
    int size = std::snprintf(nullptr, 0, format, args...)+1;
    if(size <= 0)
    {
        throw std::runtime_error("error format!");
    }

    std::unique_ptr<char[]> buf(new char[size]);
    auto n = std::snprintf(buf.get(), size, format, args...);
    assert(n == size-1);
    assert(buf[size-1] == '\0');

    
    return std::string(buf.get(), buf.get() + size);
}
template<typename ...Args>
static void Printf(const char* format, Args ...args)
{
    auto s = Format(format, args...);
    std::cout << s.c_str();
    std::flush(std::cout);
}


void LuaUtils::DumpStack(lua_State* L)
{
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++)
    {
        Printf("%d\t%s\t", i, luaL_typename(L, i));
        // cout << i << "\t" << luaL_typename(L, i) << endl;        
        switch (lua_type(L, i))
        {
        case LUA_TNUMBER:
            Printf("%g\n",lua_tonumber(L, i));
            break;
        case LUA_TSTRING:
            Printf("%s\n",lua_tostring(L, i));
            break;
        case LUA_TBOOLEAN:
            Printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
            break;
        case LUA_TNIL:
            Printf("%s\n", "nil");
            break;
        default:
            Printf("%p\n", lua_topointer(L, i));
            break;
        }
    }
}
void LuaUtils::RunCode(lua_State* L, const char* luaCode)
{
    /* 运行脚本 */
    int bRet = luaL_loadstring(L, luaCode);
    if (bRet)
    {
        std::cout << "load file error" << std::endl;
        return;
    }

    // 执行lua代码
    bRet = lua_pcall(L, 0, 0, 0);
    if (bRet)
    {
        std::cout << "pcall error" << std::endl;
        return;
    }
}