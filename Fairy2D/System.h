#pragma once
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <windows.h>

#define NOCOPYASSIGN(clazz)         \
    clazz(const clazz&);            \
    clazz& operator=(const clazz&); \
                                    \
    clazz(clazz&&);                 \
    clazz& operator=(clazz&&)

#define NAMESPACE_START(space) namespace space {
#define NAMESPACE_CLOSE(space) }

NAMESPACE_START(System)

class Mutex
{
    NOCOPYASSIGN(Mutex);
public:
    Mutex()
    {
        InitializeCriticalSectionAndSpinCount(&_section, 100);
    }

    ~Mutex()
    {
        DeleteCriticalSection(&_section);
    }

    void enter()
    {
        EnterCriticalSection(&_section);
    }

    void leave()
    {
        LeaveCriticalSection(&_section);
    }
private:
    CRITICAL_SECTION    _section;
};

class MutexGuard
{
    NOCOPYASSIGN(MutexGuard);
public:
    MutexGuard(Mutex& mutex) : _mutex(mutex)
    {
        mutex.enter();
    }

    ~MutexGuard()
    {
        _mutex.leave();
    }
private:
    Mutex&   _mutex;
};

NAMESPACE_START(Encoding)

std::string  unicodeToUTF8(const wchar_t* text);
std::string  unicodeToUTF8(const std::wstring& text);

std::wstring UTF8ToUnicode(const char* utf8);
std::wstring UTF8ToUnicode(const std::string& utf8);

NAMESPACE_CLOSE(Encoding)

NAMESPACE_START(File)

class FileReader
{
    NOCOPYASSIGN(FileReader);
public:
    FileReader();
    ~FileReader();

    bool open(const std::wstring& file , DWORD createflag = OPEN_EXISTING, DWORD sharemode = 0);
    void close();

    bool read(void* data, DWORD count);
    bool seek(DWORD length, DWORD method);

    size_t getFileSize();
    size_t getPosition();

    HANDLE    _hFile;
};

class FileWriter
{
    NOCOPYASSIGN(FileWriter);
public:
    FileWriter();
    ~FileWriter();

    bool open(const std::wstring& file , DWORD createflag = CREATE_ALWAYS, DWORD sharemode = 0);
    void close();

    bool write(const void* data, DWORD count);
    bool seek(DWORD length, DWORD method);

    size_t getFileSize();
    size_t getPosition();

    HANDLE    _hFile;
};

bool isExists(const std::wstring& file);

std::wstring getTempPath();
std::wstring getCurrentPath();
std::wstring getAppDataPath();
std::wstring getProgramFile();

std::wstring getPath(const std::wstring& file);
std::wstring getName(const std::wstring& file, bool clean = false);

std::vector<std::wstring> search(const std::wstring& path, const std::wstring& what);

NAMESPACE_CLOSE(File)

NAMESPACE_START(Directory)

bool isExists(const std::wstring& path);
bool canWrite(const std::wstring& path);

void prepare(const std::wstring& path);
void cleanup(const std::wstring& path);

std::vector<std::wstring> search(const std::wstring& path, const std::wstring& what);

NAMESPACE_CLOSE(Directory)

NAMESPACE_START(String)

std::vector<std::wstring> split(const std::wstring& text, wchar_t ch);
std::vector<std::wstring> split(const std::wstring& text);

void toLower(std::wstring& text);
void toUpper(std::wstring& text);
void replace(std::wstring& text, wchar_t old, wchar_t rep);

std::wstring trim(const std::wstring&text, wchar_t ch = L' ');
std::wstring format(const wchar_t* form, ...);
std::wstring replace(const std::wstring& text, const std::wstring& old, const std::wstring& rep);

NAMESPACE_CLOSE(String)


NAMESPACE_CLOSE(System)