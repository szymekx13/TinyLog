/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org/>
*/

#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <mutex>

namespace log {

    //ENUM FOR LOG LEVELS
    enum class Level {
        INFO,
        WARN,
        ERROR
    };

    //LOG FILE AND MUTEX FOR THREAD SAFETY
    inline std::ofstream logFile;
    inline std::mutex logMutex;

    //ANSI COLOR CODES
    inline std::string color_for(Level lvl) {
        switch (lvl) {
            case Level::INFO: return "\033[32m"; break; // green
            case Level::WARN: return "\033[33m"; break; // yellow
            case Level::ERROR: return "\033[31m"; break; // red
        }
    }

    //LOG LEVEL NAMES
    inline std::string level_name(Level lvl) {
        switch (lvl) {
            case Level::INFO: return "INFO"; break;
            case Level::WARN: return "WARN"; break;
            case Level::ERROR: return "ERROR"; break;
        }
        return "UNKNOWN";
    }

    //GET CURRENT TIMESTAMP
    inline std::string timestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm {};
    #ifdef _WIN32
        localtime_s(&local_tm, &t);
    #else
        localtime_r(&t, &local_tm);
    #endif
        std::ostringstream oss;
        oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    // EASY HELPER FOR FORMATTING STRINGS
    template <typename T>
    std::string format(const std::string& fmt, T&& args) {
        size_t size = std::snprintf(nullptr, 0, fmt.c_str(), args) +1; // Extra space for '\0'
        std::string result(size, '\0');
        std::snprintf(&result[0], size, fmt.c_str(), args);
        result.pop_back(); // remove the extra null character
        return result;
    }

    //CORE LOGGING FUNCTION
    template <typename T>
    void log(Level lvl, const std::string& fmt, T&& args) {
        auto color = color_for(lvl);
        auto reset = "\033[0m";
        auto msg = format(fmt, std::forward<T>(args));

        std::ostringstream line;
        line << "["<< timestamp() << "] "
             << color << "[" << level_name(lvl) <<"] "
             << reset << msg << std::endl;

        //Log to console and file
        {
            std::lock_guard<std::mutex> lock(logMutex);
            std::cout << color << line.str() << reset << std::endl;

            //log to file if initialized
            if (logFile.is_open()) {
                logFile << line.str();
                logFile.flush();
            }
        }
    }

    //LOGGING FUNCTIONS
    template <typename T>
    void info(const std::string& fmt, T&& args) {
        log(Level::INFO, fmt, std::forward<T>(args));
    }
    template <typename T>
    void warn(const std::string& fmt, T&& args) {
        log(Level::WARN, fmt, std::forward<T>(args));
    }
    template <typename T>
    void error(const std::string& fmt, T&& args) {
        log(Level::ERROR, fmt, std::forward<T>(args));
    }

    //INITIALIZE LOGGING TO FILE
    inline void init(const std::string& filename = "log.txt") {
        logFile.open(filename, std::ios_base::app); //append mode, adding to existing logs
        if (!logFile.is_open()) {
            std::cerr << "Failed to open file " << filename << std::endl;
        }
    }
}