#pragma once
// std
#include <filesystem>
#include <sstream>

// lib
#include <proc/process>
#include "win32_console.hpp"

namespace fs = std::filesystem;

namespace winapi {

class debugger {
  public:
    // static bool launch() {
    //	console->write_line("Launching debugger...");
    //	fs::path jit("C:\\Windows\\System32\\vsjitdebugger.exe");

    //	DWORD pid = GetCurrentProcessId();
    //	std::ostringstream s;
    //	s << "-p " << pid;
    //	std::string cmd_line = s.str();

    //	proc::process p = proc::process::launch(jit, cmd_line);
    //	if (!p.is_running()) return false;
    //	p.close_handles();

    //	console->write_line("Waiting for debugger to attach...");
    //	while (!IsDebuggerPresent()) Sleep(100);

    //	console->write_line("Debugger attached. Breaking...");
    //	return try_break();
    //}

    static bool launch() {
        console->write_line("Launching debugger...");
        fs::path jit("C:\\Windows\\System32\\vsjitdebugger.exe");

        DWORD pid = GetCurrentProcessId();
        std::wostringstream s;
        s << jit << " -p " << pid;
        std::wstring cmd_line = s.str();

        STARTUPINFOW si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);

        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessW(NULL, &cmd_line[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
            return false;

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

        console->write_line("Waiting for debugger to attach...");
        while (!IsDebuggerPresent())
            Sleep(100);

        console->write_line("Debugger attached. Breaking...");
        return try_break();
    }

    static bool break_here() { return try_break(); }

  private:
    static bool try_break() noexcept {
        __try {
            DebugBreak();
            return true;
        } __except (GetExceptionCode() == EXCEPTION_BREAKPOINT ? EXCEPTION_EXECUTE_HANDLER
                                                               : EXCEPTION_CONTINUE_SEARCH) {
            console->write_line("Error attaching debugger...");
            return false;
        }
    }
};

} // namespace winapi