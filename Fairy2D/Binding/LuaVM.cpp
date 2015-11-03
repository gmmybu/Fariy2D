#include "System.h"
#include "GLCache.h"
#include "GLDevice.h"

#include "VAOBinding.h"
#include "VBOBinding.h"
#include "OpenGLBinding.h"
#include "ProgramBinding.h"
#include "TextureBinding.h"
#include "TextRenderBinding.h"
#include "RenderTextureBinding.h"
#include "GameWindow.h"

#include "TextRender.h"

#include "LuaVM.h"
#include "tcmalloc.h"

using namespace System;
using namespace Fairy2D;
using namespace LuaBinding;

LuaVM::LuaVM()
{
    _L = NULL;
}

LuaVM::~LuaVM()
{
    close();
}

static int lPrint(lua_State* state)
{
    OutputDebugStringA(lua_tostring(state, 1));
    return 0;
}

void* lmalloc(void* ud, void* ptr, size_t osize, size_t nsize)
{
    if (nsize == 0) {
        tc_cfree(ptr);
        return NULL;
    } else {
        return tc_realloc(ptr, nsize);
    }
}

int LuaVM::start(const std::wstring& file)
{
    _L = luaL_newstate();
    lua_setallocf(_L, lmalloc, 0);
    if (_L == NULL) return false;

    luaL_openlibs(_L);

    SetupOpenGLBinding(_L);
    SetupProgramBinding(_L);
    SetupTextureBinding(_L);
    SetupTextRenderBinding(_L);
    SetupVertexArrayBinding(_L);
    SetupVertexBufferBinding(_L);
    SetupRenderTextureBinding(_L);
    GameWindow::SetupWindowBinding(_L);
    TextRender::createProgram();

    lua_register(_L, "print", lPrint);

    string name = Encoding::unicodeToUTF8(file);
    if (luaL_dofile(_L, name.c_str()) == LUA_OK) {
        lua_getfield(_L, 1, "update");
        lua_setfield(_L, LUA_REGISTRYINDEX, "game_update");

        lua_getfield(_L, 1, "render");
        lua_setfield(_L, LUA_REGISTRYINDEX, "game_render");

        lua_getfield(_L, 1, "handle_mouse_event");
        lua_setfield(_L, LUA_REGISTRYINDEX, "handle_mouse_event");

        lua_getfield(_L, 1, "handle_keyboard_event");
        lua_setfield(_L, LUA_REGISTRYINDEX, "handle_keyboard_event");
        lua_settop(_L, 0);
        return 1;
    } else {
        OutputDebugStringA(lua_tostring(_L, -1));
        exit(-1);
    }
    close();
    return 0;
}

void LuaVM::close()
{
    if (_L != NULL) {
        lua_close(_L);
        _L = NULL;

        TextRender::deleteProgram();
    }
}

void LuaVM::update(float elapsed)
{
    if (_L != NULL) {
        lua_getfield(_L, LUA_REGISTRYINDEX, "game_update");
        lua_pushnumber(_L, elapsed);
        if (lua_pcall(_L, 1, 0, 0) != LUA_OK) {
            OutputDebugStringA("update error: ");
            OutputDebugStringA(lua_tostring(_L, -1));
            OutputDebugStringA("\n");
        }
        lua_settop(_L, 0);
        lua_gc(_L, LUA_GCCOLLECT, 0);
    }
}

void LuaVM::render()
{
    if (_L != NULL) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_MULTISAMPLE);
        lua_getfield(_L, LUA_REGISTRYINDEX, "game_render");
        if (lua_pcall(_L, 0, 0, 0) != LUA_OK) {
            OutputDebugStringA("render error: ");
            OutputDebugStringA(lua_tostring(_L, -1));
            OutputDebugStringA("\n");
        }
        lua_settop(_L, 0);
        GLCache::clear();

        glFinish();

        GLDevice::instance().present();
    }
}

void LuaVM::fireMouseEvent(int x, int y, int how, bool pressed)
{
    if (_L != NULL) {
        lua_getfield(_L, LUA_REGISTRYINDEX, "handle_mouse_event");
        lua_pushnumber(_L, x);
        lua_pushnumber(_L, y);
        lua_pushnumber(_L, how);
        lua_pushboolean(_L, pressed);
        if (lua_pcall(_L, 4, 0, 0) != LUA_OK) {
            OutputDebugStringA("mosueEvent error: ");
            OutputDebugStringA(lua_tostring(_L, -1));
            OutputDebugStringA("\n");
        }
        lua_settop(_L, 0);
    }
}

void LuaVM::fireKeyboardEvent(int what, int how)
{
    if (_L != NULL) {
        lua_getfield(_L, LUA_REGISTRYINDEX, "handle_keyboard_event");
        lua_pushnumber(_L, what);
        lua_pushnumber(_L, how);
        if (lua_pcall(_L, 2, 0, 0) != LUA_OK) {
            OutputDebugStringA("keyboardEvent error: ");
            OutputDebugStringA(lua_tostring(_L, -1));
            OutputDebugStringA("\n");
        }
        lua_settop(_L, 0);
    }
}