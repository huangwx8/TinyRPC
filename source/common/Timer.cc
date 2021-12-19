#include <common/Timer.hh>

#include <unistd.h>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>

#include <pthread.h>
#include <errno.h>

#include <common/Defines.hh>
#include <common/Logger.hh>

int GlobalTimer::ConstructCounter = 0;

struct TimerTask
{
    time_t Expire;
    std::function<void()> Execution;
    bool Compare(TimerTask& rhs)
    {
        return Expire > rhs.Expire;
    }
};

// 调度定时器任务的数据结构
std::priority_queue<TimerTask, 
    std::vector<TimerTask>, 
    std::function<bool(TimerTask&, TimerTask&)>> timers(&TimerTask::Compare);
// Linux信号集
sigset_t set;
// 定时器线程
pthread_t timer_thread;


/** 
 * c-style dedicated signal process thread
 * refer to "pthread_sigmask man"
 */
static void* sig_thread(void* args)
{
    int s, sig;
    while (1) 
    {
        s = sigwait(&set, &sig);
        assert(s == 0);
        if (sig == SIGALRM)
        {
            Timer::Tick();
        }
    }
}

void Timer::Start()
{
    // Linux信号对多线程的支持不是很人道，一般会选择开辟一个新的线程专门处理linux信号
    
    // 信号集增加 SIGALRM 信号
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    // 所有线程屏蔽 SIGALRM
    assert(pthread_sigmask(SIG_BLOCK, &set, NULL) == 0);
    // 用一个线程使用sigwait调用接受信号
    assert(pthread_create(&timer_thread, NULL, &sig_thread, NULL) == 0);
    // TICK_INTERVAL 秒后，产生一个SIGALRM
    alarm(TICK_INTERVAL);
}

void Timer::Tick()
{
    while(!timers.empty())
    {
        auto&& task = timers.top();
        time_t CurrentTime = time(NULL);
        if (CurrentTime >= task.Expire)
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
    timers.push({CurrentTime + sec, func});
}

void Timer::Stop()
{
    // 注销SIGALRM
    alarm(0);
    // 注销所有定时器任务
    while(!timers.empty())
    {
        timers.pop();
    }
    // 撤销定时器线程
    assert(pthread_cancel(timer_thread) == 0);
}
