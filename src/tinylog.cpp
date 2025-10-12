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
#include <filesystem>


namespace LOG {

    std::ofstream logFile;
    std::mutex logMutex;
    Level currentLevel = Level::TRACE;
    bool useColor = true;
    bool useTimestamp = true;
    Level consoleLevel = Level::TRACE;
    Level fileLevel = Level::INFO;
    namespace {
        std::string filename = "log.txt";
        size_t maxFileSize = 50 * 1024; // 5 MB
        int maxBackupFiles = 5;
    }

    void set_max_backup_files(int count) {
        maxBackupFiles = count;
    }
    void set_max_file_size(size_t bytes) {
        maxFileSize = bytes;
    }

    void rotate_logs() {
        namespace fs = std::filesystem;
        if (!fs::exists(filename)) {
            return;
        }
        // close the file before rotation
        if (logFile.is_open()) {
            logFile.close();
        }
        auto size = fs::file_size(filename);
        if (size < maxFileSize) {
            logFile.open(filename, std::ios_base::app);
            return;
        }
        auto stem = fs::path(filename).stem().string();
        auto ext = fs::path(filename).extension().string();

        for (int i = maxBackupFiles - 1; i>=1; --i) {
            auto OldName = stem + "_" + std::to_string(i) + ext;
            auto NewName = stem + "_" + std::to_string(i+1) + ext;
            if (fs::exists(OldName)) {
                fs::rename(OldName, NewName);
            }
        }
        //
        fs::rename(filename, stem + "_1" + ext);
        logFile.open(filename, std::ios_base::app);
    }

    void init(const std::string &fname) {
        filename = fname;
        if (!logFile.is_open()) {
            logFile.open(fname, std::ios_base::app);
        }

        if (!logFile.is_open()) {
            std::cerr << "[TinyLog] Failed to open log file: " << fname
                      << ". Logging to console only.\n";
        }
    }

}