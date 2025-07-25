#pragma once
// std
#include <Windows.h>
#include <iostream>
#include <vector>

// lib
#include "utils.h"

#define ConFile(std)                                                                               \
    CreateFile(std, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,        \
               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)

namespace winapi {

class _console {
  public:
    static inline void write(std::string msg) {
        if (instance_ == nullptr) {
            instance_ = new _console();
        }

        std::cout << msg;
    }

    static inline void write_line(std::string msg) { std::cout << msg << std::endl; }

    template <typename... T>
    static inline void write(T&... args) {
        ((std::cout << args), ...);
    }

    template <typename... T>
    void write_line(T&... args) {
        ((std::cout << args), ...);
        std::cout << std::endl;
    }

    std::string read_line() {
        std::string res;
        std::cin >> res;
        return res;
    }

    bool read_line(std::string* out) {
        std::cin >> *out;
        return !out->empty();
    }

    static inline void error(std::string msg);
    static inline void error_line(std::string msg);

    template <typename... T>
    void error(T&... args) {
        ((std::cerr << args), ...);
    }

    template <typename... T>
    void error_line(T&... args) {
        ((std::cerr << args), ...);
        std::cerr << std::endl;
    }

    static _console* get_instance() {
        if (instance_ == nullptr) {
            instance_ = new _console();
        }

        return instance_;
    }

    static bool detach() {
        if (instance_ != nullptr) {
            if (instance_->destroy_console()) {
                delete instance_;
                return true;
            }
            return false;
        }
        return true;
    }

  private:
    static inline bool initialized_                                       = false;
    static inline _console* instance_                                     = nullptr;

    static inline std::vector<std::reference_wrapper<std::ios>> channels_ = {std::cout, std::clog,
                                                                             std::cerr, std::cin};

    _console() {
        if (!create_console()) {
            std::string err = get_last_error();
            throw std::runtime_error(err);
        }
    }

    ~_console() {
        if (!destroy_console()) {
            std::string err = get_last_error();
            std::cerr << "Error destroying console: " << err << std::endl;
        }
    }

    bool create_console() {
        if (GetConsoleWindow())
            return true;
        if (!AllocConsole())
            return false;

        try {
            FILE* con;
            freopen_s(&con, "CONOUT$", "w", stdout);
            freopen_s(&con, "CONOUT$", "w", stderr);
            freopen_s(&con, "CONIN$", "r", stdin);

            for (auto& s : channels_) {
                s.get().clear();
            }

            HANDLE hConOut = ConFile("CONOUT$");
            HANDLE hConIn  = ConFile("CONIN$");

            SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
            SetStdHandle(STD_ERROR_HANDLE, hConOut);
            SetStdHandle(STD_INPUT_HANDLE, hConIn);
            return true;
        } catch (const std::exception& ex) {
            return false;
        }
    }

    bool destroy_console() {
        if (!FreeConsole()) {
            // std::string err = get_last_error();
            return false;
        }
        return true;
    }
} static* console = _console::get_instance();

} // namespace winapi