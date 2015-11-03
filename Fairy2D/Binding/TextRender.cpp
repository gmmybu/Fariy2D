#include "GLCache.h"
#include "TextRender.h"

GLBuffer*       TextRender::__buffer;
GLProgram*      TextRender::__program;
GLVertexArray*  TextRender::__vertexArray;

const char textrender_vertex[] =
    "#version 330                                 \n"
    "layout(location=0) in vec4  a_position;      \n"
    "layout(location=1) in vec2  a_textureCoord;  \n"
    "uniform mat4 MVP;                            \n"
    "out vec2 v_texCoord;                         \n"
    "void main()                                  \n"
    "{                                            \n"
    "   gl_Position = MVP * a_position;           \n"
    "   v_texCoord  = a_textureCoord;             \n"
    "}                                            \n";

const char textrender_frag[] =
    "uniform sampler2D texture;                         \n"
    "uniform vec4 color;                                \n"
    "in vec2 v_texCoord;                                \n"
    "void main()                                        \n"
    "{                                                  \n"
    "   float red = texture2D(texture, v_texCoord).r;   \n"
    "   gl_FragColor = vec4(1, 1, 1, red) * color;      \n"
    "}                                                  \n";

void TextRender::createProgram()
{
    __program = new GLProgram;
    __program->create(textrender_vertex, textrender_frag);

    __vertexArray = new GLVertexArray;

    __buffer = new GLBuffer;
    __buffer->create(8 * 2 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

    GLCache::bindVertexArray(*__vertexArray);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, *__buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, TRUE, 0, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, TRUE, 0, (const GLvoid*)32);

}

void TextRender::deleteProgram()
{
    delete __buffer;
    delete __vertexArray;
    delete __program;
}

TextRender::TextRender(const wstring& face, int height, BOOL bold)
{
    _textures = NULL;

    _memdc = CreateCompatibleDC(NULL);
    SetBkMode(_memdc, TRANSPARENT);
    SetTextColor(_memdc, RGB(255,255,255));

    LOGFONT font;
    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(font), &font);
    font.lfHeight = height;
    wcscpy_s(font.lfFaceName, face.c_str());
    _font = CreateFontIndirect(&font);
    _font = (HFONT)SelectObject(_memdc, _font);

    TEXTMETRIC metric;
    GetTextMetrics(_memdc, &metric);

    SIZE size;
    GetTextExtentPoint32(_memdc, L"²Ø", 1, &size);
    BITMAPINFOHEADER bmphdr = {0};
    bmphdr.biSize   = sizeof(bmphdr);
    bmphdr.biWidth  = size.cx;
    bmphdr.biHeight = -size.cy;   
    bmphdr.biPlanes = 1;
    bmphdr.biBitCount = 32;
    _bitmap = CreateDIBSection(NULL, (PBITMAPINFO)&bmphdr, DIB_RGB_COLORS, (void**)&_data, NULL, 0);
    _bitmap = (HBITMAP)SelectObject(_memdc, _bitmap);
    _width  = size.cx;
    _height = size.cy;
}

TextRender::~TextRender()
{
    reset();

    _font = (HFONT)SelectObject(_memdc, _font);
    DeleteObject(_font);
    
    _bitmap = (HBITMAP)SelectObject(_memdc, _bitmap);
    DeleteObject(_bitmap);
    
    DeleteDC(_memdc);
}

int TextRender::charWidth(wchar_t ch)
{
    auto itr = _characters.find(ch);
    if (itr == _characters.end()) {
        addGlyph(ch);
        itr = _characters.find(ch);
    }
    return itr->second._width;
}

int TextRender::charHeight()
{
    return _height;
}

TextRender::TextureItem* TextRender::createTexture()
{
    auto texture = new TextureItem;
    texture->_freeleft = 0;
    texture->_freetop  = 0;
    texture->_texture.create(1024, 1024);
    texture->_next = NULL;
    return texture;
}

TextRender::TextureItem* TextRender::ensureTexture(int width)
{
    if (_textures == NULL) {
        _textures = createTexture();
    }

    if (_textures->_freeleft + width < 1024)
        return _textures;

    if (_textures->_freetop + charHeight() < 1024) {
        _textures->_freetop += charHeight();
        _textures->_freeleft = 0;
        return _textures;
    }

    auto texture = createTexture();
    texture->_next = _textures;
    _textures = texture;
    return texture;
}

void TextRender::addGlyph(wchar_t ch)
{
    int width;
    int height = this->charHeight();
    GetCharWidth32(_memdc, ch, ch, &width);
    memset(_data, 0, _width * height * 4);

    RECT rect ={0, 0, width, height};
    DrawText(_memdc, &ch, 1, &rect, DT_VCENTER|DT_CENTER);

    vector<BYTE> pixels;
    pixels.resize(width * height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            pixels[i * width + j] = GetRValue(_data[i * _width + j]);
        }
    }

    auto texture = ensureTexture(width);
    texture->_texture.update(texture->_freeleft, texture->_freetop, width, height, pixels.data());

    CharItem item;
    item._width = width;

    int left = texture->_freeleft;
    int top  = texture->_freetop;
    item._textureCoord[0][0] = left;           
    item._textureCoord[0][1] = top + height;
    item._textureCoord[1][0] = left;
    item._textureCoord[1][1] = top;
    item._textureCoord[2][0] = left + width;
    item._textureCoord[2][1] = top  + height;
    item._textureCoord[3][0] = left + width;
    item._textureCoord[3][1] = top;
    for (int i = 0; i < 4; i++) {
        item._textureCoord[i][0] /= 1024.f;
        item._textureCoord[i][1] /= 1024.f;
    }

    item._texture = texture;
    _characters[ch] = item;
    texture->_freeleft += width;
}

int TextRender::measure(const std::wstring& text)
{
    int width = 0;
    for (size_t i = 0; i < text.size(); i++) {
        width += charWidth(text[i]);
    }
    return width;
}

//ÔÝÊ±²»ÐÞ¸Ä
int TextRender::render(const std::wstring& text, float left, float bottom, const Color& color, const float* matrix)
{
    int width = 0;
    for (size_t i = 0; i < text.size(); i++) {
        width += render(text[i], left + width, bottom, color, matrix);
    }
    return width;
}

int TextRender::render(wchar_t ch, float left, float top, const Color& color, const float* matrix)
{
    int height = this->charHeight();
    int width  = this->charWidth(ch);

    GLfloat positions[4][2];
    positions[0][0] = left;
    positions[0][1] = top;
    positions[1][0] = left;
    positions[1][1] = top  + height;
    positions[2][0] = left + width;
    positions[2][1] = top;
    positions[3][0] = left + width;
    positions[3][1] = top  + height;

    size_t size = 4 * 2 * sizeof(GLfloat);
    __buffer->update(0, size, positions);
    CharItem& item = _characters[ch];
    __buffer->update(size, size, item._textureCoord);
    GLCache::useProgram(*__program);
    GLCache::bindVertexArray(*__vertexArray);
    __program->setUniform4fv("color", (GLfloat*)&color.r, 1);
    __program->setUniformTexture("texture", item._texture->_texture, GL_TEXTURE_2D);
    __program->setUniformMatrix4fv("MVP", matrix);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //GLASSERT(glGetError() == GL_NO_ERROR);
    return width;
}

void TextRender::reset()
{
    _characters.clear();

    auto current = _textures;
    while (current != NULL) {
        auto next = current->_next;
        delete current;
        current = next;
    }
    _textures = NULL;
}