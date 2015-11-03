#include "LuaVM.h"
#include "GLDevice.h"
#include "GameWindow.h"

using namespace Fairy2D;

BEGIN_MESSAGE_MAP(GameWindow, Window)
    ON_MESSAGE(WM_CREATE,  onCreate)
    ON_MESSAGE(WM_KEYDOWN, onKeyDown)
    ON_MESSAGE(WM_KEYUP,   onKeyUp)
    ON_MESSAGE(WM_CHAR,    onChar)

    ON_MESSAGE(WM_MOUSEMOVE,   onMouseMove)
    ON_MESSAGE(WM_LBUTTONDOWN, onMouseDown)
    ON_MESSAGE(WM_LBUTTONUP,   onMouseUp)

    ON_MESSAGE(WM_RBUTTONDOWN, onRMouseDown)
    ON_MESSAGE(WM_RBUTTONUP,   onRMouseUp)

    ON_MESSAGE(WM_DESTROY,     onDestroy)

END_MESSAGE_MAP()

LRESULT GameWindow::onCreate(WPARAM wParam, LPARAM lParam)
{
    GLDevice::instance().initialize(_window);
    return 0;
}

LRESULT GameWindow::onKeyDown(WPARAM wParam, LPARAM lParam)
{
    LuaVM::instance().fireKeyboardEvent(wParam, WM_KEYDOWN);
    return 0;
}

LRESULT GameWindow::onKeyUp(WPARAM wParam, LPARAM lParam)
{
    LuaVM::instance().fireKeyboardEvent(wParam, WM_KEYUP);

    if (wParam == VK_ESCAPE) {
        close();
    }
    return 0;
}

LRESULT GameWindow::onChar(WPARAM wParam, LPARAM lParam)
{
    LuaVM::instance().fireKeyboardEvent(wParam, WM_CHAR);
    return 0;
}

LRESULT GameWindow::onMouseMove(WPARAM wParam, LPARAM lParam)
{
    POINT point = getCursorPos();
    LuaVM::instance().fireMouseEvent(point.x, point.y, WM_MOUSEMOVE, GetCapture() == _window);
    return 0;
}

LRESULT GameWindow::onMouseDown(WPARAM wParam, LPARAM lParam)
{
    POINT point = getCursorPos();
    SetCapture(_window);
    LuaVM::instance().fireMouseEvent(point.x, point.y, WM_LBUTTONDOWN, TRUE);
    return 0;
}

LRESULT GameWindow::onMouseUp(WPARAM wParam, LPARAM lParam)
{
    POINT point = getCursorPos();
    ReleaseCapture();

    LuaVM::instance().fireMouseEvent(point.x, point.y, WM_LBUTTONUP, FALSE);
    return 0;
}

LRESULT GameWindow::onRMouseDown(WPARAM wParam, LPARAM lParam)
{
    POINT point = getCursorPos();
    SetCapture(_window);
    LuaVM::instance().fireMouseEvent(point.x, point.y, WM_RBUTTONDOWN, TRUE);
    return 0;
}

LRESULT GameWindow::onRMouseUp(WPARAM wParam, LPARAM lParam)
{
    POINT point = getCursorPos();
    ReleaseCapture();

    LuaVM::instance().fireMouseEvent(point.x, point.y, WM_RBUTTONUP, FALSE);
    return 0;
}

LRESULT GameWindow::onDestroy(WPARAM wParam, LPARAM lParam)
{
    LuaVM::instance().close();
    GLDevice::instance().release();

    PostQuitMessage(0);
    return 0;
}

static GameWindow* window;

GameWindow::GameWindow()
{
    window = this;
}

int lSetViewport(lua_State* state)
{
    int width  = lua_tointeger(state, 1);
    int height = lua_tointeger(state, 2);
    RECT rect = {0, 0, width, height};
    AdjustWindowRect(&rect, window->getWindowLong(GWL_STYLE), FALSE);
    window->setWindowPos(NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER|SWP_NOMOVE);
    window->centerWindow();
    glViewport(0, 0, width, height);
    return 0;
}

void GameWindow::SetupWindowBinding(lua_State* state)
{
    lua_register(state, "glViewport", lSetViewport);
}