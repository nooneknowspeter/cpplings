#include "include/log.hpp"
#include "include/ascii.hpp"
#include <print>
#include <utility>

enum class LogLevel
{
    Info,
    Warning,
    Critical,
    Fatal,
};

// implementation
struct Log::Impl
{
    static bool m_is_debug;

    template <class... Args>
    static void printLogLevel(LogLevel log_level, std::format_string<Args...> fmt, Args &&...args)
    {
        switch (log_level)
        {
        case LogLevel::Info:
            if (!m_is_debug)
            {
                return;
            }

            std::print("{0} {1} INFO: {2}", ASCII::Colors::Foreground::white, ASCII::Colors::Background::blue,
                       ASCII::Styles::clear_style);
        case LogLevel::Warning:
            if (!m_is_debug)
            {
                return;
            }

            std::print("{0} {1} WARNING: {2}", ASCII::Colors::Foreground::white, ASCII::Colors::Background::yellow,
                       ASCII::Styles::clear_style);
        case LogLevel::Critical:
            std::print("{0} {1} CRITICAL: {2}", ASCII::Colors::Foreground::black, ASCII::Colors::Background::white,
                       ASCII::Styles::clear_style);
        case LogLevel::Fatal:
            std::print("{0} {1} FATAL: {2}", ASCII::Colors::Foreground::white, ASCII::Colors::Background::red,
                       ASCII::Styles::clear_style);
        }

        std::println(fmt, std::forward(args)...);
    }
};

// facade api
template <class... Args> void Log::Info(std::format_string<Args...> fmt, Args &&...args)
{
    Log::Impl::printLogLevel(LogLevel::Info, fmt, std::forward(args)...);
}

void Log::configureLogger(bool is_debug_mode_enabled)
{
    Log::Impl::m_is_debug = is_debug_mode_enabled;
}
