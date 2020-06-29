#pragma once

#include "logger.h"

#include <optional>
#include <string>
#include <vector>

struct File
{
    std::string path;
    std::string extension;
    struct stat fileInfo;
};

struct Extension
{
    std::string extension;
    size_t count;
    bool selected;
};

class FileManager
{
public:
    FileManager();
    virtual ~FileManager();

    void ClearFiles();

    void AddDirectory(const std::string& directory);
    void RemoveDirectory(const std::string& directory);
    void AddFile(const File& file);
    void RemoveFile(const std::string& file);

    bool SelectExtension(const std::string& extension, bool select);
    bool CheckExtension(const std::string& extension) const;
    void SortExtensions();

    void SortFiles();
    void UpdateFiles();

    const std::vector<std::string>& GetDirectories() const;
    const std::vector<File>& GetFiles() const;
    const std::vector<Extension>& GetExtensions() const;

private:
    std::vector<std::string> directories_;
    std::vector<File> files_;
    std::vector<Extension> extensions_;
};

void Update(FileManager& filemanager, Logger& logger);