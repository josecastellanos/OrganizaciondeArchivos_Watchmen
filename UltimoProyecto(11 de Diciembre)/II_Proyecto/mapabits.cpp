#include "mapabits.h"

mapabits::mapabits()
{
}

void mapabits::mostrarbits(unsigned char byte)
{
   unsigned char MASK = 1 << 7; // o = 128
   unsigned int i;
   for ( i = 1; i <= 8; i++ )
    {
       printf("%c", byte & MASK ? '1' : '0' );
       byte <<= 1;
    }
   printf("-");
}

void mapabits::mostrarmapa(unsigned char *m, int cuantos)
{
   int i;
   for (i=0; i< cuantos; i++)
   {
     mostrarbits(m[i]);
   }
   printf("\n ------- fin del mapa --------\n");
}

int mapabits::cant_apagados(unsigned char *m, int cuantos)
{
    int i;
    int result=0;

    for(i=0; i<cuantos; i++)
    {
        int valordelbit = m[i / 8] & (1<<(7-i%8));
        if(!(valordelbit == (1<<(7-i%8))))
        {
            result++;
        }
    }
    return result;
}

int mapabits::freeblock(unsigned char *m, int cuantos)
{
    int i;

    for(i=0; i<cuantos; i++)
    {
        int valordelbit = m[i / 8] & (1<<(7-i%8));
        if(valordelbit != (1<<(7-i%8)))
        {
            return i;
        }
    }
}

void mapabits::killblock(unsigned char *m, int cual, int cuantos)
{
    int i;

    for(i=0; i<cuantos; i++)
    {
        if(i==cual)
        {
            int casilla = cual / 8 ;
            m[casilla] = m[casilla] | (1 << (7-cual%8));
        }
    }
}
