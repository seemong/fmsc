/**
 * sthread.cpp --
 * 
 * Implements POSIX thread class that allows the setting of priority
 */

#include <cassert>
#include <pthread.h>
#include <thread>
#include <chrono>
#include "sthread.h"
using namespace std;

namespace Sthread {
    
void
set_thread_priority(thread& t, int priority) {
    sched_param sch_params;
    sch_params.sched_priority = priority;
    if(pthread_setschedparam(t.native_handle(), SCHED_RR, &sch_params)) {
        assert(false);
    }
}

void
sleep_millis(int millis) {
    this_thread::sleep_for(chrono::milliseconds(millis));
}

};
