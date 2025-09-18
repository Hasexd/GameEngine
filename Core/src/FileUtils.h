#pragma once

#include <string>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Core
{
    class FileUtils
    {
    public:
        static std::string GetExecutableDir()
        {
#ifdef _WIN32
            char path[MAX_PATH];
            GetModuleFileNameA(NULL, path, MAX_PATH);
            std::string execPath(path);
            return execPath.substr(0, execPath.find_last_of("\\/"));
#else
            char path[1024];
            ssize_t count = readlink("/proc/self/exe", path, 1024);
            std::string execPath(path, (count > 0) ? count : 0);
            return execPath.substr(0, execPath.find_last_of("/"));
#endif
        }

        static std::string GetAssetPath(const std::string& assetName)
        {
            return GetExecutableDir() + "/assets/" + assetName;
        }
    };
}