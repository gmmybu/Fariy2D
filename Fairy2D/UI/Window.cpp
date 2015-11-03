#include "Window.h"
#include <map>

static Window* s_window;

static LRESULT WINAPI windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_NCCREATE) {
        s_window->_window = hWnd;
    }
    return s_window->windowProc(uMsg, wParam, lParam);
}

static void prepareClass()
{
    static bool isReady = false;

    if (!isReady) {
        WNDCLASSEX wc    = {0};
        wc.cbSize        = sizeof(WNDCLASSEX);
        wc.style         = CS_DBLCLKS|CS_OWNDC;
        wc.lpfnWndProc   = windowProc;
        wc.hInstance     = NULL;
        wc.hIcon         = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
        wc.hCursor       = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
        wc.lpszClassName = WINDOW_CLASS;

        if (!RegisterClassEx(&wc)) {
            OutputDebugString(L"prepareClass Failed\n");
            exit(-1);
        }
        isReady = true;
    }
}

Window::Window() : _window(NULL)
{
    s_window = this;
}

Window::~Window()
{
    s_window = nullptr;
}

bool Window::create(
    int       left,
    int       top,
    int       width,
    int       height,
    DWORD     style,
    LPCTSTR   title,
    HWND      parent,
    HMENU     menu,
    HINSTANCE instance)
{
    prepareClass();

    return CreateWindow(
        WINDOW_CLASS,
        title,
        style,
        left,
        top,
        width,
        height,
        parent,
        menu,
        instance,
        NULL) != NULL;
}

bool Window::createEx(
    int       left,
    int       top,
    int       width,
    int       height,
    DWORD     style,
    DWORD     styleEx,
    LPCTSTR   title,
    HWND      parent,
    HMENU     menu,
    HINSTANCE instance)
{
    prepareClass();

    return ::CreateWindowEx(
        styleEx,
        WINDOW_CLASS,
        title,
        style,
        left,
        top,
        width,
        height,
        parent,
        menu,
        instance,
        NULL) != NULL;
}

LRESULT Window::windowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY) {
        delete this;
        return 0;
    }
    return DefWindowProc(_window, uMsg, wParam, lParam);
}

bool Window::trackMouseEvent(bool hover)
{
    TRACKMOUSEEVENT track;
    track.cbSize = sizeof(TRACKMOUSEEVENT);
    track.dwFlags = TME_LEAVE;
    if (hover) {
        track.dwFlags |= TME_HOVER;
    }
    track.hwndTrack = _window;
    track.dwHoverTime = HOVER_DEFAULT;
    return TrackMouseEvent(&track);
}

POINT Window::getCursorPos()
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(_window, &point);
    return point;
}

void Window::centerWindow()
{
    RECT rect = getWindowRect();
    int cx = rect.right - rect.left;
    int cy = rect.bottom - rect.top;
    int ys = GetSystemMetrics(SM_CYSCREEN);
    int xs = GetSystemMetrics(SM_CXSCREEN);
    setWindowPos(0, (xs - cx) / 2, (ys - cy - 100) / 2,
        0, 0, SWP_NOZORDER|SWP_NOSIZE);
}
