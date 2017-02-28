#ifndef TIMERS_H

#define TIMERS_H

#include <ctime>

#define TIMER_T unsigned long long int
#define BEGIN_TIMER TIMER_T startTimer = clock();
#define END_TIMER TIMER_T endTimer = clock(); TIMER_T resTimer = endTimer - startTimer;

#define RES_TIMER resTimer

#endif
