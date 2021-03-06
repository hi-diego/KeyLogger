#include "Looper.h"
#include <Windows.h>
#include <functional>
#include <thread>
/// <summary>
/// 
/// </summary>
/// <param name="sleep"></param>
/// <param name="keepDoing"></param>
/// <param name="callback"></param>
void Looper::DoWhile(int sleep, bool keepDoing, const std::function<void()>& callback)
{
    while (keepDoing) {
        Sleep(sleep);
        callback();
    }
}
/// <summary>
/// 
/// </summary>
/// <param name="sleep"></param>
/// <param name="keepDoing"></param>
/// <param name="callback"></param>
std::thread Looper::Thread(int sleep, bool keepDoing, const std::function<void()>& callback)
{
    std::thread thread(Looper::DoWhile, sleep, keepDoing, callback);
    return thread;
}