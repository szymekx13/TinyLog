/*
* Copyright (c) 2025 Szymon Niedbała
 *
 * Licensed under the MIT License.
 * You may obtain a copy of the License at https://opensource.org/licenses/MIT
 */
#include "../include/tinylog.hpp"

int main() {
    log::init();

    log::info("Server started on port %d", 8080);
    log::warn("Disk usage at %d%%", 85);
    log::error("Failed to open file: %s", "config.txt");
    return 0;
}