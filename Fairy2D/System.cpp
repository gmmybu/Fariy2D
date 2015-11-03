#include "System.h"
#include <shlobj.h>

NAMESPACE_START(System)

NAMESPACE_START(Encoding)

std::wstring UTF8ToUnicode(const char* utf8)
{
    if (utf8 == NULL) return std::wstring();

    std::wstring text;
    int size = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    if (size > 0) {
        text.resize(size - 1);
        MultiByteToWideChar(CP_UTF8, 0, utf8, -1, (wchar_t*)text.c_str(), size);
    }
    return text;
}

std::wstring UTF8ToUnicode(const std::string& utf8)
{
    if (utf8.empty()) return std::wstring();

    std::wstring text;
    int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
    if (size > 0) {
        text.resize(size);
        MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), (wchar_t*)text.c_str(), size);
    }
    return text;
}

std::string unicodeToUTF8(const wchar_t* text)
{
    if (text == NULL) return std::string();

    std::string utf8;
    int size = WideCharToMultiByte(CP_UTF8, 0, text, -1, NULL, 0, NULL, NULL);
    if (size > 0) {
        utf8.resize(size - 1);
        WideCharToMultiByte(CP_UTF8, 0, text, -1, (char*)utf8.c_str(), size, NULL, NULL);
    }
    return utf8;
}

std::string unicodeToUTF8(const std::wstring& text)
{
    if (text.empty()) return std::string();

    std::string utf8;
    int size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), text.size(), NULL, 0, NULL, NULL);
    if (size > 0) {
        utf8.resize(size);
        WideCharToMultiByte(CP_UTF8, 0, text.c_str(), text.size(), (char*)utf8.c_str(), size, NULL, NULL);
    }
    return utf8;
}

NAMESPACE_CLOSE(Encoding)

NAMESPACE_START(File)

FileReader::FileReader() :
    _hFile(INVALID_HANDLE_VALUE) { }

FileReader::~FileReader()
{
    close();
}

void FileReader::close()
{
    if (_hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(_hFile);
        _hFile = INVALID_HANDLE_VALUE;
    }
}

bool FileReader::open(const std::wstring& file , DWORD createflag, DWORD sharemode)
{
    close();

    _hFile = CreateFile(file.c_str(), GENERIC_READ, sharemode,
        NULL, createflag, FILE_ATTRIBUTE_NORMAL, NULL);
    return _hFile != INVALID_HANDLE_VALUE;
}

bool FileReader::read(void* data, DWORD count)
{
    if (_hFile == INVALID_HANDLE_VALUE)
        return false;

    DWORD bytesRead;
    bool  success = ReadFile( _hFile, data, count, &bytesRead, NULL) && bytesRead == count;
    if (!success) close();
    return success;
}

bool FileReader::seek(DWORD length, DWORD method)
{
    if (_hFile == INVALID_HANDLE_VALUE)
        return false;

    LARGE_INTEGER li;
    li.LowPart  = length;
    li.HighPart = 0;
    BOOL success = SetFilePointerEx(_hFile, li, &li, method);
    if (!success) close();
    return success;
}

size_t FileReader::getFileSize()
{
    if (_hFile == INVALID_HANDLE_VALUE)
        return INVALID_FILE_SIZE;

    DWORD size = GetFileSize(_hFile, NULL);
    if (size == INVALID_FILE_SIZE) close();
    return size;
}

size_t FileReader::getPosition()
{
    if (_hFile == INVALID_HANDLE_VALUE)
        return INVALID_FILE_SIZE;

    LARGE_INTEGER li = {0};
    BOOL flag = SetFilePointerEx(_hFile, li, &li, FILE_CURRENT);
    if (!flag) {
        close();
        return INVALID_FILE_SIZE;
    }
    return li.LowPart;
}

FileWriter::FileWriter() :
    _hFile(INVALID_HANDLE_VALUE) { }

FileWriter::~FileWriter()
{
    close();
}

void FileWriter::close()
{
    if (_hFile != INVALID_HANDLE_VALUE) {
        CloseHandle(_hFile);
        _hFile = INVALID_HANDLE_VALUE;
    }
}

