#pragma once

#include <string>

namespace ASCII
{

inline const std::string ART = R"(
                  _ _                 
                 | (_)                
  ___ _ __  _ __ | |_ _ __   __ _ ___ 
 / __| '_ \| '_ \| | | '_ \ / _` / __|
| (__| |_) | |_) | | | | | | (_| \__ \
 \___| .__/| .__/|_|_|_| |_|\__, |___/
     | |   | |               __/ |    
     |_|   |_|              |___/     
     
)";

struct Cursor
{
    static constexpr std::string CLEAR_TERMINAL = "\x1b[2J\x1b[H";
};

struct Styles
{
    static constexpr std::string BOLD = "\x1B[1m";
    static constexpr std::string UNDERLINE = "\x1B[4m";
    static constexpr std::string CLEAR_STYLE = "\x1B[0m";
};

struct Colors
{

    struct Foreground
    {
        static constexpr std::string BLACK = "\x1b[30m";
        static constexpr std::string WHITE = "\x1b[37m";
        static constexpr std::string RED = "\x1b[31m";
        static constexpr std::string GREEN = "\x1b[32m";
        static constexpr std::string BLUE = "\x1b[34m";
        static constexpr std::string YELLOW = "\x1b[33m";
    };

    struct Background
    {
        static constexpr std::string BLACK = "\x1b[40m";
        static constexpr std::string WHITE = "\x1b[47m";
        static constexpr std::string RED = "\x1b[41m";
        static constexpr std::string GREEN = "\x1b[42m";
        static constexpr std::string BLUE = "\x1b[44m";
        static constexpr std::string YELLOW = "\x1b[43m";
    };
};

}; // namespace ASCII
