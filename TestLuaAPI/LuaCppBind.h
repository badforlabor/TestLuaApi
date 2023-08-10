/**
 * Auth :   liubo
 * Date :   2023-08-10 00:00:00
 * Comment: 绑定C++函数的一些工具
 *  自动注册！参考：https://github.com/siddontang/luahelper/blob/master/reghelper.h
 */

#pragma once
#include <windows.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace MyLuaBind
{
    template <typename T>
    struct TypeHelper
    {
    };

    inline void GetValue(TypeHelper<void>, lua_State* pState, int index)
    {
    }

    inline bool GetValue(TypeHelper<bool>, lua_State* pState, int index)
    {
        return lua_toboolean(pState, index) == 1;
    }

    inline char GetValue(TypeHelper<char>, lua_State* pState, int index)
    {
        return static_cast<char>(lua_tonumber(pState, index));
    }

    inline short GetValue(TypeHelper<short>, lua_State* pState, int index)
    {
        return static_cast<short>(lua_tonumber(pState, index));
    }

    inline int GetValue(TypeHelper<int>, lua_State* pState, int index)
    {
        return static_cast<int>(lua_tonumber(pState, index));
    }

    inline long GetValue(TypeHelper<long>, lua_State* pState, int index)
    {
        return static_cast<long>(lua_tonumber(pState, index));
    }

    inline unsigned char GetValue(TypeHelper<unsigned char>, lua_State* pState, int index)
    {
        return static_cast<unsigned char>(lua_tonumber(pState, index));
    }

    inline unsigned short GetValue(TypeHelper<unsigned short>, lua_State* pState, int index)
    {
        return static_cast<unsigned short>(lua_tonumber(pState, index));
    }

    inline unsigned int GetValue(TypeHelper<unsigned int>, lua_State* pState, int index)
    {
        return static_cast<unsigned int>(lua_tonumber(pState, index));
    }

    inline unsigned long GetValue(TypeHelper<unsigned long>, lua_State* pState, int index)
    {
        return static_cast<unsigned long>(lua_tonumber(pState, index));
    }

    inline char* GetValue(TypeHelper<char*>, lua_State* pState, int index)
    {
        return const_cast<char*>(lua_tostring(pState, index));
    }

    inline const char* GetValue(TypeHelper<const char*>, lua_State* pState, int index)
    {
        return lua_tostring(pState, index);
    }

    inline float GetValue(TypeHelper<float>, lua_State* pState, int index)
    {
        return static_cast<float>(lua_tonumber(pState, index));
    }

    inline double GetValue(TypeHelper<double>, lua_State* pState, int index)
    {
        return static_cast<double>(lua_tonumber(pState, index));
    }

    inline void PushValue(lua_State* pState, bool value)
    {
        lua_pushboolean(pState, int(value));
    }

    inline void PushValue(lua_State* pState, char value)
    {
        lua_pushnumber(pState, value);
    }

    inline void PushValue(lua_State* pState, short value)
    {
        lua_pushnumber(pState, value);
    }

    inline void PushValue(lua_State* pState, int value)
    {
        lua_pushnumber(pState, value);
    }

    inline void PushValue(lua_State* pState, long value)
    {
        lua_pushnumber(pState, value);
    }

    inline void PushValue(lua_State* pState, unsigned char value)
    {
        lua_pushnumber(pState, value);
    }

    inline void PushValue(lua_State* pState, unsigned short value)
    {
        lua_pushnumber(pState, value);
    }

    inline void PushValue(lua_State* pState, unsigned int value)
    {
        lua_pushnumber(pState, value);
    }

    inline void PushValue(lua_State* pState, unsigned long value)
    {
        lua_pushnumber(pState, value);
    }

    inline void PushValue(lua_State* pState, char* value)
    {
        lua_pushstring(pState, value);
    }

    inline void PushValue(lua_State* pState, const char* value)
    {
        lua_pushstring(pState, value);
    }

    inline void PushValue(lua_State* pState, float value)
    {
        lua_pushnumber(pState, value);
    }

    inline void PushValue(lua_State* pState, double value)
    {
        lua_pushnumber(pState, value);
    }

#ifdef OLD_CODE
    template <typename Ret>
    class CCallHelper
    {
    public:
        static int Call(Ret (*func)(), lua_State* pState)
        {
            Ret ret = (*func)();
            PushValue(pState, ret);
            return 1;
        }

        template <typename P1>
        static int Call(Ret (*func)(P1), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            Ret ret = (*func)(p1);
            PushValue(pState, ret);
            return 1;
        }

        template <typename P1, typename P2>
        static int Call(Ret (*func)(P1, P2), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            P2 p2 = GetValue(TypeHelper<P2>(), pState, 2);
            Ret ret = (*func)(p1, p2);
            PushValue(pState, ret);
            return 1;
        }

        template <typename P1, typename P2, typename P3>
        static int Call(Ret (*func)(P1, P2, P3), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            P2 p2 = GetValue(TypeHelper<P2>(), pState, 2);
            P3 p3 = GetValue(TypeHelper<P3>(), pState, 3);
            Ret ret = (*func)(p1, p2, p3);
            PushValue(pState, ret);
            return 1;
        }
    };

    template <>
    class CCallHelper<void>
    {
    public:
        static int Call(void (*func)(), lua_State* pState)
        {
            (*func)();
            return 0;
        }

        template <typename P1>
        static int Call(void (*func)(P1), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            (*func)(p1);
            return 0;
        }

        template <typename P1, typename P2>
        static int Call(void (*func)(P1, P2), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            P2 p2 = GetValue(TypeHelper<P2>(), pState, 2);
            (*func)(p1, p2);
            return 0;
        }

        template <typename P1, typename P2, typename P3>
        static int Call(void (*func)(P1, P2, P3), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            P2 p2 = GetValue(TypeHelper<P2>(), pState, 2);
            P3 p3 = GetValue(TypeHelper<P3>(), pState, 3);
            (*func)(p1, p2, p3);
            return 0;
        }
    };
#endif

    // 声明
    template <typename Ret, typename... ARGS>
    struct CCallHelperArgs
    {
        static int Call(Ret (*func)(ARGS...), lua_State* pState)
        {
            throw new std::exception("not implement!");
            return 1;
        }
    };

    // 偏特化
    template <typename Ret>
    struct CCallHelperArgs<Ret>
    {
        static int Call(Ret (*func)(), lua_State* pState)
        {
            Ret ret = (*func)();
            PushValue(pState, ret);
            return 1;
        }
    };

    template <typename Ret, typename P1>
    struct CCallHelperArgs<Ret, P1>
    {
        static int Call(Ret (*func)(P1), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            Ret ret = (*func)(p1);
            PushValue(pState, ret);
            return 1;
        }
    };

    template <typename Ret, typename P1, typename P2>
    struct CCallHelperArgs<Ret, P1, P2>
    {
        static int Call(Ret (*func)(P1, P2), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            P2 p2 = GetValue(TypeHelper<P2>(), pState, 2);
            Ret ret = (*func)(p1, p2);
            PushValue(pState, ret);
            return 1;
        }
    };

    template <typename Ret, typename P1, typename P2, typename P3>
    struct CCallHelperArgs<Ret, P1, P2, P3>
    {
        static int Call(Ret (*func)(P1, P2, P3), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            P2 p2 = GetValue(TypeHelper<P2>(), pState, 2);
            P3 p3 = GetValue(TypeHelper<P3>(), pState, 3);
            Ret ret = (*func)(p1, p2, p3);
            PushValue(pState, ret);
            return 1;
        }
    };

    template <>
    struct CCallHelperArgs<void>
    {
        static int Call(void (*func)(), lua_State* pState)
        {
            (*func)();
            return 0;
        }
    };

    // 特例化void类型
    template <typename P1>
    struct CCallHelperArgs<void, P1>
    {
        static int Call(void (*func)(P1), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            (*func)(p1);
            return 0;
        }
    };

    template <typename P1, typename P2>
    struct CCallHelperArgs<void, P1, P2>
    {
        static int Call(void (*func)(P1, P2), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            P2 p2 = GetValue(TypeHelper<P2>(), pState, 2);
            (*func)(p1, p2);
            return 0;
        }
    };

    template <typename P1, typename P2, typename P3>
    struct CCallHelperArgs<void, P1, P2, P3>
    {
        static int Call(void (*func)(P1, P2, P3), lua_State* pState)
        {
            P1 p1 = GetValue(TypeHelper<P1>(), pState, 1);
            P2 p2 = GetValue(TypeHelper<P2>(), pState, 2);
            P3 p3 = GetValue(TypeHelper<P3>(), pState, 3);
            (*func)(p1, p2, p3);
            return 0;
        }
    };

    template <typename Func>
    class CCallDispatcherArgs
    {
    public:
        template <typename Ret, typename... Args>
        static int Dispatch(lua_State* pState, Ret (*func)(Args...))
        {
            return CCallHelperArgs<Ret, Args...>::Call(func, pState);
        }
    };

#ifdef OLD_CODE
    template <typename Func>
    class CCallDispatcher
    {
    public:        
        template <typename Ret>
        static int Dispatch(Ret (*func)(), lua_State* pState)
        {
            return CCallHelperArgs<Ret>::Call(func, pState);
        }

        template <typename Ret, typename P1>
        static int Dispatch(Ret (*func)(P1), lua_State* pState)
        {
            return CCallHelperArgs<Ret, P1>::Call(func, pState);
        }

        template <typename Ret, typename P1, typename P2>
        static int Dispatch(Ret (*func)(P1, P2), lua_State* pState)
        {
            return CCallHelperArgs<Ret, P1, P2>::Call(func, pState);
        }

        template <typename Ret, typename P1, typename P2, typename P3>
        static int Dispatch(Ret (*func)(P1, P2, P3), lua_State* pState)
        {
            return CCallHelperArgs<Ret, P1, P2, P3>::Call(func, pState);
        }
    };
#endif

    inline void* GetRegFunction(lua_State* pState)
    {
        return lua_touserdata(pState, lua_upvalueindex(1));
    }


    template <typename Func>
    class CCallRegister
    {
    public:
        static int Call(lua_State* pState)
        {
            Func* func = static_cast<Func*>(GetRegFunction(pState));            
#ifdef OLD_CODE
            return CCallDispatcher<Func>::Dispatch(*func, pState);
#else
            return CCallDispatcherArgs<Func>::Dispatch(pState, *func);
#endif
        }
    };


    template <typename Func>
    void RegFunction(lua_State* pState, Func func, const char* funcName)
    {
        int funcSize = sizeof(Func);
        void* data = lua_newuserdata(pState, funcSize);
        memcpy(data, &func, funcSize);

        lua_pushcclosure(pState, CCallRegister<Func>::Call, 1);
        lua_setglobal(pState, funcName);
    }
}

#define REGISTER_FUNCTION(state, func) MyLuaBind::RegFunction(state, func, #func)
