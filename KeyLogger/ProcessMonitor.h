#pragma once
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <functional>
#include <psapi.h>
/// <summary>
/// 
/// </summary>
class ProcessMonitor {
    /// <summary>
    // To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
    // and compile with -DPSAPI_VERSION=1
    /// </summary>
    private: static std::string GetProcessName(DWORD processID);
    /// <summary>
    /// 
    /// </summary>
    private: static std::string GetActiveWindowTitle();
    /// <summary>
    /// 
    /// </summary>
    public: static std::string OnWindowChange(std::string activeWindow, const std::function<void(std::string)>& callback);
};