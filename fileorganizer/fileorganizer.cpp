#include "filemanager.h"
#include "logger.h"

#include <iostream>
#include <map>
#include <string>

enum struct Choice
{
    AddDirectory = 1,
    RemoveDirectory,
    SelectExtensions,
    ListFiles,
    Quit,
};

class ConsoleLogger : public Logger
{
public:
    void Log(const LoggerType type, const std::string& message)
    {
        std::cout << message << "\n";
    }
};

void Clear()
{
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

void Pauze()
{
    char ch;
    while (std::cin.readsome(&ch, 1) != 0);
    std::cout << "Press any key to continue...";
    getchar();
}

void SelectExtensions(FileManager& filemanager)
{
    int choice = 0;
    do {
        Clear();
        auto index = 0;
        const auto& extensions = filemanager.GetExtensions();
        for (const auto& extension : extensions)
        {
            std::cout << ++index << " - ";
            std::cout << "[";
            if (extension.selected)
            {
                std::cout << "x";
            }
            else
            {
                std::cout << " ";
            }
            std::cout << "] ";
            if (extension.extension.empty())
            {
                std::cout << "no extension";
            }
            else
            {
                std::cout << extension.extension;
            }
            std::cout << " " << extension.count << "x\n";
        }
        std::cout << "0 - return to Main Menu\n";
        std::cout << "Selection: ";
        std::cin >> choice;

        const auto indexextension = choice - 1;
        if (indexextension >= 0 && indexextension < extensions.size())
        {
            const auto& selectedextension = extensions[indexextension];
            filemanager.SelectExtension(selectedextension.extension, !selectedextension.selected);
        }
    } while (choice != 0);
}

void AddDirectory(FileManager& filemanager)
{
    Clear();
    std::cout << "Directory: ";
    std::string directory;
    std::cin >> directory;
    filemanager.AddDirectory(directory);
}

void RemoveDirectories(FileManager& filemanager)
{
    int choice = 0;
    do {
        Clear();
        auto index = 0;
        const auto& directories = filemanager.GetDirectories();
        for (const auto& directory : directories)
        {
            std::cout << ++index << " - ";
            std::cout << directory;
            std::cout << "\n";
        }
        std::cout << "0 - return to Main Menu\n";
        std::cout << "Selection: ";
        std::cin >> choice;

        const auto indexdirectory = choice - 1;
        if (indexdirectory >= 0 && indexdirectory < directories.size())
        {
            const auto& selecteddirectory = directories[indexdirectory];
            filemanager.RemoveDirectory(selecteddirectory);
        }
    } while (choice != 0);
}

void ListFiles(const FileManager& filemanager)
{
    Clear();

    bool result = false;
    for (const auto& file : filemanager.GetFiles())
    {
        if (!filemanager.CheckExtension(file.extension))
        {
            continue;
        }

        std::cout << file.path;

        std::cout << " File size:";
        if (file.fileInfo.st_size > (1024 * 1024 * 1204))
        {
            auto filesize = file.fileInfo.st_size / (1024 * 1024 * 1204);
            std::cout << filesize << "Gb";
        }
        else if (file.fileInfo.st_size > (1024 * 1024))
        {
            auto filesize = file.fileInfo.st_size / (1024 * 1024);
            std::cout << filesize << "Mb";
        }
        else if (file.fileInfo.st_size > 1024)
        {
            auto filesize = file.fileInfo.st_size / 1024;
            std::cout << filesize << "Kb";
        }
        else
        {
            std::cout << file.fileInfo.st_size << "byte";
        }
        std::cout << "\n";
        result = true;
    }
    if (!result)
    {
        std::cout << "No files!";
    }
}

int main()
{
    std::map<Choice, std::string> choices = {
        {Choice::AddDirectory, "Add directory" },
        {Choice::RemoveDirectory, "Remove directory" },
        {Choice::SelectExtensions, "Select extensions" },
        {Choice::ListFiles, "List files" },
        {Choice::Quit, "Quit" },
    };

    FileManager filemanager;
    ConsoleLogger logger;

    int ichoice = static_cast<int>(Choice::Quit);
    do {
        Clear();

        std::cout << "Main Menu\n";
        std::cout << "Please make your selection\n";

        for (const auto& choice : choices)
        {
            std::cout << static_cast<int>(choice.first) << " - " << choice.second << "\n";
        }
        std::cout << "\n";
        std::cout << "Total " << filemanager.GetFiles().size() << " files found\n";
        std::cout << "\n";
        std::cout << "Selection: ";
        std::cin >> ichoice;

        switch (static_cast<Choice>(ichoice)) {
        case Choice::AddDirectory: {
            AddDirectory(filemanager);
            Update(filemanager, logger);
            Pauze();
            break;
        }
        case Choice::RemoveDirectory: {
            RemoveDirectories(filemanager);
            Update(filemanager, logger);
            Pauze();
            break;
        }
        case Choice::SelectExtensions: {
            SelectExtensions(filemanager);
            break;
        }
        case Choice::ListFiles: {
            ListFiles(filemanager);
            Pauze();
            break;
        }
        case Choice::Quit: {
            std::cout << "Goodbye!";
            break;
        }
        }
    } while (ichoice != static_cast<int>(Choice::Quit));
}