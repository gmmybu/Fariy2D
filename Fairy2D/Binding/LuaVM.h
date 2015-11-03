#pragma once
#include <string>
#include "lua.hpp"

class LuaVM
{
public:
    LuaVM();
    ~LuaVM();

    static LuaVM& instance()
    {
        static LuaVM lvm;
        return lvm;
    }

    int     start(const std::wstring& file);
    void    close();

    void    update(float elapsed);
    void    render();

    void    fireMouseEvent(int x, int y, int how, bool pressed);
    void    fireKeyboardEvent(int what, int how);
private:
    lua_State*    _L;
};