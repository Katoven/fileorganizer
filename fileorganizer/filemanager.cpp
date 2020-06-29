#include "filemanager.h"

#include "stringhelperfunctions.h"

#include <sys/stat.h>

#include <iostream>
#include <filesystem>

//#include <regex>
//static const std::regex imgextensions("\\.(?:BMP|ICO|JPEG|JPG|NEF|PNG|TIF|GIF)", std::regex_constants::icase);
//static const std::regex vidextensions("\\.(?:MOV|MPG|MP4|AVI)", std::regex_constants::icase);

FileManager::FileManager()
{

}

FileManager::~FileManager()
{
}

void FileManager::ClearFiles()
{
    files_.clear();
    extensions_.clear();
}

void FileManager::AddDirectory(const std::string& directory)
{
    directories_.push_back(directory);
}

void FileManager::RemoveDirectory(const std::string& directory)
{
    auto it = std::find_if(directories_.begin(), directories_.end(), [&](const auto& e) { return e == directory; });
    if (it != directories_.end())
    {
        directories_.erase(it);
    }
}

void FileManager::AddFile(const File& file)
{
    files_.push_back(file);

    auto findextension = std::find_if(extensions_.begin(), extensions_.end(), [&](const auto& e) {return iequal(e.extension, file.extension); });
    if (findextension == extensions_.end())
    {
        Extension newextension;
        newextension.extension = file.extension;
        newextension.count = 1;
        newextension.selected = false;
        extensions_.push_back(newextension);
    }
    else
    {
        findextension->count++;
    }
}

void FileManager::RemoveFile(const std::string& file)
{
}

bool FileManager::SelectExtension(const std::string& extension, bool select)
{
    auto findextension = std::find_if(extensions_.begin(), extensions_.end(), [&](const auto& e) {return iequal(e.extension, extension); });
    if (findextension == extensions_.end())
    {
        return false;
    }
    findextension->selected = select;
    return true;
}

bool FileManager::CheckExtension(const std::string& extension) const
{
    auto findextension = std::find_if(extensions_.begin(), extensions_.end(), [&](const auto& e) {return iequal(e.extension, extension); });
    if (findextension == extensions_.end())
    {
        return false;
    }
    return findextension->selected;
}

void FileManager::SortExtensions()
{
    std::sort(extensions_.begin(), extensions_.end(), [](const auto& a, const auto& b) {
        return a.count > b.count;
        });
}

void FileManager::SortFiles()
{
    // Sort files by alphabet
    std::sort(files_.begin(), files_.end(), [](const auto& a, const auto& b) {
        return a.path > b.path;
        });
}

void FileManager::UpdateFiles()
{
    // Get file info and filter files
    auto previt = files_.begin();
    for (auto it = previt + 1; it != files_.end(); ++it, ++previt)
    {
        if (previt->path == it->path)
        {
            // identical
            files_.erase(it);
            it = previt + 1;
            continue;
        }
        if (!std::filesystem::exists(it->path))
        {
            // file don't exist any more
            files_.erase(it);
            it = previt + 1;
            continue;
        }
        if (stat(it->path.c_str(), &it->fileInfo) != 0) {  // Use stat( ) to get the info
            files_.erase(it);
            it = previt + 1;
            continue;
        }
    }
}

const std::vector<std::string>& FileManager::GetDirectories() const
{
    return directories_;
}

const std::vector<File>& FileManager::GetFiles() const
{
    return files_;
}

const std::vector<Extension>& FileManager::GetExtensions() const
{
    return extensions_;
}

void Update(FileManager& filemanager, Logger& logger)
{
    logger.Log(LoggerType::Message, "Retrieving files...");

    filemanager.ClearFiles();

    for (const auto& rootdirectory : filemanager.GetDirectories())
    {
        std::vector<std::string> directories;
        directories.push_back(rootdirectory);

        for (auto index = 0; index < directories.size(); ++index)
        {
            try {
                for (const auto& entry : std::filesystem::directory_iterator(directories[index]))
                {
                    if (entry.is_directory())
                    {
                        directories.push_back(entry.path().string());
                    }
                    else if (entry.is_regular_file())
                    {
                        File newfile;
                        newfile.path = entry.path().string();
                        newfile.extension = entry.path().extension().string();
                        filemanager.AddFile(newfile);
                    }
                }
            }
            catch (const std::exception & e)
            {
                logger.Log(LoggerType::Error, e.what());
            }
        }
    }
    if (!filemanager.GetFiles().empty())
    {
        logger.Log(LoggerType::Message, "Sorting files...");

        filemanager.SortFiles();

        logger.Log(LoggerType::Message, "Get file info and filter files...");
        filemanager.UpdateFiles();
    }
    filemanager.SortExtensions();

    logger.Log(LoggerType::Message, "Finished...");
}