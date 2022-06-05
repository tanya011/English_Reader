#pragma once
#include <functional>
#include <chrono>
#include <future>
#include <cstdio>

class CallBackTimer
{
public:
    CallBackTimer()
            :_execute(false)
    {}

    ~CallBackTimer() {
        if( _execute.load(std::memory_order_acquire) ) {
            stop();
        };
    }

    void stop()
    {
        _execute = false;
    }

    void start(int interval, std::function<void(void)> func)
    {
        _execute = true;
        std::thread([this, interval, func]()
                           {
                               while (_execute) {
                                   func();
                                   std::this_thread::sleep_for(
                                           std::chrono::milliseconds(interval));
                               }
                           }).detach();;
    }

private:
    std::atomic<bool> _execute;
};