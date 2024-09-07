#include "timer.h"

Timer::Timer(Timer::ULL time, const function<void ()> &fun_)
{
    complete=0;
    intervals=time;
    Reset();
    fun=fun_;
}

void Timer::Reset()
{
    complete=0;
    time_begin=curTime=QDateTime::currentMSecsSinceEpoch();
}

void Timer::start()
{
    if(complete)return;
    curTime=QDateTime::currentMSecsSinceEpoch();
    if(curTime-time_begin>=intervals){
        complete=1;
        fun();
    }
}

uint Timer::GetRealIntervals()
{
    return curTime-time_begin;
}
