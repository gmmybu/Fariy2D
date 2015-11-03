#include "GLCache.h"
#include "GLBuffer.h"
#include "GLVertexArray.h"
#include "VAOBinding.h"

using namespace Fairy2D;

namespace LuaBinding
{
    void lBindVertexArrayBuffer(lua_State* state, int pos)
    {
        lua_getfield(state, LUA_REGISTRYINDEX, "__vertexArrayBinding"); //b
        lua_pushnil(state);                                             //b nil                
        lua_copy(state, 1, -1);                                         //b v
        lua_gettable(state, -2);                                        //b t
        lua_pushnil(state);                                             //b t nil        
        lua_copy(state, 2, -1);                                         //b t bf
        lua_rawseti(state, -2, pos);                                    //b t
        lua_pop(state, 2);
    }

    void lUnBindVertexArrayBuffer(lua_State* state, int pos)
    {
        lua_getfield(state, LUA_REGISTRYINDEX, "__vertexArrayBinding"); //b
        lua_pushnil(state);                                             //b nil                
        lua_copy(state, 1, -1);                                         //b v
        lua_gettable(state, -2);                                        //b t
        lua_pushnil(state);                                             //b t nil        
        lua_rawseti(state, -2, lua_tointeger(state, 3));                //b t
        lua_pop(state, 2);
    }

    int lVertexArrayElement(lua_State* state)
    {
        auto vertexArray = (GLVertexArray*)lua_touserdata(state, 1);
        auto vertexBuffer = (GLBuffer*)lua_touserdata(state, 2);
        GLCache::bindVertexArray(*vertexArray);
        
        if (vertexBuffer != NULL) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *vertexBuffer);

            lBindVertexArrayBuffer(state, 1024);
        } else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            lUnBindVertexArrayBuffer(state, 1024);
        }
        return 0;
    }

    int lVertexAttribPointer(lua_State* state)
    {
        auto vertexArray = (GLVertexArray*)lua_touserdata(state, 1);
        auto vertexBuffer = (GLBuffer*)lua_touserdata(state, 2);
        GLCache::bindVertexArray(*vertexArray);

        if (vertexBuffer != NULL) {
            glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
            glVertexAttribPointer(lua_tointeger(state, 3),
                lua_tointeger(state, 4),
                GL_FLOAT, TRUE,
                lua_tointeger(state, 5), 
                (const GLvoid*)lua_tointeger(state, 6));
            glEnableVertexAttribArray(lua_tointeger(state, 3));

            lBindVertexArrayBuffer(state, lua_tointeger(state, 3));
        } else {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glVertexAttribPointer(lua_tointeger(state, 3),
                4, GL_FLOAT, TRUE, 0, 0);
            glDisableVertexAttribArray(lua_tointeger(state, 3));

            lUnBindVertexArrayBuffer(state, lua_tointeger(state, 3));
        }
        return 0;
    }

    int lDestroyVertexArray(lua_State* state)
    {
        auto vertexArray = (GLVertexArray*)lua_touserdata(state, 1);
        vertexArray->~GLVertexArray();
        return 0;
    }

    int lCreateVertexArray(lua_State* state)
    {
        auto vertexArray = (GLVertexArray*)lua_newuserdata(state, sizeof(GLVertexArray)); //u
        new (vertexArray) GLVertexArray;
        lua_getfield(state, LUA_REGISTRYINDEX, "__vertexArray");                          //u meta
        lua_setmetatable(state, -2);                                                      //u

        lua_getfield(state, LUA_REGISTRYINDEX, "__vertexArrayBinding");                   //u b
        lua_pushnil(state);                                                               //u b nil
        lua_copy(state, -3, -1);                                                          //u b u
        lua_newtable(state);                                                              //u b u t
        lua_settable(state, -3);                                                          //u b  
        lua_pop(state, 1);                                                                //u
        return 1;
    }

    void SetupVertexArrayBinding(lua_State* state)
    {
        luaL_Reg regs[] = {
            {"bindElement", lVertexArrayElement},
            {"bindBuffer",  lVertexAttribPointer},
            {"__gc",        lDestroyVertexArray},
            {NULL, NULL},
        };

        lua_newtable(state);
        luaL_setfuncs(state, regs, 0);
        lua_pushnil(state);
        lua_copy(state, -2, -1);
        lua_setfield(state, -2, "__index");
        lua_setfield(state, LUA_REGISTRYINDEX, "__vertexArray");

        lua_newtable(state);

        lua_newtable(state);
        lua_pushstring(state, "k");
        lua_setfield(state, -2, "__mode");
        lua_setmetatable(state, -2);
        lua_setfield(state, LUA_REGISTRYINDEX, "__vertexArrayBinding");

        lua_register(state, "createVertexArray", lCreateVertexArray);
    }
}