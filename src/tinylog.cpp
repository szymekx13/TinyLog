/*
* Copyright (c) 2025 Szymon Niedbała
 *
 * Licensed under the MIT License.
 * You may obtain a copy of the License at https://opensource.org/licenses/MIT
 */

#include "../include/tinylog.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace LOG {

    std::ofstream logFile;
    std::mutex logMutex;
    Level currentLevel = Level::TRACE;
    bool useColor = true;
    bool useTimestamp = true;

    void loadConfig(const std::string &path) {
        std::ifstream cnf(path);
        if (!cnf.is_open()) {
            printSystemMsg("No config.json found. Creating new one...\n");

            //deafult config
            std::ofstream create(path);
            create << R"({
    "log_file": "log.txt",
    "min_level": "INFO",
    "color": true,
    "timestamp": true
})";
            create.close();

            //after creating new config, load it
            cnf.open(path);
            if (!cnf.is_open()) {
                printSystemMsg("Unable to create config.json. Using defaults ");
                return;
            }
        }else {
            printSystemMsg("Found config.json. Loading settings...\n");
        }

        std::stringstream buffer;
        buffer << cnf.rdbuf();
        std::string json = buffer.str();

        // Log file
        std::string log_filename = "log.txt";
        if (json.find("\"log_file\"") != std::string::npos) {
            size_t pos = json.find("\"log_file\"");
            size_t start = json.find(":", pos) + 1;
            size_t end = json.find(",", start);
            log_filename = trim(json.substr(start, end - start));
        }
        logFile.open(log_filename, std::ios_base::app);

        // Log level
        if (json.find("\"min_level\"") != std::string::npos) {
            size_t pos = json.find("\"min_level\"");
            size_t start = json.find(":", pos) + 1;
            size_t end = json.find(",", start);
            std::string level_str = trim(json.substr(start, end - start));

            if (level_str == "\"TRACE\"") currentLevel = Level::TRACE;
            else if (level_str == "\"DEBUG\"") currentLevel = Level::DEBUG;
            else if (level_str == "\"INFO\"") currentLevel = Level::INFO;
            else if (level_str == "\"WARN\"") currentLevel = Level::WARN;
            else if (level_str == "\"ERROR\"") currentLevel = Level::ERROR;
            else if (level_str == "\"FATAL\"") currentLevel = Level::FATAL;
        }

        // Flags
        useColor = json.find("\"color\": false") == std::string::npos;
        useTimestamp = json.find("\"timestamp\": false") == std::string::npos;
    }

    void init(const std::string &filename) {
        loadConfig();

        if (!logFile.is_open()) {
            logFile.open(filename, std::ios_base::app);
        }

        if (!logFile.is_open()) {
            std::cerr << "[TinyLog] Failed to open log file: " << filename
                      << ". Logging to console only.\n";
        }
    }

}