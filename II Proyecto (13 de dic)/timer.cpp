#include "timer.h"

timer::timer()
{
}

void timer::iniciar()
{
    inicio = clock();
}

void timer::detener()
{
    final = clock();
}

double timer::getMS()
{
    return (((clock() - inicio)*1000)/CLOCKS_PER_SEC )/1000 ;
}
