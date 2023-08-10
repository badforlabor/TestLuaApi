/**
 * Auth :   liubo
 * Date :   2023-08-10 00:00:00
 * Comment: 测试lua中调用C++的函数！
 */

#include <iostream>
#include <string.h>

#include "LuaUtils.h"
#include "AutoRegTestFunc.h"
using namespace std;

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


static int average(lua_State* L)
{
    /* 得到参数个数 */
    int n = lua_gettop(L);
    double sum = 0;
    int i;

    /* 循环求参数之和 */
    for (i = 1; i <= n; i++)
    {
        /* 求和 */
        sum += lua_tonumber(L, i);
    }
    /* 压入平均值 */
    lua_pushnumber(L, sum / n);
    /* 压入和 */
    lua_pushnumber(L, sum);
    /* 返回返回值的个数 */
    return 2;
}

static void Test3()
{
    /* 初始化Lua */
    /* 指向Lua解释器的指针 */
    lua_State* L = luaL_newstate();

    /* 载入Lua基本库 */
    luaL_openlibs(L);

    /* 注册函数 */
    lua_register(L, "average", average);

    // 设置lua中的全局变量
    lua_pushinteger(L, 18); //入栈
    lua_setglobal(L, "age"); //1.先将18值出栈，保存值，2.在lua中，把值存储到全局变量age中

    // 设置lua中table
    lua_newtable(L); //创建一张空表，并将其压栈
    lua_pushstring(L, "lili"); // 入栈
    // 1.先将"lili"值出栈，保存值，2.找table的name键，如果键存在，存储到name键中
    lua_setfield(L, -2, "name"); //栈顶是lili，新创建的table在lili下，所以是-2
    // 将table赋值到lua中，并弹出table
    lua_setglobal(L, "newTable");

    const char* luaCode = R"delimiter(
avg, sum = average(10, 20, 30, 40, 50)
print("The average is ", avg)
print("The sum is ", sum)
 
print("age", age)
for k,v in pairs(newTable) do 
     print("k = ",k," v = ",v)
end
print("name", newTable.name)
        )delimiter";

    LuaUtils::RunCode(L, luaCode);
    
    /* 清除Lua */
    lua_close(L);
}

static AutoRegTestFunc autoTest(Test3);