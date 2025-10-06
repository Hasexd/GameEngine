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
		static std::string GetCoreProjectDir()
		{
#ifdef _WIN32
			char exePath[MAX_PATH];
			GetModuleFileNameA(NULL, exePath, MAX_PATH);
			std::string execDir(exePath);
			execDir = execDir.substr(0, execDir.find_last_of("\\/"));

			std::filesystem::path binPath(execDir);
			std::filesystem::path projectRoot = binPath.parent_path().parent_path().parent_path();
			std::filesystem::path coreDir = projectRoot / "Core";

			return coreDir.string();
#else
			char path[1024];
			ssize_t count = readlink("/proc/self/exe", path, 1024);
			std::string execDir(path, (count > 0) ? count : 0);
			execDir = execDir.substr(0, execDir.find_last_of("/"));

			std::filesystem::path binPath(execDir);
			std::filesystem::path projectRoot = binPath.parent_path().parent_path().parent_path();
			std::filesystem::path coreDir = projectRoot / "Core";

			return coreDir.string();
#endif
		}

		static std::string GetFileName(const std::filesystem::path& path)
		{
			std::string fileString = path.string();
			size_t lastSlash = fileString.find_last_of("/\\");
			return (lastSlash == std::string::npos) ? fileString : fileString.substr(lastSlash + 1);
		}

        static std::string GetShaderPath(const std::string& shaderName)
        {
            return GetCoreProjectDir() + "/shaders/" + shaderName;
        }
    };
}