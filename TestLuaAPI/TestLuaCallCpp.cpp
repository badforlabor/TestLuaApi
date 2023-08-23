/**
 * Auth :   liubo
 * Date :   2023-08-22 15:00
 * Comment: lua中定义"类"
 */

#include <cassert>
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

namespace test_luacallcpp
{

    struct Book
    {
        ~Book()
        {
            std::cout << "~Book" << std::endl;
        }
        std::string Name;
        float Price;

        void Show()
        {
            std::cout << "book name=" << Name << ", price=" << Price << std::endl;            
        }
    };
    // 使用UserData
    static int NewBookWithUD(lua_State *L)
    {
        size_t size = sizeof(Book);
        
        // 由Lua自己管理内存！需要手动调用构造函数（或者调用memset清零）
        auto book = (Book*)lua_newuserdata(L, size);
        // memset(book, 0, sizeof(size));
        // Book* book = static_cast<Book*>(data);
        new (book) Book();
        

        book->Name = "";
        book->Price = 0;

        // 相当于是：setmetatable(book, BookMT)
        luaL_getmetatable(L, "BookMT");
        lua_setmetatable(L, -2);    // 设置元表，并弹栈
        
        assert(lua_isuserdata(L, 1));

        // 新建的userdata会自动压栈，所以不需要再次push
        return 1;
    }
    static int DumpBook(lua_State *L)
    {
        assert(lua_isuserdata(L, 1));
        // 第 1 步，取出lightuserdata
        struct Book *book = (struct Book *)lua_touserdata(L, 1);
        // 第 2 步
        if(book != NULL)
        {
            std::cout << "book name=" << book->Name << ", price=" << book->Price << std::endl;
        }
        return 0;
    }
    
    static const struct luaL_Reg BookMetaMethods[] = {
        { "DumpBook", DumpBook },
        { NULL, NULL }
    };
    
    // 定义metatable
    // luaL_newlib()
    static void InitLua(lua_State* L)
    {
        LuaUtils::DumpStack(L);
        
        // 新建book，相当于是 local BookMT = {}
        luaL_newmetatable(L, "BookMT");

        LuaUtils::DumpStack(L);        

        // 设置index，相当于是：BookMT.__index = BookMT
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        // 注册函数，相当于是BookMT.funcxxx = xxx
        luaL_setfuncs(L, BookMetaMethods, 0);        
        
        lua_register(L, "NewBookWithUD", NewBookWithUD);
    }
    
    
    static void TestCallCpp()
    {
        /* 初始化Lua */
        /* 指向Lua解释器的指针 */
        lua_State* L = luaL_newstate();

        /* 载入Lua基本库 */
        luaL_openlibs(L);

        InitLua(L);

        const char* luaCode = R"code(
            local b = NewBookWithUD()
            b:DumpBook()

        )code";

        LuaUtils::RunCode(L, luaCode);

        /* 清除Lua */
        lua_close(L);
    }

    static AutoRegTestFunc autoTestCallCpp(TestCallCpp);
}
