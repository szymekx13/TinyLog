/*
* Copyright (c) 2025 Szymon Niedbała
 *
 * Licensed under the MIT License.
 * You may obtain a copy of the License at https://opensource.org/licenses/MIT
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
#include <stdexcept>

namespace LOG {

    //ENUM FOR LOG LEVELS
    enum class Level {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };
    //LOG FILE AND MUTEX FOR THREAD SAFETY
    extern  std::ofstream logFile;
    extern  std::mutex logMutex;
    extern  Level currentLevel;
    extern bool useColor;
    extern bool useTimestamp;
    extern Level consoleLevel;
    extern Level fileLevel;


    inline void set_console_level(Level lvl) {
        consoleLevel = lvl;
    }
    inline void set_file_level(Level lvl) {
        fileLevel = lvl;
    }
    //ANSI COLOR CODES
    inline std::string color_for(Level lvl) {
        switch (lvl) {
            case Level::INFO: return "\033[32m"; break; // green
            case Level::WARN: return "\033[33m"; break; // yellow
            case Level::ERROR: return "\033[31m"; break; // red
            case Level::TRACE: return "\033[95m"; break; // bright magenta
            case Level::DEBUG: return "\033[36m"; break; // cyan
            case Level::FATAL: return "\x1b[38;5;1m"; break; // bright red
                default: return "\033[0m"; // reset
        }
    }

    void rotate_logs();
    void set_max_file_size(size_t bytes);
    void set_max_backup_files(int count);
    //LOG LEVEL NAMES
    inline std::string level_name(Level lvl) {
        switch (lvl) {
            case Level::INFO: return "INFO"; break;
            case Level::WARN: return "WARN"; break;
            case Level::ERROR: return "ERROR"; break;
            case Level::FATAL: return "FATAL"; break;
            case Level::TRACE: return "TRACE"; break;
            case Level::DEBUG: return "DEBUG"; break;
        }
        return "UNKNOWN";
    }

    //helper function to use colors in [TinyLog] messages
    inline void printSystemMsg(const std::string& msg) {
        const std::string white = "\033[97m";
        const std::string cyan = "\033[36m";
        const std::string reset = "\033[0m";
        std::cout << white << "[" << cyan << "TinyLog" << white << "] "<< reset << msg << std::endl;
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
    template <typename... T>
    std::string format(const std::string& fmt, T&&... args) {
        size_t size = std::snprintf(nullptr, 0, fmt.c_str(), std::forward<T>(args)...) +1; // Extra space for '\0'
        std::string result(size, '\0');
        std::snprintf(&result[0], size, fmt.c_str(), std::forward<T>(args)...);
        result.pop_back(); // remove the extra null character
        return result;
    }

    //CORE LOGGING FUNCTION
    template <typename... T>
    void log(Level lvl, const std::string& fmt, T&&... args) {
        auto color = color_for(lvl);
        auto reset = "\033[0m";
        auto msg = format(fmt, std::forward<T>(args)...);

        std::ostringstream line;
        line << "["<< timestamp() << "] "
             << color << "[" << level_name(lvl) <<"] "
             << reset << msg << std::endl;

        //Log to console and file
        {
            std::lock_guard<std::mutex> lock(logMutex);

            if (lvl >= consoleLevel) {
                std::cout << color << line.str() << reset << std::endl;
            }
            if (lvl >= fileLevel) {
                //log to file if initialized
                if (logFile.is_open()) {
                    logFile << line.str();
                    logFile.flush();
                }
            }
        }
    }

    // Funkcja pomocnicza dla błędów krytycznych z numerem wiersza i nazwą funkcji
    template <typename... T>
    void fatal_impl(const std::string& fmt, int line, const char* func, T&&... args) {
        std::string new_fmt = fmt + " [at line %d in %s]";
        log(Level::FATAL, new_fmt, std::forward<T>(args)..., line, func);
        throw std::runtime_error("Error: Fatal error occurred. Check logs for details.");
    }

    // The macro passes the line number and the function name as the 2nd and 3rd arguments


    //LOGGING FUNCTIONS
    template <typename... T>
    void info(const std::string& fmt, T&&... args) {
        log(Level::INFO, fmt, std::forward<T>(args)...);
    }
    template <typename... T>
    void warn(const std::string& fmt, T&&... args) {
        log(Level::WARN, fmt, std::forward<T>(args)...);
    }
    template <typename... T>
    void error(const std::string& fmt, T&&... args) {
        log(Level::ERROR, fmt, std::forward<T>(args)...);
    }
    template <typename... T>
    void trace(const std::string& fmt, T&&... args) {
        log(Level::TRACE, fmt, std::forward<T>(args)...);
    }
    template <typename... T>
    void debug(const std::string& fmt, T&&... args) {
        log(Level::DEBUG, fmt, std::forward<T>(args)...);
    }
    //TRIM WHITESPACES
    static std::string trim(const std::string& s) {
        auto start = s.find_first_not_of("\t\n\r");
        auto end = s.find_last_not_of("\t\n\r");
        return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
    }
    //INITIALIZE LOGGING TO FILE
    void init(const std::string& fname = "log.txt");
    void loadConfig(const std::string& path = "config.json");

}

#define LOG_FATAL(fmt, ...) LOG::fatal_impl(fmt, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG::info(fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG::warn(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG::error(fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG::debug(fmt, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) LOG::trace(fmt, ##__VA_ARGS__)