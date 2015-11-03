#include "GLProgram.h"

NS_GL_BEGIN

GLProgram::GLProgram()
{
    _program = 0;
}

GLProgram::~GLProgram()
{
    if (_program != 0) {
        glDeleteProgram(_program);
        _program = 0;

        glDeleteShader(_vertexShader);
        glDeleteShader(_fragShader);
    }
}

GLProgram::operator GLuint()
{
    return (GLuint)_program;
}

GLint GLProgram::getUniformLocation(const GLchar* location)
{
    GLASSERT(_program != 0);
    auto itr = _uniforms.find(location);
    if (itr != _uniforms.end())
        return itr->second;

    GLint where = glGetUniformLocation(_program, location);
    GLASSERT(where != -1);

    _uniforms.insert(make_pair(location, where));
    return where;
}

GLint GLProgram::getAttribLocation(const GLchar* location)
{
    GLASSERT(_program != 0);
    auto itr = _attributes.find(location);
    if (itr != _attributes.end())
        return itr->second;

    GLint where = glGetAttribLocation(_program, location);
    GLASSERT(where != -1);

    _attributes.insert(make_pair(location, where));
    return where;
}

GLuint GLProgram::createShader(const GLchar* source, GLuint type)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        GLsizei length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        GLchar* loginfo = (GLchar*)malloc(sizeof(GLchar) * length);
        glGetShaderInfoLog(shader, length, &length, loginfo);
        OutputDebugStringA(loginfo);
        free(loginfo);
        glDeleteShader(shader);
        GLASSERT(0);
    }
    return shader;
}

BOOL GLProgram::createFromFile(const GLchar* vertexFile, const GLchar* fragFile)
{
    GLASSERT(0);
    return FALSE;
}

BOOL GLProgram::create(const GLchar* vertexSource, const GLchar* fragSource)
{
    GLASSERT(_program == 0);
    _vertexShader = createShader(vertexSource, GL_VERTEX_SHADER);
    _fragShader   = createShader(fragSource,   GL_FRAGMENT_SHADER);

    _program = glCreateProgram();
    glAttachShader(_program, _vertexShader);
    glAttachShader(_program, _fragShader);
    glLinkProgram(_program);

    GLint status;
    glGetProgramiv(_program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        GLsizei length;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &length);
        GLchar* loginfo = (GLchar*)malloc(sizeof(GLchar) * length);
        glGetProgramInfoLog(_program, length, &length, loginfo);
        OutputDebugStringA(loginfo);
        free(loginfo);
        glDeleteProgram(_program);
        _program = 0;

        glDeleteShader(_vertexShader);
        glDeleteShader(_fragShader);
        GLASSERT(0);
    }
    return TRUE;
}

BOOL GLProgram::isValid()
{
    return _program != 0;
}

void GLProgram::useProgram()
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    if (current == _program)
        return;

    glUseProgram(_program);
}

void GLProgram::endProgram()
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);

    glUseProgram(0);
}

void GLProgram::setVertexAttribPointer(const GLchar* location, GLint size, GLenum type,
                                       GLboolean normalized, GLsizei stride, const GLvoid* data)
{
    GLint index = getAttribLocation(location);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, data);
}

void GLProgram::setUniform1i(const GLchar* location, GLint i1)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform1i(index, i1);
}

void GLProgram::setUniform2i(const GLchar* location, GLint i1, GLint i2)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform2i(index, i1, i2);
}

void GLProgram::setUniform3i(const GLchar* location, GLint i1, GLint i2, GLint i3)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform3i(index, i1, i2, i3);
}

void GLProgram::setUniform4i(const GLchar* location, GLint i1, GLint i2, GLint i3, GLint i4)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform3i(index, i1, i2, i3);
}

void GLProgram::setUniform1iv(const GLchar* location, const GLint* ints, size_t numberofArrays)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform1iv(index, numberofArrays, ints);
}

void GLProgram::setUniform2iv(const GLchar* location, const GLint* ints, size_t numberofArrays)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform2iv(index, numberofArrays, ints);
}

void GLProgram::setUniform3iv(const GLchar* location, const GLint* ints, size_t numberofArrays)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform3iv(index, numberofArrays, ints);
}

void GLProgram::setUniform4iv(const GLchar* location, const GLint* ints, size_t numberofArrays)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform3iv(index, numberofArrays, ints);
}

void GLProgram::setUniform1f(const GLchar* location, GLfloat f1)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform1f(index, f1);
}

void GLProgram::setUniform2f(const GLchar* location, GLfloat f1, GLfloat f2)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform2f(index, f1, f2);
}

void GLProgram::setUniform3f(const GLchar* location, GLfloat f1, GLfloat f2, GLfloat f3)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform3f(index, f1, f2, f3);
}

void GLProgram::setUniform4f(const GLchar* location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform4f(index, f1, f2, f3, f4);
}

void GLProgram::setUniform1fv(const GLchar* location, const GLfloat* floats, size_t numberOfArrays)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform1fv(index, numberOfArrays, floats);
}

void GLProgram::setUniform2fv(const GLchar* location, const GLfloat* floats, size_t numberOfArrays)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform2fv(index, numberOfArrays, floats);
}

void GLProgram::setUniform3fv(const GLchar* location, const GLfloat* floats, size_t numberOfArrays)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform3fv(index, numberOfArrays, floats);
}

void GLProgram::setUniform4fv(const GLchar* location, const GLfloat* floats, size_t numberOfArrays)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniform4fv(index, numberOfArrays, floats);
}

void GLProgram::setUniformMatrix4fv(const GLchar* location, const GLfloat* matrixArray)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniformMatrix4fv(index, 1, GL_TRUE, matrixArray);
}

void GLProgram::setUniformMatrix3fv(const GLchar* location, const GLfloat* matrixArray)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLint index = getUniformLocation(location);
    glUniformMatrix3fv(index, 1, GL_FALSE, matrixArray);
}

void GLProgram::setUniformTexture(const GLchar* location, GLint texture, GLenum target, int level)
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    GLASSERT(current == _program);
    GLASSERT(glIsTexture(texture));
    GLint index = getUniformLocation(location);
    glActiveTexture(GL_TEXTURE0 + level);
    glBindTexture(target, texture);
    glUniform1i(index, level);
}

NS_GL_CLOSE