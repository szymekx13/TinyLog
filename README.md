
![TinyLog](https://socialify.git.ci/szymekx13/TinyLog/image?description=1&issues=1&language=1&name=1&theme=Dark)

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B23-orange)
![Build](https://img.shields.io/badge/build-passing-success)

A lightweight, thread-safe and minimalistic logging library for C++.

TinyLog provides a simple interface for logging messages with different severity levels (INFO, WARNING, ERROR) and supports logging to both console and file.

---

## 🚀 Features

- 📜 Simple and intuitive API
- 🧵 Thread-safe logging
- 🖥️ Console and file output
- 🎨 Colored console messages
- 🕒 Automatic timestamps
- ⚙️ **Configuration file support (`config.json`)**
- 🧩 Cross-platform (Windows, Linux, macOS)

---
## 📚 API Reference
### `void LOG::init(const std::string &filename = "log.txt");`
Initializes the logger. If a filename is provided, logs will be written to that file.
### `void log::info(const std::string& fmt, Args&&... args)`
Logs an informational message

### `void LOG::warn(const std::string& fmt, Args&&... args)`
Log a warning message

### `void LOG::error(const std::string& fmt, Args&&... args)`
Log an error message

### `void LOG::trace(const std::string& fmt, T&&... args)`
Log a trace message

### `void LOG::debug(const std::string& fmt, T&&... args)`
Log a debug message

### `void LOG::fatal(const std::string& fmt, T&&... args)`
Log a fatal message and terminate the program

### `void LOG::fatal(const std::string& fmt, T&&... args)`
Log a fatal message and throws a `std::runtime_error` exception
- Do not use `std::exit()` here, exceptions allows the user to handle cleanup
- Use the `LOG_FATAL` macro for automatic inclusion of line number and function name

### `void set_max_backup_files(int count)`
Sets the maximum number of backup log files that will be saved.  
Older files beyond this limit will be automatically deleted.

### `void set_max_file_size(size_t bytes)`
Sets the maximum size of files, after reaching limit new file will be created

### `void rotate_logs()`
Rotates the log files when the main log file exceeds the configured maximum size (`maxFileSize`).  
Older log files are renamed with incremental suffixes (`_1`, `_2`, ...), and the oldest log beyond the limit (`maxBackupFiles`) is deleted.

### `inline void set_console_level(Level lvl)`
Sets the minimum log level for console output.  
Logs with a lower level than `lvl` will be ignored in the console.

### `inline void set_file_level(Level lvl)`
Sets the minimum log level for file output (`log.txt`).  
Logs with a lower level than `lvl` will not be written to the file.

## Example usage of LOG_FATAL macro:
```cpp
try{
    LOG_FATAL("Unable to connect to database: %s", "Connection timed out");
}catch(const std::runtime_error& e){
    std::cerr << e.what() << std::endl;
}
```
> **🛈 Disclaimer**  
> Every `LOG` function is templated as `template<typename... T>`,  
> which means you can use **printf-style formatting** directly:
> ```cpp
> LOG::info("User {} logged in from IP {}", username, ip);
> ```
> (No need for `std::format` or `fmt::format`!)

## 📦 Example usage

```cpp
#include "../include/tinylog.hpp"

int main(){
    log::init("logs.txt"); //Initialize the logger, by deafult log.txt will be
    //created in the working directory, but you can specify a different path and name
    //also genarate config.json file with default settings
    
    log::info("This is an info message");
    log::warning("This is a warning message");
    log::error("This is an error message");
}
```
If no `config.json` file found TinyLog automatically creates one with default settings.

---

## 🧰 Building
```bahs
git clone https://github.com/<your-username>/tinylog.git
cd tinylog
cmake -B build
cmake --build build
```
Example executable will be placed in build/examples/.

---
## 💻 Dependencies
- C++23 compatible editor (but C++11 should also work)
- CMake 3.10 or higher
- No external dependencies

---

## 🧠 TODO

- Add custom log levels
- Add rotating file logs
---

## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
## 💡 Author
Created with ❤️ by [Szymon Niedbała](https://github.com/szymekx13)