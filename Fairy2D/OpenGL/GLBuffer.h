#pragma once
#include "GLCommon.h"

NS_GL_BEGIN

class GLBuffer
{
public:
    GLBuffer();
    ~GLBuffer();

    operator GLuint();

    void    create(size_t size, const GLvoid* data, GLenum usage = GL_STREAM_DRAW);
    void    update(size_t from, size_t size, const GLvoid* data);
private:
    GLuint  _buffer;

    GL_NO_COPY(GLBuffer);
};

NS_GL_CLOSE