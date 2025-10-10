# 🪶 TinyLog

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B23-orange)
![Build](https://img.shields.io/badge/build-passing-success)

A lightweight, thread-safe and minimalistic logging library for C++.

TinyLog provides a simple interface for logging messages with different severity levels (INFO, WARNING, ERROR) and supports logging to both console and file.

---

## 🚀 Features

- 📜 Simple and clean API
- 🧵 Thread-safe logging
- 🖥️ Console and file logging
- 🎨 Colored console output
- 🕒 Automatic timestamping

---

## 📦 Example usage

````cpp
#include "tinylog.hpp"

int main(){
    log::init("logs.txt"); //Initialize the logger, by deafult log.txt will be
    //created in the working directory, but you can specify a different path and name
    
    log::info("This is an info message");
    log::warning("This is a warning message");
    log::error("This is an error message");
}
````

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
## 📄 License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---
## 💻 Dependencis
- C++23 compatible editor (but C++11 should also work)
- CMake 3.10 or higher
- A C++ compiler that supports C++23 (like GCC, Clang, MSVC)
- No external dependencies
- Operating System: Cross-platform (Windows, Linux, macOS)
---
## 🧠 TODO

- Add custom log levels
- Add configuration file support
- Add rotating file logs

---
## 📚 API Reference
### `void LOG::init(const std::string &filename = "log.txt");`
Initializes the logger. If a filename is provided, logs will be written to that file.
### `template<typename... Args> void log::info(const std::string& fmt, Args&&... args)`
Logs an informational message


### `template<typename... Args> void log::warn(const std::string& fmt, Args&&... args)`
Log a warning message

### `template<typename... Args> void log::error(const std::string& fmt, Args&&... args)`
Log an error message

### `    template <typename... T> void trace(const std::string& fmt, T&&... args)`
Log a trace message

### `    template <typename... T> void debug(const std::string& fmt, T&&... args)`
Log a debug message

### `    template <typename... T> void fatal(const std::string& fmt, T&&... args)`
Log a fatal message and terminate the program

### `template<typename... T> void LOG::fatal(const std::string& fmt, T&&... args)`
Log a fatal message and throws a `std::runtime_error` exception
- Do not use `std::exit()` here, exceptions allows the user to handle cleanup
- Use the `LOG_FATAL` macro for automatic inclusion of line number and function name
Example:
```cpp
try{
    LOG_FATAL("Unable to connect to database: %s", "Connection timed out");
}catch(const std::runtime_error& e){
    std::cerr << e.what() << std::endl;
}
```
---
## 💡 Author
Created with ❤️ by [Szymon Niedbała](https://github.com/szymekx13)