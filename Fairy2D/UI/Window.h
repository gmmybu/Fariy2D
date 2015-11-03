#pragma once
#include <windows.h>

#define HANDLER(func) LRESULT func(WPARAM wParam, LPARAM lParam)

#define DECLARE_MESSAGE_MAP() \
    LRESULT windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
    LRESULT theClass::windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) \
{ \
    typedef baseClass base; \
    switch(uMsg) \
    {

#define    ON_MESSAGE(msg, func) \
    case msg: return func(wParam, lParam);

#define END_MESSAGE_MAP() \
    default: \
    break; \
    } \
    return base::windowProc(uMsg, wParam, lParam); \
}

#define WINDOW_CLASS L"Fairy2DWindow"

class Window
{
public:
    Window();
    virtual ~Window();

    bool create(
        int       left     = 0,
        int       top      = 0,
        int       width    = 0,
        int       height   = 0,
        DWORD     style    = WS_POPUP,
        LPCTSTR   title    = NULL,
        HWND      parent   = NULL,
        HMENU     menu     = NULL,
        HINSTANCE instance = NULL);

    bool createEx(
        int       left     = 0,
        int       top      = 0,
        int       width    = 0,
        int       height   = 0,
        DWORD     style    = WS_POPUP,
        DWORD     styleEx  = 0,
        LPCTSTR   title    = NULL,
        HWND      parent   = NULL,
        HMENU     menu     = NULL,
        HINSTANCE instance = NULL);

    void close()
    {
        postMessage(WM_CLOSE, 0, 0);
    }

    bool enable(BOOL bEnabled)
    {
        return EnableWindow(_window, bEnabled);
    }

    bool show(int nShow)
    {
        return ShowWindow(_window, nShow);
    }

    bool destroy()
    {
        return DestroyWindow(_window);
    }

    bool isZoomed()
    {
        return IsZoomed(_window);
    }

    bool isVisible()
    {
        return IsWindowVisible(_window);
    }

    bool isIconic()
    {
        return IsIconic(_window);
    }

    bool isFocused()
    {
        return GetFocus() == _window;
    }

    bool isActive()
    {
        return GetActiveWindow() == _window;
    }

    bool isEnabled()
    {
        return IsWindowEnabled(_window);
    }

    LONG setWindowLong(int nIndex, LONG dwNewLong)
    {
        return SetWindowLong(_window, nIndex, dwNewLong);
    }

    LONG getWindowLong(int nIndex)
    {
        return GetWindowLong(_window, nIndex);
    }

    bool setWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
    {
        return SetWindowPos(_window, hWndInsertAfter, X, Y, cx, cy, uFlags);
    }

    LRESULT sendMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        return SendMessage(_window, Msg, wParam, lParam);
    }

    bool postMessage(UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        return PostMessage(_window, Msg, wParam, lParam);
    }

    RECT getWindowRect()
    {
        RECT rect;
        GetWindowRect(_window, &rect);
        return rect;
    }

    RECT getClientRect()
    {
        RECT rect;
        GetClientRect(_window, &rect);
        return rect;
    }

    bool invalidateRect(RECT* rect, BOOL bErase = FALSE)
    {
        return InvalidateRect(_window, rect, bErase);
    }

    bool update()
    {
        return UpdateWindow(_window);
    }

    bool screenToClient(POINT* point)
    {
        return ScreenToClient(_window, point);
    }

    bool clientToScreen(POINT* point)
    {
        return ClientToScreen(_window, point);
    }

    POINT getCursorPos();
    void  centerWindow();

    bool trackMouseEvent(bool bHover);

    virtual LRESULT windowProc(UINT msg, WPARAM wParam, LPARAM lParam);

    HWND _window;
};