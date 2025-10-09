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
## 🧠 TODO

- Add custom log levels
- Add configuration file support
- Add rotating file logs

---
## 💡 Author
Created with ❤️ by [Szymon Niedbała](https://github.com/szymekx13)