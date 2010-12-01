#ifndef MAPABITS_H
#define MAPABITS_H

#include <stdio.h>
#include <stdlib.h>

class mapabits
{
public:
    mapabits();
    void mostrarbits(unsigned char byte);
    void mostrarmapa(unsigned char *m, int cuantos);
    int cant_apagados(unsigned char *m, int cuantos);
    int freeblock(unsigned char *m, int cuantos);
    void killblock(unsigned char *m, int cual, int cuantos);

};

#endif // MAPABITS_H
