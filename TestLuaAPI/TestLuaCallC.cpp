/**
 * Auth :   liubo
 * Date :   2023-08-10 00:00:00
 * Comment: 测试lua中调用C++的函数！
 */

#include <iostream>
#include <string.h>

#include "LuaUtils.h"
#include "AutoRegTestFunc.h"
#include "LuaCppBind.h"
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


static int test0()
{
    return 0;
}

static int test1(int a)
{
    return a;
}

static int test2(int a, float b)
{
    return a - b;
}

static const char* test3(const char* str)
{
    return str;
}

static void test4()
{

}
static void test5(int a)
{
    std::cout<<a;
}

static void test6(int a, float b)
{
    std::cout<<a << "\t" << b;
}
static void test7(int a, float b, char* c)
{
    std::cout<<a << "\t" << b << "\t" << (c == nullptr ? "" : c);
}

static void Test4()
{
    /* 初始化Lua */
    /* 指向Lua解释器的指针 */
    lua_State* L = luaL_newstate();

    /* 载入Lua基本库 */
    luaL_openlibs(L);

    // 自动注册！
    REGISTER_FUNCTION(L, test0);
    REGISTER_FUNCTION(L, test1);
    REGISTER_FUNCTION(L, test2);
    REGISTER_FUNCTION(L, test3);
    REGISTER_FUNCTION(L, test4);
    REGISTER_FUNCTION(L, test5);
    REGISTER_FUNCTION(L, test6);
    REGISTER_FUNCTION(L, test7);

    const char* luaCode = R"delimiter(
print("test0:", test0())
print("test1:", test1(10))
print("test2:", test2(12, 1))
print("test3:", test3("Hello World"))
print("test4:", test4())
print("test5:", test5(1))
print("test6:", test6(1, 2))
print("test7:", test7(1, 2, "3"))
        )delimiter";

    LuaUtils::RunCode(L, luaCode);
    
    /* 清除Lua */
    lua_close(L);
}
static AutoRegTestFunc autoTest4(Test4);