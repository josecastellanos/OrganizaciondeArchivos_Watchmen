#ifndef TIMER_H
#define TIMER_H

#include <ctime>

using namespace std;

class timer
{
public:
    timer();
    clock_t inicio, final;
    void iniciar();
    void detener();
    double getMS();

};

#endif // TIMER_H