bool FileWriter::open(const std::wstring& file , DWORD createflag, DWORD sharemode) {
    close();

    _hFile = ::CreateFile(file.c_str(), GENERIC_WRITE, sharemode, NULL,
        createflag, FILE_ATTRIBUTE_NORMAL, NULL);
    return _hFile != INVALID_HANDLE_VALUE;
}

bool FileWriter::write(const void* data, DWORD count) {
    if (_hFile == INVALID_HANDLE_VALUE)
        return false;

    DWORD bytesWrite;
    BOOL  success = WriteFile(_hFile, data, count, &bytesWrite, NULL) && bytesWrite == count;
    if (!success) close();
    return success;
}

bool FileWriter::seek(DWORD length, DWORD method)
{
    if (_hFile == INVALID_HANDLE_VALUE)
        return false;

    LARGE_INTEGER li;
    li.LowPart  = length;
    li.HighPart = 0;
    BOOL status = SetFilePointerEx(_hFile, li, &li, method);
    if (!status) close();
    return status;
}

size_t FileWriter::getFileSize()
{
    if (_hFile == INVALID_HANDLE_VALUE)
        return INVALID_FILE_SIZE;

    DWORD size = ::GetFileSize(_hFile, NULL);
    if (size == INVALID_FILE_SIZE) close();
    return size;
}

size_t FileWriter::getPosition()
{
    if (_hFile == INVALID_HANDLE_VALUE)
        return INVALID_FILE_SIZE;

    LARGE_INTEGER li = {0};
    BOOL status = SetFilePointerEx(_hFile, li, &li, FILE_CURRENT);
    if (!status) {
        close();
        return INVALID_FILE_SIZE;
    }
    return li.LowPart;
}

std::wstring getPath(const std::wstring& file)
{
    size_t index = file.find_last_of(L'\\');
    return index != std::wstring::npos ? file.substr(0, index + 1) : file;
}

std::wstring getName(const std::wstring& file, bool clean)
{
    std::wstring path = file;

    size_t index = file.find_last_of(L'\\');
    if (index != std::wstring::npos) {
        path = path.substr(index + 1);
    }

    if (clean && (index = path.find_last_of(L'.')) != std::wstring::npos) {
        path = path.substr(0, index);
    }
    return path;
}

std::wstring getTempPath()
{
    wchar_t temp[MAX_PATH] = {0};
    ::GetTempPath(MAX_PATH, temp);
    return temp;
}

std::wstring getAppDataPath()
{
    wchar_t appdata[MAX_PATH];
    SHGetSpecialFolderPath(0, appdata, CSIDL_APPDATA, 0);
    return appdata;
}

std::wstring getProgramFile()
{
    wchar_t program[MAX_PATH];
    GetModuleFileName(NULL, program, MAX_PATH);
    return program;
}

std::wstring getCurrentPath()
{
    static std::wstring currentPath;
    if (currentPath.empty()) {
        currentPath = getPath(getProgramFile());
    }
    return currentPath;
}

