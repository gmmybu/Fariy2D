#pragma once
#include "lua.hpp"

namespace LuaBinding
{
    template<class T>
    struct converter;

    template<>
    struct converter<int>
    {
        static int convert(lua_State* state, int pos)
        {
            return lua_tointeger(state, pos);
        }

        static void forward(lua_State* state, int val)
        {
            lua_pushinteger(state, val);
        }
    };

    template<>
    struct converter<unsigned int>
    {
        static unsigned int convert(lua_State* state, int pos)
        {
            return lua_tounsigned(state, pos);
        }

        static void forward(lua_State* state, unsigned int val)
        {
            lua_pushunsigned(state, val);
        }
    };

    template<>
    struct converter<unsigned char>
    {
        static unsigned char convert(lua_State* state, int pos)
        {
            return lua_tounsigned(state, pos);
        }

        static void forward(lua_State* state, unsigned char val)
        {
            lua_pushunsigned(state, val);
        }
    };

    template<>
    struct converter<signed char>
    {
        static signed char convert(lua_State* state, int pos)
        {
            return lua_tointeger(state, pos);
        }

        static void forward(lua_State* state, signed char val)
        {
            lua_pushinteger(state, val);
        }
    };

    template<>
    struct converter<short>
    {
        static short convert(lua_State* state, int pos)
        {
            return lua_tointeger(state, pos);
        }

        static void forward(lua_State* state, short val)
        {
            lua_pushinteger(state, val);
        }
    };

    template<>
    struct converter<unsigned short>
    {
        static unsigned short convert(lua_State* state, int pos)
        {
            return lua_tounsigned(state, pos);
        }

        static void forward(lua_State* state, unsigned short val)
        {
            lua_pushunsigned(state, val);
        }
    };

    template<>
    struct converter<unsigned long>
    {
        static unsigned long convert(lua_State* state, int pos)
        {
            return lua_tounsigned(state, pos);
        }

        static void forward(lua_State* state, unsigned long val)
        {
            lua_pushunsigned(state, val);
        }
    };

    template<>
    struct converter<float>
    {
        static float convert(lua_State* state, int pos)
        {
            return (float)lua_tonumber(state, pos);
        }

        static void forward(lua_State* state, float val)
        {
            lua_pushnumber(state, val);
        }
    };

    template<>
    struct converter<double>
    {
        static double convert(lua_State* state, int pos)
        {
            return lua_tonumber(state, pos);
        }

        static void forward(lua_State* state, double val)
        {
            lua_pushnumber(state, val);
        }
    };

    template<class T>
    struct realcall
    {
        template<typename F>
        static int call(lua_State* state, F f)
        {
            converter<T>::forward(state, f());
            return 1;
        }

        template<typename F, typename P1>
        static int call(lua_State* state, F f, P1 p1)
        {
            converter<T>::forward(state, f(p1));
            return 1;
        }

        template<typename F, typename P1, typename P2>
        static int call(lua_State* state, F f, P1 p1, P2 p2)
        {
            converter<T>::forward(state, f(p1, p2));
            return 1;
        }

        template<typename F, typename P1, typename P2, typename P3>
        static int call(lua_State* state, F f, P1 p1, P2 p2, P3 p3)
        {
            converter<T>::forward(state, f(p1, p2, p3));
            return 1;
        }

        template<typename F, typename P1, typename P2, typename P3, typename P4>
        static int call(lua_State* state, F f, P1 p1, P2 p2, P3 p3, P4 p4)
        {
            converter<T>::forward(state, f(p1, p2, p3, p4));
            return 1;
        }
    };

    template<>
    struct realcall<void>
    {
        template<typename F>
        static int call(lua_State* state, F f)
        {
            f();
            return 0;
        }

        template<typename F, typename P1>
        static int call(lua_State* state, F f, P1 p1)
        {
            f(p1);
            return 0;
        }

        template<typename F, typename P1, typename P2>
        static int call(lua_State* state, F f, P1 p1, P2 p2)
        {
            f(p1, p2);
            return 0;
        }

        template<typename F, typename P1, typename P2, typename P3>
        static int call(lua_State* state, F f, P1 p1, P2 p2, P3 p3)
        {
            f(p1, p2, p3);
            return 0;
        }

