#pragma once
#include "GLTexture.h"
#include "stb_image.h"

NS_GL_BEGIN

GLTexture::GLTexture()
{
    _texture = 0;
    _target  = 0;
    _height  = 0;
    _width   = 0;
}

GLTexture::~GLTexture()
{
    if (_texture != 0) {
        glDeleteTextures(1, &_texture);
    }
}

void GLTexture::create(size_t width, size_t height, const GLvoid* pixels)
{
    GLASSERT(_texture == 0);

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    _target = GL_TEXTURE_2D;
    _width  = width;
    _height = height;
}

void GLTexture::create(size_t width, const GLvoid* pixels)
{
    GLASSERT(_texture == 0);

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_1D, _texture);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_1D, 0);

    _target = GL_TEXTURE_1D;
    _width  = width;
    _height = 1;
}

void GLTexture::update(GLint offsetX, GLint offsetY, size_t width, size_t height, const GLvoid* pixels)
{
    GLASSERT(_target == GL_TEXTURE_2D && _texture != 0);

    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::update(GLint x, GLint y, GLint offsetX, GLint offsetY, size_t width, size_t height)
{
    GLASSERT(_target == GL_TEXTURE_2D && _texture != 0);

    glBindTexture(GL_TEXTURE_2D, _texture);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, offsetY, x, y, width, height);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::update(GLint offsetX, size_t width, const GLvoid* pixels)
{
    GLASSERT(_target == GL_TEXTURE_1D);

    glBindTexture(GL_TEXTURE_1D, _texture);
    glTexSubImage1D(GL_TEXTURE_1D, 0, offsetX, width, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_1D, 0);
}

GLTexture::operator GLuint()
{
    return _texture;
}

size_t GLTexture::getWidth() const
{
    return _width;
}

size_t GLTexture::getHeight() const
{
    return _height;
}

NS_GL_CLOSE