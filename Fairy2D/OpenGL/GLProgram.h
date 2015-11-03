#pragma once
#include "GLCommon.h"

NS_GL_BEGIN

class GLProgram
{
public:
    GLProgram();
    ~GLProgram();

    operator GLuint();

    GLint   getUniformLocation(const GLchar* location);
    GLint   getAttribLocation(const GLchar* location);

    BOOL    createFromFile(const GLchar* vertexFile, const GLchar* fragFile);
    BOOL    create(const GLchar* vertexSource, const GLchar* fragSource);
    BOOL    isValid();

    void    useProgram();
    void    endProgram();
    void    setVertexAttribPointer(const GLchar* location, GLint size, GLenum type,
                GLboolean normalized, GLsizei stride, const GLvoid* data);

    void    setUniform1i(const GLchar* location, GLint i1);
    void    setUniform2i(const GLchar* location, GLint i1, GLint i2);
    void    setUniform3i(const GLchar* location, GLint i1, GLint i2, GLint i3);
    void    setUniform4i(const GLchar* location, GLint i1, GLint i2, GLint i3, GLint i4);
    
    void    setUniform1iv(const GLchar* location, const GLint* ints, size_t numberofArrays);
    void    setUniform2iv(const GLchar* location, const GLint* ints, size_t numberofArrays);
    void    setUniform3iv(const GLchar* location, const GLint* ints, size_t numberofArrays);
    void    setUniform4iv(const GLchar* location, const GLint* ints, size_t numberofArrays);

    void    setUniform1f(const GLchar* location, GLfloat f1);
    void    setUniform2f(const GLchar* location, GLfloat f1, GLfloat f2);
    void    setUniform3f(const GLchar* location, GLfloat f1, GLfloat f2, GLfloat f3);
    void    setUniform4f(const GLchar* location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4);

    void    setUniform1fv(const GLchar* location, const GLfloat* floats, size_t numberOfArrays);
    void    setUniform2fv(const GLchar* location, const GLfloat* floats, size_t numberOfArrays);
    void    setUniform3fv(const GLchar* location, const GLfloat* floats, size_t numberOfArrays);
    void    setUniform4fv(const GLchar* location, const GLfloat* floats, size_t numberOfArrays);

    void    setUniformMatrix4fv(const GLchar* location, const GLfloat* matrixArray);
    void    setUniformMatrix3fv(const GLchar* location, const GLfloat* matrixArray);

    void    setUniformTexture(const GLchar* location, GLint texture, GLenum target, int level = 0);
private:
    GLuint  createShader(const GLchar* source, GLuint type);

    GLuint             _program;
    GLuint             _vertexShader;
    GLuint             _fragShader;

    map<string, GLint>    _attributes;
    map<string, GLint>    _uniforms;

    GL_NO_COPY(GLProgram);
};

NS_GL_CLOSE
