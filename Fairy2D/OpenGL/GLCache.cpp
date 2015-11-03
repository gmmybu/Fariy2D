#include "GLCache.h"

NS_GL_BEGIN

GLuint _program;
GLuint _vertexArray;

void GLCache::bindVertexArray(GLuint vertexArray)
{
    if (vertexArray != _vertexArray) {
        _vertexArray = vertexArray;
        glBindVertexArray(vertexArray);
    }
}

void GLCache::useProgram(GLuint program)
{
    if (program != _program) {
        _program = program;
        glUseProgram(program);
    }
}

void GLCache::clear()
{
    _program = _vertexArray = 0;
}

NS_GL_CLOSE