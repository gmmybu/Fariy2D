#include "GLBuffer.h"
#include "VBOBinding.h"

using namespace Fairy2D;

namespace LuaBinding
{
    int lVertexBufferDataf(lua_State* state)
    {
        GLBuffer* buffer = (GLBuffer*)lua_touserdata(state, 1);
        size_t    length = lua_rawlen(state, 3);
        float*    data   = new float[length];
        for (size_t i = 0; i < length; i++) {
            lua_rawgeti(state, 3, i + 1);
            data[i] = lua_tonumber(state, -1);
            lua_pop(state, 1);
        }

        glBindBuffer(GL_ARRAY_BUFFER, *buffer);
        glBufferSubData(GL_ARRAY_BUFFER, lua_tointeger(state, 2), length * 4, data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        delete[] data;
        return 0;
    }

    int lVertexBufferDatai(lua_State* state)
    {
        GLBuffer* buffer = (GLBuffer*)lua_touserdata(state, 1);
        size_t    length = lua_rawlen(state, 3);
        GLuint*   data   = new GLuint[length];
        for (size_t i = 0; i < length; i++) {
            lua_rawgeti(state, 3, i + 1);
            data[i] = lua_tointeger(state, -1);
            lua_pop(state, 1);
        }

        glBindBuffer(GL_ARRAY_BUFFER, *buffer);
        glBufferSubData(GL_ARRAY_BUFFER, lua_tointeger(state, 2), length * 4, data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        delete[] data;
        return 0;
    }

    int lDestroyVertexBuffer(lua_State* state)
    {
        GLBuffer* buffer = (GLBuffer*)lua_touserdata(state, 1);
        buffer->~GLBuffer();
        OutputDebugString(L"lDestroyVertexBuffer.\n");
        return 0;
    }

    int lCreateVertexBuffer(lua_State* state)
    {
        GLBuffer* buffer = (GLBuffer*)lua_newuserdata(state, sizeof(GLBuffer));
        new (buffer) GLBuffer;
        buffer->create(lua_tointeger(state, 1), NULL);

        lua_getfield(state, LUA_REGISTRYINDEX, "__vertexBuffer");
        lua_setmetatable(state, -2);
        return 1;
    }

    void SetupVertexBufferBinding(lua_State* state)
    {
        luaL_Reg regs[] = {
            {"copyf",   lVertexBufferDataf},
            {"copyi",   lVertexBufferDatai},
            {"__gc",    lDestroyVertexBuffer},
            {NULL, NULL}
        };

        lua_newtable(state);
        luaL_setfuncs(state, regs, 0);
        lua_pushnil(state);
        lua_copy(state, -2, -1);
        lua_setfield(state, -2, "__index");
        lua_setfield(state, LUA_REGISTRYINDEX, "__vertexBuffer");

        lua_register(state, "createVertexBuffer", lCreateVertexBuffer);
    }
}