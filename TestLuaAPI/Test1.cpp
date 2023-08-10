#include <iostream>
#include <string.h>

#include "AutoRegTestFunc.h"
using namespace std;

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

static void Test1()
{
    //1.创建Lua状态，返回一个指向堆栈的指针
    lua_State* L = luaL_newstate();
    if (L == NULL)
    {
        return;
    }

    //2.加载lua文件	
    //int bRet = luaL_loadfile(L, "hello.lua");

    const char* luaCode = R"delimiter(
str = "I am x-man."
tbl = {name = "DC", id = 20114442}
function add(a,b)
	return a + b
end
        )delimiter";

    int bRet = luaL_loadstring(L, luaCode);
    if (bRet)
    {
        cout << "load file error" << endl;
        return;
    }

    //3.运行lua文件
    bRet = lua_pcall(L, 0, 0, 0);
    if (bRet)
    {
        cout << "pcall error" << endl;
        return;
    }

    //4.读取全局变量，
    // 1.把 str 压栈 2.由lua去寻找全局变量str的值，并将str的值返回栈顶（替换str）
    // 如果存在相同命名的其他变量、table或函数，就会报错（读取位置发生访问冲突）
    lua_getglobal(L, "str");
    // -1取出栈顶元素，转化为string
    string str = lua_tostring(L, -1);
    cout << "str = " << str.c_str() << endl;

    //5.读取table，把table压栈
    lua_getglobal(L, "tbl");
    //-------------------------------
    // 1.把name压入栈中，2.由lua去寻找table中name键的值，并将键值返回栈顶（替换name）
    // 相当于lua_pushstring(L, "name") + lua_gettable(L, -2)执行结果是一样的
    lua_getfield(L, -1, "name");
    // 把name压入栈中
    //lua_pushstring(L, "name");
    // 弹出栈上的name，并从表中找到name的键值，把结果放在栈上相同的位置
    //lua_gettable(L, -2);
    //---------------------------------
    str = lua_tostring(L, -1);
    // 因为table在栈顶的下面，所以取-2，把id压栈，由lua找到table中id键的值，并返回栈顶（替换id）
    lua_getfield(L, -2, "id");
    // id的值已经在栈顶，取-1
    int id = lua_tonumber(L, -1);
    cout << "tbl:name = " << str.c_str() << endl;
    cout << "tbl:id = " << id << endl;

    // 读取函数，
    // 1.将函数add放入栈中，2.由lua去寻找函数add，并将函数add返回栈顶（替换add）。
    lua_getglobal(L, "add"); // 获取函数，压入栈中
    lua_pushnumber(L, 10); // 压入第一个参数
    lua_pushnumber(L, 20); // 压入第二个参数
    // 栈过程：参数出栈->保存参数->参数出栈->保存参数->函数出栈->调用函数->返回结果入栈
    // 调用函数，调用完成以后，会将返回值压入栈中，2表示参数个数，1表示返回结果个数。
    int iRet = lua_pcall(L, 2, 1, 0);
    if (iRet == 0)
    {
        if (lua_isnumber(L, -1)) //取值输出
        {
            int fValue = lua_tonumber(L, -1);
            cout << "Result is " << fValue << endl;
        }
    }
    else
    {
        // 调用出错
        const char* pErrorMsg = lua_tostring(L, -1);
        cout << pErrorMsg << endl;
        lua_close(L);
        return;
    }

    // 栈的索引方式可以是正数也可以是负数，区别是：1永远表示栈底元素，-1永远表示栈顶元素。
    //至此，栈中的情况是：
    //=================== 栈顶 =================== 
    // 索引    类型      值
    // 5或-1   int       30 
    // 4或-2   int       20114442
    // 3或-3   string    shun 
    // 2或-4   table	 tbl
    // 1或-5   string	 I am so cool~
    //=================== 栈底 =================== 

    lua_pushstring(L, "Master");
    // 会将"Master"值出栈，保存值，找到到table的name键，如果键存在，存储到name键中
    lua_setfield(L, 2, "name");
    // 读取
    lua_getfield(L, 2, "name");
    str = lua_tostring(L, -1);
    cout << "tbl:name = " << str.c_str() << endl;

    // 创建新的table
    lua_newtable(L);
    lua_pushstring(L, "A New Girlfriend");
    lua_setfield(L, -2, "name");
    // 读取
    lua_getfield(L, -1, "name");
    str = lua_tostring(L, -1);
    cout << "newtbl:name = " << str.c_str() << endl;

    //7.关闭state
    // 销毁指定 Lua 状态机中的所有对象， 并且释放状态机中使用的所有动态内存。
    // （如果有垃圾收集相关的元方法的话，会调用它们）
    lua_close(L);
}
static AutoRegTestFunc autoTest(Test1);