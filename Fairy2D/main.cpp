#include "LuaVM.h"
#include "System.h"
#include "GameWindow.h"

using namespace System;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nShowCmd)
{
    GameWindow* window = new GameWindow;
    window->create(0, 0, 600, 480, WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX);

    wchar_t file[MAX_PATH];
    GetModuleFileName(NULL, file, MAX_PATH);
    LuaVM::instance().start(File::getPath(file) + L"\\main.lua");

    window->update();
    window->show(SW_NORMAL);
    window->centerWindow();

    LARGE_INTEGER interval;
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    interval.QuadPart = freq.QuadPart / 120;

    LARGE_INTEGER nLast;
    QueryPerformanceCounter(&nLast);

    MSG msg;
    LARGE_INTEGER nNow;
    while (true)
	{
	    if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	    {
            QueryPerformanceCounter(&nNow);

            LONGLONG delta = nNow.QuadPart - nLast.QuadPart;
            if (delta > interval.QuadPart)
            {
                nLast.QuadPart = nNow.QuadPart;
                LuaVM::instance().update(delta * 1.f / freq.QuadPart);
                LuaVM::instance().render();
            }
	        continue;
        }

	    if (WM_QUIT == msg.message)
	        break;

	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}
    return 0;
}