bool isExists(const std::wstring& file)
{
    DWORD attr = GetFileAttributes(file.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return FALSE;

    return !(attr&FILE_ATTRIBUTE_DIRECTORY);
}

std::vector<std::wstring> search(const std::wstring& path, const std::wstring& what)
{
    std::vector<std::wstring> files;

    std::wstring temp = path;
    if (temp[temp.size() - 1] != L'\\') {
        temp.append(1, '\\');
    }

    WIN32_FIND_DATA find;
    HANDLE handle = FindFirstFile((temp + what).c_str(), &find);
    for (BOOL flag = handle != INVALID_HANDLE_VALUE; flag; flag = FindNextFile(handle, &find)) {
        std::wstring name = find.cFileName;
        if (!(find.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
            files.push_back(temp + name);
        }
    }
    if (handle != INVALID_HANDLE_VALUE) {
        FindClose(handle);
    }
    return files;
}

NAMESPACE_CLOSE(File)

NAMESPACE_START(Directory)

bool isExists(const std::wstring& path)
{
    DWORD attr = GetFileAttributes(path.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES)
        return false;

    return (attr&FILE_ATTRIBUTE_NORMAL) && (attr&FILE_ATTRIBUTE_DIRECTORY);
}

void prepare(const std::wstring& path)
{
    std::wstring temp;
    auto names = String::split(path, L'\\');
    for (std::wstring& name : names) {
        temp += name + L'\\';
        CreateDirectory(temp.c_str(), NULL);
    }
}

void cleanup(const std::wstring& path)
{
    auto files = File::search(path, L"*");
    for (std::wstring& file : files) {
        DeleteFile(file.c_str());
    }
    DeleteFile(path.c_str());
}

bool canWrite(const std::wstring& path)
{
    if (path.empty()) return false;

    File::FileWriter writer;
    std::wstring file = path + L".6roomtest";
    if (writer.open(file)) {
        writer.close();
        DeleteFile(file.c_str());
        return true;
    }
    return false;
}

std::vector<std::wstring> search(const std::wstring& path, const std::wstring& what)
{
    std::vector<std::wstring> directories;

    std::wstring temp = path;
    if (temp[temp.size() - 1] != L'\\') {
        temp.append(1, '\\');
    }

    WIN32_FIND_DATA find;
    HANDLE handle = FindFirstFile((temp + what).c_str(), &find);
    for (BOOL flag = handle != INVALID_HANDLE_VALUE; flag; flag = FindNextFile(handle, &find)) {
        std::wstring name = find.cFileName;
        if (name == L"." || name == L"..") continue;
        if (find.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
            directories.push_back(temp + name);
        }
    }
    if (handle != INVALID_HANDLE_VALUE) {
        FindClose(handle);
    }
    return directories;
}

NAMESPACE_CLOSE(Directory)

NAMESPACE_START(String)

std::vector<std::wstring> split(const std::wstring& text, wchar_t ch)
{
    std::wstring token;
    std::vector<std::wstring> tokens;
    for (auto itr = text.begin(); itr != text.end(); itr++) {
        if (*itr != ch) {
            token.append(1, *itr);
        } else if (!token.empty()) {
            tokens.push_back(token);
            token.clear();
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::wstring> split(const std::wstring& text)
{
    std::wstring temp = text;
    replace(temp, L'\r', L'\n');
    return split(temp, L'\n');
}

void toLower(std::wstring& text)
{
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] >= L'A' && text[i] <= L'Z') {
            text[i] = text[i] - L'A' + L'a';
        }
    }
}

void toUpper(std::wstring& text)
{
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] >= L'a' && text[i] <= L'z') {
            text[i] = text[i] + L'A' - L'a';
        }
    }
}

void replace(std::wstring& text, wchar_t old, wchar_t rep)
{
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == old) {
            text[i] = rep;
        }
    }
}

std::wstring replace(const std::wstring& text, const std::wstring& old, const std::wstring& rep)
{
    if (old.empty()) return text;
    //naive
    std::wstring target;
    size_t pos = 0;
    while (pos < text.size()) {
        size_t nex = text.find(old, pos);
        if (nex == std::wstring::npos) {
            target.append(text.substr(pos));
            break;
        }

        if (nex != pos) {
            target.append(text.substr(pos, nex - pos));
        }

        target.append(rep);
        pos = nex + old.size();
    }
    return target;
}

std::wstring trim(const std::wstring&text, wchar_t ch)
{
    if (text.empty()) return text;

    size_t from = 0;
    for (; text[from] == ch; from++) {}
    if (from == text.size()) return text;

    size_t last = text.size() - 1;
    for (; text[last] == ch; last--) {}
    return text.substr(from, last - from + 1);
}

std::wstring format(const wchar_t* what, ...)
{
    wchar_t text[2048];

    va_list args;
    va_start(args, what);
    vswprintf_s(text, 2000, what, args);
    va_end(args);

    return text;
}

NAMESPACE_CLOSE(String)

NAMESPACE_CLOSE(System)