#pragma once
#include <string>

class FileSystem
{
public:
    static FileSystem* current();

    virtual void* read(const std::wstring& file, size_t& size) = 0;
    virtual void  free(void* data) = 0;
};
