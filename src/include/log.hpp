#pragma once

#include "include/ascii.hpp"
#include <experimental/propagate_const>
#include <format>
#include <print>
#include <utility>

struct Log
{
    template <class... Args> static constexpr void info(std::format_string<Args...> fmt, Args &&...args)
    {
        if (!m_is_debug)
        {
            return;
        }

        std::print("{0} {1} INFO: {2}", ASCII::Colors::Foreground::WHITE, ASCII::Colors::Background::BLUE,
                   ASCII::Styles::CLEAR_STYLE);

        std::println(fmt, std::forward<Args>(args)...);
    }

    template <class... Args> static constexpr void warning(std::format_string<Args...> fmt, Args &&...args)
    {
        if (!m_is_debug)
        {
            return;
        }

        std::print("{0} {1} WARNING: {2}", ASCII::Colors::Foreground::WHITE, ASCII::Colors::Background::YELLOW,
                   ASCII::Styles::CLEAR_STYLE);

        std::println(fmt, std::forward<Args>(args)...);
    }

    template <class... Args> static constexpr void critical(std::format_string<Args...> fmt, Args &&...args)
    {
        std::print("{0} {1} CRITICAL: {2}", ASCII::Colors::Foreground::BLACK, ASCII::Colors::Background::WHITE,
                   ASCII::Styles::CLEAR_STYLE);

        std::println(fmt, std::forward<Args>(args)...);
    }

    template <class... Args> static constexpr void fatal(std::format_string<Args...> fmt, Args &&...args)
    {

        std::print("{0} {1} FATAL: {2}", ASCII::Colors::Foreground::WHITE, ASCII::Colors::Background::RED,
                   ASCII::Styles::CLEAR_STYLE);

        std::println(fmt, std::forward<Args>(args)...);
    }

    static constexpr void configureLogger(bool is_debug_mode_enabled = false)
    {
        if (is_debug_mode_enabled)
        {
            Log::m_is_debug = true;
        }
    }

  private:
    static bool m_is_debug;
}; // namespace Log

inline bool Log::m_is_debug{false};
