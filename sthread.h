/**
 * sthreaad.h --
 * 
 * Customizes STL threads for Posix priorities
 */

#ifndef STHREAD_H
#define STHREAD_H

#include <thread>
using namespace std;

namespace Sthread {
    /**
     * Set the thread priority using POSIX threads
     */
    void set_thread_priority(thread& t, int priority);
    
    /**
     * Sleep for the given number of milliseconds
     */
    void sleep_millis(int millis);
};

#endif // STHREAD_H
