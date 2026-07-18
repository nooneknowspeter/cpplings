#include <experimental/propagate_const>
#include <format>
#include <memory>

struct Log
{

    void configureLogger(bool is_debug_mode_enabled);

    template <class... Args> void Info(std::format_string<Args...> fmt, Args &&...args);

    template <class... Args> void Warning(std::format_string<Args...> fmt, Args &&...args);

    template <class... Args> void Critical(std::format_string<Args...> fmt, Args &&...args);

    template <class... Args> void Fatal(std::format_string<Args...> fmt, Args &&...args);

  private:
    struct Impl;
    std::experimental::propagate_const<std::unique_ptr<Log::Impl>> pImpl;
};
