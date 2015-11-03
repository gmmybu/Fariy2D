#include "System.h"
#include "FileSystem.h"
using namespace System;

class LocalFileSystem : public FileSystem
{
public:
    LocalFileSystem(const std::wstring& path);

    void* read(const std::wstring& file, size_t& size);
    void  free(void* data);
private:
    std::wstring path;
};

LocalFileSystem::LocalFileSystem(const std::wstring& path)
{
    this->path = path;
}

void* LocalFileSystem::read(const std::wstring& file, size_t& size)
{
    File::FileReader reader;
    if (!reader.open(path + file))
        return NULL;

    DWORD fileSize = reader.getFileSize();
    if (fileSize == INVALID_FILE_SIZE)
        return NULL;

    void* data = malloc(fileSize + 4);
    memset((char*)data + fileSize, 0, 4);

    if (!reader.read(data, fileSize)) {
        ::free(data);
        return NULL;
    }

    size = fileSize;
    return data;
}

void LocalFileSystem::free(void* data)
{
    ::free(data);
}

FileSystem* FileSystem::current()
{
    static LocalFileSystem* system;
    if (system == NULL) {
        wchar_t file[MAX_PATH];
        GetModuleFileName(NULL, file, MAX_PATH);
        system = new LocalFileSystem(File::getPath(file) + L"\\Resource\\");
    }
    return system;
}