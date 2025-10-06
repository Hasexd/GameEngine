#pragma once

#include <unordered_map>
#include <filesystem>
#include <functional>
#include <thread>

namespace Core
{

	enum class FileStatus
	{
		Created,
		Modified,
		Deleted
	};

	class FileWatcher
	{
	public:
		FileWatcher() = default;
		FileWatcher(const std::string& pathToWatch, std::chrono::duration<int, std::milli> delay);

		void Start(const std::function<void(std::string, FileStatus)>& action);
		
	private:
		bool Contains(const std::string& key);
	private:
		std::string m_PathToWatch;
		std::chrono::duration<int, std::milli> m_Delay;
		std::unordered_map<std::string, std::filesystem::file_time_type> m_Paths;
		bool m_Running = false;
	};
}