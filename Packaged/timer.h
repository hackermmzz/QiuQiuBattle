#ifndef TIMERE_H
#define TIMERE_H
#include"QDateTime"
#include"functional"
using namespace std;
struct Timer
{
    using ULL=unsigned long long;
    ULL time_begin;
    ULL curTime;
    ULL intervals;
    bool complete;
    function<void(void)> fun;
    ////////////////////
    Timer(ULL time,const function<void(void)>&fun);
    void Reset();
    void start();
    uint GetRealIntervals();
};

#endif // TIMERE_H
