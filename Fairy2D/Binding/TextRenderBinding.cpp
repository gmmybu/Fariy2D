#include "System.h"
#include "TextRender.h"
#include "TextRenderBinding.h"

using namespace System;

namespace LuaBinding
{
    int lCharWidth(lua_State* state)
    {
        auto render = (TextRender*)lua_touserdata(state, 1);
        auto text = Encoding::UTF8ToUnicode(lua_tostring(state, 2));
        lua_pushinteger(state, render->charWidth(text[0]));
        return 1;
    }

    int lCharHeight(lua_State* state)
    {
        auto render = (TextRender*)lua_touserdata(state, 1);
        lua_pushinteger(state, render->charHeight());
        return 1;
    }

    int lMeasure(lua_State* state)
    {
        auto render = (TextRender*)lua_touserdata(state, 1);
        auto text = Encoding::UTF8ToUnicode(lua_tostring(state, 2));
        lua_pushinteger(state, render->measure(text));
        return 1;
    }

    int lRender(lua_State* state)
    {
        auto render = (TextRender*)lua_touserdata(state, 1);
        auto text = Encoding::UTF8ToUnicode(lua_tostring(state, 2));

        float color[4];
        for (size_t i = 0; i < 4; i++) {
            lua_rawgeti(state, 5, i + 1);
            color[i] = lua_tonumber(state, -1);
            lua_pop(state, 1);
        }

        float matrix[16];
        for (size_t i = 0; i < 16; i++) {
            lua_rawgeti(state, 6, i + 1);
            matrix[i] = lua_tonumber(state, -1);
            lua_pop(state, 1);
        }

        Color fcolor;
        memcpy(&fcolor, color, sizeof(Color));

        lua_pushinteger(state, render->render(text, lua_tonumber(state, 3),
            lua_tonumber(state, 4), fcolor, matrix));
        return 1;
    }

    int lDestroyTextRender(lua_State* state)
    {
        auto render = (TextRender*)lua_touserdata(state, 1);
        render->~TextRender();
        return 0;
    }

    int lCreateTextRender(lua_State* state)
    {
        auto fontface = Encoding::UTF8ToUnicode(lua_tostring(state, 1));
        int  fontsize = lua_tointeger(state, 2);

        auto render = (TextRender*)lua_newuserdata(state, sizeof(TextRender));
        new (render) TextRender(fontface, fontsize);
        lua_getfield(state, LUA_REGISTRYINDEX, "__textRender");
        lua_setmetatable(state, -2);
        return 1;
    }

    void SetupTextRenderBinding(lua_State* state)
    {
        luaL_Reg regs[] = {
            {"charWidth",  lCharWidth},
            {"charHeight", lCharHeight},
            {"measure",    lMeasure},
            {"render",   lRender},
            {"__gc",       lDestroyTextRender},
            {NULL, NULL}
        };

        lua_newtable(state);
        luaL_setfuncs(state, regs, 0);
        lua_pushnil(state);
        lua_copy(state, -2, -1);
        lua_setfield(state, -2, "__index");
        lua_setfield(state, LUA_REGISTRYINDEX, "__textRender");

        lua_register(state, "createTextRenderer", lCreateTextRender);
    }
}