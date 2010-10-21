#include "libbits.h"

void mostrarbits(unsigned char byte)
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
