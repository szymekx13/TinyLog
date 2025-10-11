/*
* Copyright (c) 2025 Szymon Niedbała
 *
 * Licensed under the MIT License.
 * You may obtain a copy of the License at https://opensource.org/licenses/MIT
 */
#include "../include/tinylog.hpp"
#include <fstream>
#include <iostream>

int main() {
    LOG::init();
    std::ofstream file("file", std::ios::app);
    if (!file.is_open()) {
        LOG::error("Failed to open file: %s", "file.txt");
        return 1;
    }

    LOG::info("Server started on port %d", 8080); // or with macro LOG_INFO("Server started on port %d", 8080);
    LOG::warn("Disk usage at %d%%", 85); // or with macro LOG_WARN("Disk usage at %d%%", 85);
    LOG::debug("Mouse position: x=%d, y=%d", 150, 300); // or with macro LOG_DEBUG("Mouse position: x=%d, y=%d", 150, 300);
    LOG::trace("Loading module: %s", "graphics.dll"); // or with macro LOG_TRACE("Loading module: %s", "graphics.dll");
    LOG_ERROR("Failed to load module"); // using macro

    for (int i = 0; i <= 100; i++) {
        LOG_INFO("Test log %x", i);
    }

    LOG::rotate_logs();

    try {
        // simulate a fatal error
        LOG_FATAL("Unable to connect to database: %s", "Connection timed out");
    }catch (const std::runtime_error& e) {
        std::cerr<< e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}