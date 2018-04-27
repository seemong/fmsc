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
    void set_thread_priority(thread& t, int priority);
};

#endif // STHREAD_H
