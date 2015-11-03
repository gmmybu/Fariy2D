#pragma once
#include "Window.h"
#include "lua.hpp"

class GameWindow : public Window
{
public:
    GameWindow();

    static void SetupWindowBinding(lua_State* state);
protected:
    DECLARE_MESSAGE_MAP()

    HANDLER(onCreate);

    HANDLER(onKeyDown);
    HANDLER(onKeyUp);
    HANDLER(onChar);

    HANDLER(onMouseMove);

    HANDLER(onMouseUp);
    HANDLER(onMouseDown);

    HANDLER(onRMouseUp);
    HANDLER(onRMouseDown);

    HANDLER(onDestroy);
};