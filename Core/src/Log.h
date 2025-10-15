#pragma once

#include <print>
#include <source_location>
#include <format>
#include <string>

enum class LogLevel : uint8_t
{
	Trace,
	Debug,
	Info,
	Warning,
	Error,
	Critical
};

namespace
{
	void LogInternal(LogLevel level, const std::string& message, const std::source_location& loc = std::source_location::current())
	{
		const char* levelStr = nullptr;
		int textColor = 37;

		switch (level)
		{
		case LogLevel::Trace:
			levelStr = "[TRACE]";
			textColor = 32;
			break;
		case LogLevel::Debug:
			levelStr = "[DEBUG]";
			textColor = 36;
			break;
		case LogLevel::Info:
			levelStr = "[INFO]";
			textColor = 34;
			break;
		case LogLevel::Warning:
			levelStr = "[WARNING]";
			textColor = 33;
			break;
		case LogLevel::Error:
			levelStr = "[ERROR]";
			textColor = 35;
			break;
		case LogLevel::Critical:
			levelStr = "[CRITICAL]";
			textColor = 31;
			break;
		}

		std::string filePath = std::string(loc.file_name());
		size_t lastSlash = filePath.find_last_of("/\\");
		std::string fileName = (lastSlash == std::string::npos) ? filePath : filePath.substr(lastSlash + 1);

		std::print("\033[1;{}m", textColor);
		std::println("{} {}:{} - {}", levelStr, fileName, loc.line(), message);
		std::print("\033[0m");
	}
}

#ifndef DIST
#define LOG_TRACE(...) LogInternal(LogLevel::Trace, std::format(__VA_ARGS__))
#define LOG_DEBUG(...) LogInternal(LogLevel::Debug, std::format(__VA_ARGS__))
#define LOG_INFO(...) LogInternal(LogLevel::Info, std::format(__VA_ARGS__))
#define LOG_WARN(...) LogInternal(LogLevel::Warning, std::format(__VA_ARGS__))
#define LOG_ERROR(...) LogInternal(LogLevel::Error, std::format(__VA_ARGS__))
#define LOG_CRITICAL(...) LogInternal(LogLevel::Critical, std::format(__VA_ARGS__))

#define ASSERT(condition) \
		do { \
			if (!(condition)) { \
				LOG_CRITICAL("ASSERTION FAILED: {}", #condition); \
				std::abort(); \
			} \
		} while (false)

#define ASSERT_MSG(condition, message) \
		do { \
			if (!(condition)) { \
				LOG_CRITICAL("ASSERTION FAILED: {} - {}", #condition, message); \
				std::abort(); \
			} \
		} while(0)

#else
#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRITICAL(...)
#define ASSERT(condition, ...)
#define ASSERT_MSG(condition, message)

#endif