#pragma once
// std
#include <Windows.h>
#include <iostream>
#include <string>

namespace winapi {

std::string get_last_error() {
    DWORD error_code = ::GetLastError();
    if (error_code == 0)
        return std::string();

    LPSTR message_buffer = nullptr;
    size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                    FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                (LPSTR)&message_buffer, 0, NULL);

    std::string msg(message_buffer, size);
    LocalFree(message_buffer);
    return msg;
}

std::wstring string_to_wstring(const std::string& str) {
    if (str.empty())
        return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

std::string wstring_to_string(const std::wstring& wstr) {
    if (wstr.empty())
        return std::string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0,
                                          nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &str[0], size_needed, nullptr,
                        nullptr);
    return str;
}

} // namespace winapi