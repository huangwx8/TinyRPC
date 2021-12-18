#include <common/Timer.hh>

#include <unistd.h>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>

#include <common/Defines.hh>

struct TimerTask
{
    time_t Expire;
    std::function<void()> Execution;
    bool Compare(TimerTask& rhs)
    {
        return Expire > rhs.Expire;
    }
};

std::priority_queue<TimerTask, 
    std::vector<TimerTask>, 
    std::function<bool(TimerTask&, TimerTask&)>> timers(&TimerTask::Compare);

/** c-style signal handler function */
static void sig_handler(int sig)
{
    if (sig == SIGALRM)
    {
        Timer::Tick();
    }
}

/** c-style signal control function */
static void addsig(int sig)
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

void Timer::Start()
{
    alarm(TICK_INTERVAL);
    addsig(SIGALRM);
}

void Timer::Tick()
{
    while(!timers.empty())
    {
        auto&& task = timers.top();
        time_t CurrentTime = time(NULL);
        if (CurrentTime > task.Expire)
        {
            task.Execution();
            timers.pop();
        }
        else 
        {
            break;
        }
    }
    alarm(TICK_INTERVAL);
}

void Timer::AddTimer(int sec, std::function<void()> func)
{
    time_t CurrentTime = time(NULL);
    timers.push({CurrentTime, func});
}

void Timer::Stop()
{
    while(!timers.empty())
    {
        timers.pop();
    }
}
