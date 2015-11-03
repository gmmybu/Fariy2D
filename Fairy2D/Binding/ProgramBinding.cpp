#include "GLCache.h"
#include "GLVertexArray.h"
#include "GLProgram.h"

#include "ProgramBinding.h"

using namespace Fairy2D;

namespace LuaBinding
{
    int lSetUniformf(lua_State* state)
    {
        auto program = (GLProgram*)lua_touserdata(state, 1);
        GLCache::useProgram(*program);

        GLuint loc = program->getUniformLocation(lua_tostring(state, 2));
        int top = lua_gettop(state);
        if (top == 3) {
            glUniform1i(loc,
                lua_tointeger(state, 3));
        } else if (top == 4) {
            glUniform2f(loc,
                lua_tonumber(state, 3),
                lua_tonumber(state, 4));
        } else if (top == 5) {
            glUniform3f(loc,
                lua_tonumber(state, 3),
                lua_tonumber(state, 4),
                lua_tonumber(state, 5));
        } else if (top == 6) {
            glUniform4f(loc,
                lua_tonumber(state, 3),
                lua_tonumber(state, 4),
                lua_tonumber(state, 5),
                lua_tonumber(state, 6));
        }
        return 0;
    }

    int lSetUniformi(lua_State* state)
    {
        auto program = (GLProgram*)lua_touserdata(state, 1);
        GLCache::useProgram(*program);

        GLuint loc = program->getUniformLocation(lua_tostring(state, 2));
        int top = lua_gettop(state);
        if (top == 3) {
            glUniform1i(loc,
                lua_tointeger(state, 3));
        } else if (top == 4) {
            glUniform2i(loc,
                lua_tointeger(state, 3),
                lua_tointeger(state, 4));
        } else if (top == 5) {
            glUniform3i(loc,
                lua_tointeger(state, 3),
                lua_tointeger(state, 4),
                lua_tointeger(state, 5));
        } else if (top == 6) {
            glUniform4i(loc,
                lua_tointeger(state, 3),
                lua_tointeger(state, 4),
                lua_tointeger(state, 5),
                lua_tointeger(state, 6));
        }
        return 0;
    }

    int lSetUniformv(lua_State* state)
    {
        auto program = (GLProgram*)lua_touserdata(state, 1);
        GLCache::useProgram(*program);

        GLuint loc = program->getUniformLocation(lua_tostring(state, 2));
        size_t size = lua_tointeger(state, 3);
        size_t length = lua_rawlen(state, 4);

        static float data[1024];
        for (size_t i = 0; i < length; i++) {
            lua_rawgeti(state, 4, i + 1);
            data[i] = lua_tonumber(state, -1);
            lua_pop(state, 1);
        }
        if (size == 1) {
            glUniform1fv(loc, length / size, data);
        } else if (size == 2) {
            glUniform2fv(loc, length / size, data);
        } else if (size == 3) {
            glUniform3fv(loc, length / size, data);
        } else if (size == 4) {
            glUniform4fv(loc, length / size, data);
        }
        return 0;
    }

    int lSetUniformMatrix(lua_State* state)
    {
        auto program = (GLProgram*)lua_touserdata(state, 1);
        GLCache::useProgram(*program);
        
        GLuint loc = program->getUniformLocation(lua_tostring(state, 2));
        float data[16];

        for (size_t i = 0; i < 16; i++) {
            lua_rawgeti(state, 3, i + 1);
            data[i] = lua_tonumber(state, -1);
            lua_pop(state, 1);
        }
        glUniformMatrix4fv(loc, 1, TRUE, data);
        return 0;
    }

    int lDrawArrays(lua_State* state)
    {
        auto program = (GLProgram*)lua_touserdata(state, 1);

        GLCache::useProgram(*program);


        auto vertexArray = (GLVertexArray*)lua_touserdata(state, 2);
        GLCache::bindVertexArray(*vertexArray);

        glDrawArrays(lua_tointeger(state, 3), lua_tointeger(state, 4),
            lua_tointeger(state, 5));

        return 0;
    }

    int lDrawElements(lua_State* state)
    {
        auto program = (GLProgram*)lua_touserdata(state, 1);
        GLCache::useProgram(*program);

        auto vertexArray = (GLVertexArray*)lua_touserdata(state, 2);
        GLCache::bindVertexArray(*vertexArray);
     
        glDrawElements(lua_tointeger(state, 3), lua_tointeger(state, 4),
            GL_UNSIGNED_INT, (GLvoid*)lua_tointeger(state, 5));
        return 0;
    }

    int lDestroyProgram(lua_State* state)
    {
        auto program = (GLProgram*)lua_touserdata(state, 1);
        program->~GLProgram();
        return 0;
    }

    int lCreateProgram(lua_State* state)
    {
        auto program = (GLProgram*)lua_newuserdata(state, sizeof(GLProgram));

        new (program) GLProgram;
        if (program->create(lua_tostring(state, 1), lua_tostring(state, 2))) {
            lua_getfield(state, LUA_REGISTRYINDEX, "__program");
            lua_setmetatable(state, -2);
            return 1;
        }
        return 0;
    }

    void SetupProgramBinding(lua_State* state)
    {
        luaL_Reg regs[] = {
            {"setUniformf",      lSetUniformf},
            {"setUniformi",      lSetUniformi},
            {"setUniformv",      lSetUniformv},
            {"setUniformMatrix", lSetUniformMatrix},

            {"drawArrays",       lDrawArrays},
            {"drawElements",     lDrawElements},

            {"__gc",             lDestroyProgram},
            {NULL,  NULL}
        };

        lua_newtable(state);
        luaL_setfuncs(state, regs, 0);

        lua_pushnil(state);
        lua_copy(state, -2, -1);
        lua_setfield(state, -2, "__index");
        lua_setfield(state, LUA_REGISTRYINDEX, "__program");

        lua_register(state, "createProgram", lCreateProgram);
    }
}