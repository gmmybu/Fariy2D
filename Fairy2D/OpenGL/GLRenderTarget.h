#pragma once
#include "GLTexture.h"

NS_GL_BEGIN

class GLRenderTarget
{
public:
    GLRenderTarget();
    ~GLRenderTarget();

    GLTexture&  getTexture();
    BOOL    create(size_t width, size_t height);
    void    beginRender();
    void    endRender();
    GLenum  getStatus();
private:
    GLTexture   _texture;

    GLuint      _textureMSP;
    GLuint      _depthBufferMSP;
    GLuint      _frameBufferMSP;

    GL_NO_COPY(GLRenderTarget);
};

NS_GL_CLOSE