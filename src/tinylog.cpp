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
    namespace {
        std::string filename = "log.txt";
        size_t maxFileSize = 5 * 1024 * 1024; // 5 MB
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
        auto size = fs::file_size(filename);
        if (size < maxFileSize) {
            return;
        }
        auto stem = fs::path(filename).stem().string();
        auto ext = fs::path(filename).extension().string();

        //delete oldest log if exist
        auto oldest = stem + "_" + std::to_string(maxBackupFiles) + ext;
        if (fs::exists(oldest)) {
            fs::remove(oldest);
        }
        for (int i = maxBackupFiles - 1; i>=1; --i) {
            auto OldName = stem + "_" + std::to_string(i) + ext;
            auto NewName = stem + "_" + std::to_string(i+1) + ext;
            if (fs::exists(OldName)) {
                fs::rename(OldName, NewName);
            }
        }
        //
        fs::rename(filename, stem + "_1" + ext);
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