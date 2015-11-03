#include "GLRenderTarget.h"
#include "RenderTextureBinding.h"
#include "GLCache.h"

using namespace Fairy2D;

namespace LuaBinding
{
    int lBeginRender(lua_State* state)
    {
        GLCache::clear();
        auto renderTexture = (GLRenderTarget*)lua_touserdata(state, 1);
        renderTexture->beginRender();
        return 0;
    }

    int lEndRender(lua_State* state)
    {
        auto renderTexture = (GLRenderTarget*)lua_touserdata(state, 1);
        renderTexture->endRender();
        GLCache::clear();
        return 0;
    }

    int lBind(lua_State* state)
    {
        auto renderTexture = (GLRenderTarget*)lua_touserdata(state, 1);
        glActiveTexture(GL_TEXTURE0 + lua_tointeger(state, 2));
        glBindTexture(GL_TEXTURE_2D, renderTexture->getTexture());
        return 0;
    }

    int lGetWidth(lua_State* state)
    {
        auto renderTexture = (GLRenderTarget*)lua_touserdata(state, 1);
        lua_pushinteger(state, renderTexture->getTexture().getWidth());
        return 1;
    }

    int lGetHeight(lua_State* state)
    {
        auto renderTexture = (GLRenderTarget*)lua_touserdata(state, 1);
        lua_pushinteger(state, renderTexture->getTexture().getHeight());
        return 1;
    }

    int lDestroyRenderTexture(lua_State* state)
    {
        auto renderTexture = (GLRenderTarget*)lua_touserdata(state, 1);
        renderTexture->~GLRenderTarget();
        return 0;
    }

    int lCreateRenderTexture(lua_State* state)
    {
        auto renderTexture = (GLRenderTarget*)lua_newuserdata(state, sizeof(GLRenderTarget));

        new (renderTexture) GLRenderTarget;
        if (renderTexture->create(lua_tointeger(state, 1), lua_tointeger(state, 2))) {
            lua_getfield(state, LUA_REGISTRYINDEX, "__renderTexture");
            lua_setmetatable(state, -2);
            return 1;
        }
        return 0;
    }

    void SetupRenderTextureBinding(lua_State* state)
    {
        luaL_Reg regs[] = {
            {"beginRender", lBeginRender},
            {"endRender",   lEndRender},
            {"bind",        lBind},
            {"width",       lGetWidth},
            {"height",      lGetHeight},
            {"__gc",        lDestroyRenderTexture},
            {NULL, NULL}
        };

        lua_newtable(state);
        luaL_setfuncs(state, regs, 0);
        lua_pushnil(state);
        lua_copy(state, -2, -1);
        lua_setfield(state, -2, "__index");
        lua_setfield(state, LUA_REGISTRYINDEX, "__renderTexture");

        lua_register(state, "createRenderTexture", lCreateRenderTexture);
    }
}