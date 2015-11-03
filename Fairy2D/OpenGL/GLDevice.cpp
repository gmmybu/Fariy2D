#include "GLDevice.h"
#pragma comment(lib, "opengl32.lib")

NS_GL_BEGIN

static BOOL VerifyMultiSample(PIXELFORMATDESCRIPTOR& pfd, int& pixelFormat)
{
    HWND hWnd = CreateWindow(L"BUTTON", NULL, WS_POPUP,
        0, 0, 0, 0, GetDesktopWindow(), NULL, NULL, NULL);

    GLASSERT(hWnd != NULL);

    HDC hDC = GetDC(hWnd);
    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixelFormat, &pfd);
    HGLRC hGL = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGL);

    BOOL success = FALSE;
    typedef BOOL (WINAPI* pfnChooseProc)(HDC, const int*, const float*, UINT, int*, UINT*);
    pfnChooseProc wglChoose = (pfnChooseProc)wglGetProcAddress("wglChoosePixelFormatARB");
    if (wglChoose == NULL) goto Cleanup;

    UINT    nFormats;
    float   fAttr[] = {0, 0};
    int     iAttr[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,     32,
        WGL_ALPHA_BITS_ARB,     8,
        WGL_DEPTH_BITS_ARB,     24,
        WGL_STENCIL_BITS_ARB,   8,
        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        WGL_SAMPLES_ARB,        4,
        0,                      0
    };

    if (wglChoose(hDC, iAttr, fAttr, 1, &pixelFormat, &nFormats) && nFormats >= 1) {
        success = TRUE;
        goto Cleanup;
    }

    iAttr[19] = 2;
    success = wglChoose(hDC, iAttr, fAttr, 1, &pixelFormat, &nFormats) && nFormats >= 1;

Cleanup:
    wglMakeCurrent(hDC, 0);
    wglDeleteContext(hGL);
    ReleaseDC(hWnd, hDC);
    DestroyWindow(hWnd);
    return success;
}

static void SetupPixelFormat(HDC hDC)
{
    int pixelFormat;
    PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
    {
        sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
        1,																// Version Number
        PFD_DRAW_TO_WINDOW |											// Format Must Support Window
        PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
        PFD_DOUBLEBUFFER,												// Must Support Double Buffering
        PFD_TYPE_RGBA,													// Request An RGBA Format
        32,										                        // Select Our Color Depth
        0, 0, 0, 0, 0, 0,												// Color Bits Ignored
        1,																// Alpha Buffer
        0,																// Shift Bit Ignored
        0,																// No Accumulation Buffer
        0, 0, 0, 0,														// Accumulation Bits Ignored
        16,																// 16Bit Z-Buffer (Depth Buffer)  
        0,																// No Stencil Buffer
        0,																// No Auxiliary Buffer
        PFD_MAIN_PLANE,													// Main Drawing Layer
        0,																// Reserved
        0, 0, 0															// Layer Masks Ignored
    };
    if (!VerifyMultiSample(pfd, pixelFormat)) {
        pixelFormat = ChoosePixelFormat(hDC, &pfd);
    }
    SetPixelFormat(hDC, pixelFormat, &pfd);
}

BOOL GLDevice::initialize(HWND hWnd)
{
    GLASSERT(_hDC == NULL);
    _hDC = GetDC(hWnd);
    SetupPixelFormat(_hDC);

    _hGL = wglCreateContext(_hDC);
    wglMakeCurrent(_hDC, _hGL);
    GLenum flag = glewInit();
    GLASSERT(flag == GLEW_OK);
    _hWnd = hWnd;

    return TRUE;
}

void GLDevice::release()
{
    wglMakeCurrent(_hDC, 0);
    wglDeleteContext(_hGL);
    ReleaseDC(_hWnd, _hDC);
}

void GLDevice::present()
{
    SwapBuffers(_hDC);
}

NS_GL_CLOSE