#include "GLRenderTarget.h"

NS_GL_BEGIN

GLRenderTarget::GLRenderTarget()
{
}

BOOL GLRenderTarget::create(size_t width, size_t height)
{
    _texture.create(width, height, NULL);

    glGenFramebuffers(1, &_frameBufferMSP);
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferMSP);

    //glGenRenderbuffers(1, &_depthBufferMSP);
    //glBindRenderbuffer(GL_RENDERBUFFER, _depthBufferMSP);
    //glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH_COMPONENT, width, height);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT,
    //    GL_RENDERBUFFER, _depthBufferMSP);

//     glGenRenderbuffers(1, &_textureMSP);
//     glBindRenderbuffer(GL_RENDERBUFFER, _textureMSP);
//     glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGBA8, width, height);
//     glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
//         GL_RENDERBUFFER, _textureMSP);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

//    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return TRUE;
}

GLRenderTarget::~GLRenderTarget()
{
    if (_frameBufferMSP != 0) {
        glDeleteRenderbuffers(1, &_frameBufferMSP);
    }

    if (_textureMSP != 0) {
  //      glDeleteRenderbuffers(1, &_textureMSP);
    }

    if (_depthBufferMSP != 0) {
 //       glDeleteRenderbuffers(1, &_depthBufferMSP);
    }
}

void GLRenderTarget::beginRender()
{
    GLASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && _texture != 0);

    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferMSP);
    //glEnable(GL_MULTISAMPLE);
}

void GLRenderTarget::endRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLTexture& GLRenderTarget::getTexture()
{
    return _texture;
}

NS_GL_CLOSE