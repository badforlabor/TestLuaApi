#include <iostream>  
#include <string.h>  
using namespace std;

extern "C"
{
#include "lua.h"  
#include "lauxlib.h"  
#include "lualib.h"  
}

int main()
{
	//1.创建一个state  
	// luaL_newstate返回一个指向堆栈的指针
	lua_State* L = luaL_newstate();

	//2.入栈操作  
	lua_pushstring(L, "I am so cool~");
	lua_pushnumber(L, 20);


	//3.取值操作  
	if (lua_isstring(L, 1)) {             //判断是否可以转为string  
		cout << lua_tostring(L, 1) << endl;  //转为string并返回  
	}
	if (lua_isnumber(L, 2)) {
		cout << lua_tonumber(L, 2) << endl;
	}

	//4.关闭state  
	lua_close(L);

	system("pause");
	return 0;
}
