#pragma once
#include "GLCommon.h"

NS_GL_BEGIN

class GLTexture
{
public:
    GLTexture();
    ~GLTexture();

    operator GLuint();
    void     create(size_t width, size_t height, const GLvoid* pixels);
    void     update(GLint offsetX, GLint offsetY, size_t width, size_t height, const GLvoid* pixels);
    void	 update(GLint x, GLint y, GLint offsetX, GLint offsetY, size_t width, size_t height);

    void     create(size_t width, const GLvoid* pixels);
    void     update(GLint offsetX, size_t width, const GLvoid* pixels);
    size_t   getWidth() const;
    size_t   getHeight() const;
private:
    GLuint    _texture;
    GLenum    _target;

    size_t    _width;
    size_t    _height;

    GL_NO_COPY(GLTexture);
};

NS_GL_CLOSE