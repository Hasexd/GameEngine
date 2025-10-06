#include "FileWatcher.h"

namespace Core
{
	FileWatcher::FileWatcher(const std::string& pathToWatch, std::chrono::duration<int, std::milli> delay) :
		m_PathToWatch(pathToWatch), m_Delay(delay)
	{
		for (const auto& file : std::filesystem::recursive_directory_iterator(pathToWatch))
		{
			m_Paths[file.path().string()] = std::filesystem::last_write_time(file);
		}

		m_Running = true;
	}

	void FileWatcher::Start(const std::function<void(const std::string&, FileStatus)>& action)
	{
		while (m_Running)
		{
			std::this_thread::sleep_for(m_Delay);

			auto it = m_Paths.begin();

			while (it != m_Paths.end())
			{
				if(!std::filesystem::exists(it->first))
				{
					action(it->first, FileStatus::Deleted);
					it = m_Paths.erase(it);
				}
				else
				{
					++it;
				}
			}

			for (auto& file : std::filesystem::recursive_directory_iterator(m_PathToWatch))
			{
				auto currentFileLastWriteTime = std::filesystem::last_write_time(file);

				if (!Contains(file.path().string()))
				{
					m_Paths[file.path().string()] = currentFileLastWriteTime;
					action(file.path().string(), FileStatus::Created);
				}
				else
				{
					if(m_Paths[file.path().string()] != currentFileLastWriteTime)
					{
						m_Paths[file.path().string()] = currentFileLastWriteTime;
						action(file.path().string(), FileStatus::Modified);
					}
				}

			}
		}
	}

	void FileWatcher::Stop()
	{
		m_Running = false;
	}

	bool FileWatcher::Contains(const std::string& key)
	{
		auto it = m_Paths.find(key);

		return it != m_Paths.end();
	}
}