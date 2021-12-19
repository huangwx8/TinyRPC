#pragma once

#include <functional>

class Timer
{
public:
    /** 
     * Start clock
     */
    static void Start();
    /** 
     * Receive alarm signal, execute all tasks expired
     */
    static void Tick();
    /** 
     * Assign a new task executed in the future 
     */
    static void AddTimer(int sec, std::function<void()> func);
    /** 
     * Stop clock, remove all tasks
     */
    static void Stop();
};


// Timer's RAII Wrapper
class GlobalTimer
{
public:
    GlobalTimer()
    {
        if (ConstructCounter == 0)
        {
            Timer::Start();
            ConstructCounter++;
        }
    }
    ~GlobalTimer()
    {
        if (ConstructCounter == 1)
        {
            Timer::Stop();
            ConstructCounter--;
        }
    }
    static int ConstructCounter;
};
