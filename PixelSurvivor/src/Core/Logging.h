#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Pixel
{

	class Logger
	{
	private:
		static bool s_Initialized;
		static std::shared_ptr<spdlog::logger> s_Logger;

	public:
		static void Init();
		static inline spdlog::logger& GetLogger() { return *s_Logger; }
	};

}

#ifdef PIX_DIST 
#define PIX_TRACE(...)
#define PIX_INFO(...)
#define PIX_WARN(...)
#define PIX_ERROR(...)
#define PIX_FATAL(...)

#define PIX_ASSERT(arg, ...)

#define PIX_DEBUG_ONLY(x)
#else

#define PIX_TRACE(...) ::Pixel::Logger::GetLogger().trace(__VA_ARGS__)
#define PIX_INFO(...) ::Pixel::Logger::GetLogger().info(__VA_ARGS__)
#define PIX_WARN(...) ::Pixel::Logger::GetLogger().warn(__VA_ARGS__)
#define PIX_ERROR(...) ::Pixel::Logger::GetLogger().error(__VA_ARGS__)
#define PIX_FATAL(...) ::Pixel::Logger::GetLogger().critical(__VA_ARGS__)

#ifdef PIX_PLATFORM_WINDOWS
#define PIX_ASSERT(arg, ...) { if (!(arg)) { PIX_FATAL(__VA_ARGS__); __debugbreak(); } }
#else
#define PIX_ASSERT(arg, ...) { if (!(arg)) { PIX_FATAL(__VA_ARGS__); } }
#endif

#define PIX_DEBUG_ONLY(x) x
#endif
