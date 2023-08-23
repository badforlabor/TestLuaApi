/**
 * Auth :   liubo
 * Date :   2023-08-22 10:30
 * Comment: 练习UserData
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

namespace test_luauserdata
{
    struct Book
    {
        ~Book()
        {
            std::cout << "~Book" << std::endl;
        }
        std::string Name;
        float Price;
    };
    static int NewBook(lua_State *L)
    {
        // 对象分配在C语言的堆内存中，并不在Lua中
        Book *book = new Book();

        book->Name = "";
        book->Price = 0;        
        
        lua_pushlightuserdata(L, book);
 
        return 1; // 新的lightuserdata已经在栈上了
    }
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

        // 新建的userdata会自动压栈，所以不需要再次push
        return 1;
    }
    static int ChangeBook(lua_State *L)
    {
#if true
        assert(lua_isuserdata(L, 1));
        assert(lua_isstring(L, 2));
        assert(lua_isnumber(L, 3));
        // 第 1 步，取出lightuserdata
        struct Book *book = (struct Book *)lua_touserdata(L, 1);
        string s = lua_tostring(L, 2);
        float v = lua_tonumber(L, 3);
#else
        assert(lua_isuserdata(L, -3));
        assert(lua_isstring(L, -2));
        assert(lua_isnumber(L, -1));
        // 第 1 步
        struct Book *book = (struct Book *)lua_touserdata(L, -3);
        string s = lua_tostring(L, -2);
        float v = lua_tonumber(L, -1);        
#endif
        // 第 2 步
        if(book != NULL)
        {
            book->Name = s;
            book->Price = v;
        }
        return 0;
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
    static int DeleteBook(lua_State *L)
    {
        assert(lua_isuserdata(L, 1));
        // 第 1 步，取出lightuserdata
        struct Book *book = (struct Book *)lua_touserdata(L, 1);
        // 第 2 步
        if(book != NULL)
        {
            delete book;
        }
        return 0;
    }
    
    static void TestUserData()
    {
        /* 初始化Lua */
        /* 指向Lua解释器的指针 */
        lua_State* L = luaL_newstate();

        /* 载入Lua基本库 */
        luaL_openlibs(L);

        lua_register(L, "NewBook", NewBook);
        lua_register(L, "NewBookWithUD", NewBookWithUD);
        lua_register(L, "ChangeBook", ChangeBook);
        lua_register(L, "DumpBook", DumpBook);
        lua_register(L, "DeleteBook", DeleteBook);


        const char* luaCode = R"delimiter(
            local b = NewBook()
            DumpBook(b)
            ChangeBook(b, "123", 4.5)
            DumpBook(b)
            DeleteBook(b)   -- lightuserdata，需要自己控制声明周期
            b = nil

            local b = NewBookWithUD()
            DumpBook(b)
            ChangeBook(b, "123", 4.5)
            DumpBook(b)
            --DeleteBook(b) -- 如果是userdata，不能是delete
            b = nil
        )delimiter";

        LuaUtils::RunCode(L, luaCode);

        /* 清除Lua */
        lua_close(L);
    }

    static AutoRegTestFunc autoTestUserData(TestUserData);
}
