#include "GLCommon.h"

NS_GL_BEGIN

class GLDevice
{
public:
    static GLDevice& instance()
    {
        static GLDevice device;
        return device;
    }

    BOOL    initialize(HWND hWnd);
    void    release();
    void    present();
private:
    HDC     _hDC;
    HGLRC   _hGL;
    HWND    _hWnd;
};
NS_GL_CLOSE