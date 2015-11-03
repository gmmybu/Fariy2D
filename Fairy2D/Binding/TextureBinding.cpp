#include "System.h"
#include "FileSystem.h"

#include "stb_image.h"
#include "GLTexture.h"
#include "TextureBinding.h"

using namespace System;
using namespace Fairy2D;

namespace LuaBinding
{
    int lBindTexture(lua_State* state)
    {
        auto texture = (GLTexture*)lua_touserdata(state, 1);
        glActiveTexture(GL_TEXTURE0 + lua_tointeger(state, 2));
        glBindTexture(GL_TEXTURE_2D, *texture);
        return 0;
    }

    int lGetTextureWidth(lua_State* state)
    {
        auto texture = (GLTexture*)lua_touserdata(state, 1);
        lua_pushinteger(state, texture->getWidth());
        return 1;
    }

    int lGetTextureHeight(lua_State* state)
    {
        auto texture = (GLTexture*)lua_touserdata(state, 1);
        lua_pushinteger(state, texture->getHeight());
        return 1;
    }

    int lDestroyTexture(lua_State* state)
    {
        auto texture = (GLTexture*)lua_touserdata(state, 1);
        texture->~GLTexture();
        return 0;
    }

    int lLoadTextureFromFile(lua_State* state)
    {
        FileSystem* resource = FileSystem::current();

        size_t size;
        void* buff = resource->read(Encoding::UTF8ToUnicode(lua_tostring(state, 1)), size);
        GLASSERT(buff != NULL);

        int x, y, comp = 4;
        stbi_uc* data = stbi_load_from_memory((stbi_uc*)buff, size, &x, &y, &comp, 4);
        GLASSERT(data != NULL);

        auto texture = (GLTexture*)lua_newuserdata(state, sizeof(GLTexture));       //texture

        new (texture) GLTexture;
        texture->create(x, y, NULL);
        texture->update(0, 0, x, y, data);
        stbi_image_free(data);
        resource->free(buff);

        lua_getfield(state, LUA_REGISTRYINDEX, "__texture");                        //texture m
        lua_setmetatable(state, -2);                                                //texture        
        return 1;
    }

    void SetupTextureBinding(lua_State* state)
    {
        luaL_Reg regs[] = {
            {"bind",    lBindTexture},
            {"width",   lGetTextureWidth},
            {"height",  lGetTextureHeight},
            {"__gc",    lDestroyTexture},
            {NULL, NULL}
        };

        lua_newtable(state);
        luaL_setfuncs(state, regs, 0);

        lua_pushnil(state);
        lua_copy(state, -2, -1);
        lua_setfield(state, -2, "__index");

        lua_setfield(state, LUA_REGISTRYINDEX, "__texture");

        lua_register(state, "loadTexture", lLoadTextureFromFile);
    }
}