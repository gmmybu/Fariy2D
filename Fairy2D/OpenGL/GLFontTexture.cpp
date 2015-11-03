#pragma once
#include "GLFontTexture.h"

NS_GL_BEGIN

 GLFontTexture::GLFontTexture()
{
    _texture = 0;
    _height  = 0;
    _width   = 0;
}

GLFontTexture::~GLFontTexture()
{
    if (_texture != 0) {
        glDeleteTextures(1, &_texture);
    }
}

void GLFontTexture::create(size_t width, size_t height)
{
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    _width  = width;
    _height = height;
}

void GLFontTexture::update(GLint left, GLint top, size_t width, size_t height, const GLvoid* pixels)
{
	glBindTexture(GL_TEXTURE_2D, _texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, left, top, width, height, GL_RED, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLFontTexture::operator GLuint()
{
    return _texture;
}

size_t GLFontTexture::getWidth() const
{
    return _width;
}

size_t GLFontTexture::getHeight() const
{
    return _height;
}

NS_GL_CLOSE