#pragma once
#include "GLBuffer.h"
#include "GLProgram.h"
#include "GLVertexArray.h"
#include "GLFontTexture.h"

#include "Color.h"

using namespace Fairy2D;

class TextRender
{
public:
    TextRender(const wstring& face, int size, BOOL bold = FALSE);
    ~TextRender();

    static void createProgram();
    static void deleteProgram();

    int     charWidth(wchar_t ch);
    int     charHeight();

    int     measure(const std::wstring& text);
    int     render(const std::wstring& text, float left, float bottom, const Color& color, const float* matrix);

    void    reset();
private:
    void    addGlyph(wchar_t ch);
    int     render(wchar_t ch, float left, float top, const Color& color, const float* matrix);

    struct TextureItem
    {
        GLFontTexture   _texture;
        int             _freeleft;
        int             _freetop;
        TextureItem*    _next;
    };
    TextureItem*    _textures;
    TextureItem*    createTexture();
    TextureItem*    ensureTexture(int width);

    struct CharItem
    {
        int             _width;
        float           _textureCoord[4][2];
        TextureItem*    _texture;
    };
    map<wchar_t, CharItem>    _characters;

    HDC         _memdc;
    HFONT       _font;
    HBITMAP     _bitmap;

    DWORD*      _data;
    int         _width;
    int         _height;


    static GLBuffer*        __buffer;
    static GLProgram*       __program;
    static GLVertexArray*   __vertexArray;
};