#pragma once
#include "GLCommon.h"

NS_GL_BEGIN

class GLFontTexture
{
public:
    GLFontTexture();
    ~GLFontTexture();

    operator GLuint();

    void     create(size_t width, size_t height);
    void     update(GLint left, GLint top, size_t width, size_t height, const GLvoid* pixels);
    size_t   getWidth() const;
    size_t   getHeight() const;
private:
    GLuint    _texture;

    size_t    _width;
    size_t    _height;

    GL_NO_COPY(GLFontTexture);
};




NS_GL_CLOSE

