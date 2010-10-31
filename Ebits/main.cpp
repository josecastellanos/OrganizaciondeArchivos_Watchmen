#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libbits.h>

void mostrarmapa(unsigned char *m, int cuantos)
{
   int i;
   for (i=0; i< cuantos; i++)
   {
     mostrarbits(m[i]);
   }
   printf("\n ------- fin del mapa --------\n");
}

int main()
{
   unsigned char *mapabits;
   int cant_bloques;
   int cuantos_bytes;
   printf("Cuantos bloques? ");
   scanf("%d",&cant_bloques);
   cuantos_bytes = cant_bloques / 8;
   printf("Se necesitan %d bytes \n",cuantos_bytes);
   mapabits = (unsigned char *)malloc(cuantos_bytes);
   // Setear todos los bits en cero
   memset(mapabits,0,cuantos_bytes);
   mostrarmapa(mapabits,cuantos_bytes);
   char resp='s';
   int cual, valor_bit;
   do
   {
     printf("Cual bloque quiere setear?");
     scanf("%d", &cual);
     printf("Que valor quiere setear (1/0) ");
     scanf("%d", &valor_bit);
     // Calcular que casilla del arreglo esta
     int casilla = cual / 8 ; // ya que cada casilla es de 8 bits
     // una vez ubicada la casilla, setear el bit
     if (valor_bit==1)
       mapabits[casilla] = mapabits[casilla] | (1 << (7-cual%8));
     else
       mapabits[casilla] = mapabits[casilla] & ~(1 << (7-cual%8));

    mostrarmapa(mapabits,cuantos_bytes);
    printf("Probar otra vez\n" );
    getchar();
    scanf("%c", &resp);
   } while (resp!='n');

   printf("Finalmente, indique que bloque quisiera saber el valor del bit ");
   scanf("%d",&cual);
   int valordelbit = mapabits[cual / 8] & (1<<(7-cual%8));
   if(valordelbit == (1<<(7-cual%8))) // Si el valor es igual a la mascara
          valordelbit =1;
   else
          valordelbit =0;
   printf("El valor del bit es : %d ", valordelbit);
  return 0;
}
