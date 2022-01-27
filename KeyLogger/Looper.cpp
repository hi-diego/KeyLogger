#include "Looper.h"
#include <Windows.h>
#include <functional>
/// <summary>
/// Infinite while loop.
/// </summary>
//class Looper {
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
//};