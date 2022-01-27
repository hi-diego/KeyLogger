#pragma once
#include <functional>
#include <thread>
/// <summary>
/// Infinite while loop.
/// </summary>
class Looper {
    /// <summary>
    /// 
    /// </summary>
    public: static void DoWhile(int sleep, bool keepDoing, const std::function<void()>& callback);
    /// <summary>
    /// 
    /// </summary>
    public: static std::thread Thread(int sleep, bool keepDoing, const std::function<void()>& callback);
};