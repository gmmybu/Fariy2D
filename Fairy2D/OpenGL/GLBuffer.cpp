#include "GLBuffer.h"

NS_GL_BEGIN

GLBuffer::GLBuffer()
{
    _buffer = 0;
}

GLBuffer::~GLBuffer()
{
    if (_buffer != 0) {
        glDeleteBuffers(1, &_buffer);
    }
}

GLBuffer::operator GLuint()
{
    return _buffer;
}

void GLBuffer::create(size_t size, const GLvoid* data, GLenum usage)
{
    if (_buffer == 0) {
        glGenBuffers(1, &_buffer);
    }
    glBindBuffer(GL_ARRAY_BUFFER, _buffer);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLBuffer::update(size_t from, size_t size, const GLvoid* data)
{
    glBindBuffer(GL_ARRAY_BUFFER, _buffer);
    glBufferSubData(GL_ARRAY_BUFFER, from, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

NS_GL_CLOSE