        template<typename F, typename P1, typename P2, typename P3, typename P4>
        static int call(lua_State* state, F f, P1 p1, P2 p2, P3 p3, P4 p4)
        {
            f(p1, p2, p3, p4);
            return 0;
        }
    };

    template <typename T>
    struct invoker { };

    template<typename R>
    struct invoker<R GLAPIENTRY ()>
    {
        typedef R GLAPIENTRY Func();

        static int invoke(Func f, lua_State* state)
        {
            return realcall<R>::call(state, f);
        }
    };

    template<typename R>
    struct invoker<R (GLAPIENTRY *)()>
    {
        typedef R GLAPIENTRY Func();

        static int invoke(Func f, lua_State* state)
        {
            return realcall<R>::call(state, f);
        }
    };

    template <typename R, typename P1>
    struct invoker<R GLAPIENTRY (P1)>
    {
        typedef R GLAPIENTRY Func(P1);

        static int invoke(Func f, lua_State* state)
        {
            return realcall<R>::call(state, f, converter<P1>::convert(state, 1));
        }
    };

    template <typename R, typename P1>
    struct invoker<R (GLAPIENTRY *)(P1)>
    {
        typedef R GLAPIENTRY Func(P1);

        static int invoke(Func f, lua_State* state)
        {
            return realcall<R>::call(state, f, converter<P1>::convert(state, 1));
        }
    };

    template<typename R, class P1, class P2>
    struct invoker<R GLAPIENTRY (P1, P2)>
    {
        typedef R GLAPIENTRY Func(P1, P2);

        static int invoke(Func f, lua_State* state)
        {
            return realcall<R>::call(state, f, converter<P1>::convert(state, 1), converter<P2>::convert(state, 2));
        }
    };

    template<typename R, class P1, class P2>
    struct invoker<R (GLAPIENTRY *)(P1, P2)>
    {
        typedef R GLAPIENTRY Func(P1, P2);

        static int invoke(Func f, lua_State* state)
        {
            return realcall<R>::call(state, f, converter<P1>::convert(state, 1), converter<P2>::convert(state, 2));
        }
    };

    template<typename R, class P1, class P2, class P3>
    struct invoker<R GLAPIENTRY (P1, P2, P3)>
    {
        typedef R GLAPIENTRY Func(P1, P2, P3);

        static int invoke(Func f, lua_State* state)
        {
            return realcall<R>::call(state, f, converter<P1>::convert(state, 1), converter<P2>::convert(state, 2),
                converter<P3>::convert(state, 3));
        }
    };

    template<typename R, class P1, class P2, class P3>
    struct invoker<R (GLAPIENTRY *)(P1, P2, P3)>
    {
        typedef R GLAPIENTRY Func(P1, P2, P3);

        static int invoke(Func f, lua_State* state)
        {
            return realcall<R>::call(state, f, converter<P1>::convert(state, 1), converter<P2>::convert(state, 2),
                converter<P3>::convert(state, 3));
        }
    };

    template<typename R, class P1, class P2, class P3, class P4>
    struct invoker<R GLAPIENTRY (P1, P2, P3, P4)>
    {
        typedef R GLAPIENTRY Func(P1, P2, P3, P4);

        static int invoke(Func f, lua_State* state)
        {
            return realcall<R>::call(state, f, converter<P1>::convert(state, 1), converter<P2>::convert(state, 2),
                converter<P3>::convert(state, 3), converter<P4>::convert(state, 4));
        }
    };

    template<typename R, class P1, class P2, class P3, class P4>
    struct invoker<R (GLAPIENTRY *)(P1, P2, P3, P4)>
    {
        typedef R GLAPIENTRY Func(P1, P2, P3, P4);

        static int invoke(Func f, lua_State* state)
        {
            return realcall<R>::call(state, f, converter<P1>::convert(state, 1), converter<P2>::convert(state, 2),
                converter<P3>::convert(state, 3), converter<P4>::convert(state, 4));
        }
    };

#define LUA_NEW(func)                                           \
int l##func(lua_State* state)                                   \
{                                                               \
    return invoker<decltype(gl##func)>::invoke(gl##func, state);\
}

#define LUA_REG(func) {"gl"#func, l##func},

}
