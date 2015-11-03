#pragma once
#include <windows.h>
#include "glew.h"
#include "wglew.h"

#include <vector>
#include <string>
#include <map>
#include <memory>

using namespace std;

#define GL_NO_COPY(cls)  \
private:                 \
    cls(const cls&);     \
    cls& operator=(const cls&)

#define GLUNI(x)  L##x
#define WIDEN(x)  GLUNI(x)
#define GLTRACER  WIDEN(__FUNCTION__)

#define GLASSERT(cond)                                      \
    do                                                      \
    {                                                       \
        if (!(cond)) {                                      \
            OutputDebugString(GLTRACER);                    \
            OutputDebugString(L"\nAssert Fail.\n");  \
            exit(-1);                                       \
        }                                                   \
    } while(0)

#define NS_GL_BEGIN namespace Fairy2D {
#define NS_GL_CLOSE }