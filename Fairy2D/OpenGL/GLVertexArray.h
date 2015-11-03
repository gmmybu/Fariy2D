#pragma once
#include "GLCommon.h"

NS_GL_BEGIN

class GLVertexArray
{
public:
    GLVertexArray();
    ~GLVertexArray();

    operator GLuint();
private:
    GLuint _vertexArray;

    GL_NO_COPY(GLVertexArray);
};

NS_GL_CLOSE
