#pragma once
#include "GLCommon.h"

NS_GL_BEGIN

class GLCache
{
public:
    static void bindVertexArray(GLuint vertex);

    static void useProgram(GLuint program);

    static void clear();
};


NS_GL_CLOSE
