#pragma once
#include <functional>
/// <summary>
/// Infinite while loop.
/// </summary>
class Looper {
    /// <summary>
    /// 
    /// </summary>
    public: static void DoWhile(int sleep, bool keepDoing, const std::function<void()>& callback);
};