#include "GLVertexArray.h"

NS_GL_BEGIN

GLVertexArray::GLVertexArray()
{
    glGenVertexArrays(1, &_vertexArray);
}

GLVertexArray::~GLVertexArray()
{
    glDeleteVertexArrays(1, &_vertexArray);
}

GLVertexArray::operator GLuint()
{
    return _vertexArray;
}

NS_GL_CLOSE