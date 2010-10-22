#include <string.h>
#include <_c.h>

typedef struct
{
    int disksize;
    int fes;
    int tes;
}header;

typedef struct
{
    char filename[60];
    int pointer_inodo;
}fe;

typedef struct
{
    char tag[28];
    int pointer;
}te;

typedef struct
{
    int size_bytes;
    int pDirecto;
    int pIndirecto;
    int pIndirectoDoble;
    int file_entry;
    char *tags[250];
//    char tags_cancion[128];
    int numBloques;
    songinfo tags;
}inodo;

typedef struct
{
    char tag[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[28];
    char genre[3];
}songinfo;

const int bs=1024;
const int fepb=16;
const int tepb=32;

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

void mostrarmapa(unsigned char *m, int cuantos)
{
   int i;
   for (i=0; i< cuantos; i++)
   {
     mostrarbits(m[i]);
   }
   printf("\n ------- fin del mapa --------\n");
}

int cant_apagados(unsigned char *m, int cuantos)
{
    int result=0;
    for(int i=0; i<cuantos; i++)
    {
        int valordelbit = m[i / 8] & (1<<(7-i%8));
        if(!(valordelbit == (1<<(7-i%8))))
        {
            result++;
        }
    }
    return result;
}

int freeblock(unsigned char *m, int cuantos)
{
    for(int i=0; i<cuantos; i++)
    {
        int valordelbit = m[i / 8] & (1<<(7-i%8));
        if(!(valordelbit == (1<<(7-i%8))))
        {
            return i;
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *disco;
    if(strcmp(argv[1],"-c")==0 && argc==6)
    {
        if((disco=fopen(argv[5], "wb"))==NULL)
        {
            exit(1);
        }

        header temp;

        temp.disksize=atoi(argv[2]);
        temp.fes=atoi(argv[3]);
        temp.tes=atoi(argv[4]);        

        unsigned char bloque[bs];
        memset(bloque,0,bs);

        fwrite(bloque,bs,1,disco);

        fseek(disco,0,0);

        fwrite(&temp,sizeof(header),1,disco);

        fseek(disco,bs,0);

        unsigned char *mapabits;
        mapabits = (unsigned char *)malloc(temp.disksize/8);
        memset(mapabits,0,temp.disksize/8);

        int fin=(1+(temp.disksize/8)/bs+temp.fes+temp.tes);
        int i;

        for(i=0; i<fin; i++)
        {
            int casilla = i / 8 ;
            mapabits[casilla] = mapabits[casilla] | (1 << (7-i%8));
        }

        fwrite(mapabits,temp.disksize/8,1,disco);

        fe temp2;

        strcpy(temp2.filename,"");
        temp2.pointer_inodo=-1;

        for(i=0; i<temp.fes*fepb; i++)
        {
            fwrite(&temp2,sizeof(fe),1,disco);
        }

        te temp3;

        strcpy(temp3.tag,"");
        temp3.pointer=-1;

        for(i=0; i<temp.tes*tepb; i++)
        {
            fwrite(&temp3,sizeof(te),1,disco);
        }

        fin=(temp.disksize/bs-(1+(temp.disksize/8)/bs+temp.fes+temp.tes));

        for(i=0; i<fin; i++)
        {
            fwrite(bloque,bs,1,disco);
        }

        fclose(disco);
    }
    else if(strcmp(argv[1],"-a")==0 && argc==4)
    {
        FILE *mp3;

        if((disco=fopen(argv[3], "rb+"))==NULL)
        {
            exit(1);
        }

        if((mp3=fopen(argv[2], "rb"))==NULL)
        {
            exit(1);
        }

        fseek(mp3,0,2);
        int size=ftell(mp3);

        header temp;

        fread(&temp,sizeof(header),1,disco);

        int inicio=(1+(temp.disksize/8)/bs);

        fseek(disco,inicio*bs,0);

        int i;

        for(i=0; i<temp.fes*fepb; i++)
        {
            fe temp2;
            fread(&temp2,sizeof(fe),1,disco);

            if(strcmp(temp2.filename,"")==0)
            {
                strcpy(temp2.filename,argv[2]);

                unsigned char *mapabits;
                mapabits = (unsigned char *)malloc(temp.disksize/8);

                fread(mapabits,temp.disksize/8,1,disco);

                int cant=size/bs;
                int length=(temp.disksize/8)/bs;

                if(cant<=cant_apagados(mapabits,length))
                {
                    int free=freeblock(mapabits,length)+(1+(temp.disksize/8)/bs+temp.fes+temp.tes);

                    fseek(archivo,free*bs,0);


                }
            }
        }
    }
    else if(strcmp(argv[1],"-e")==0 && argc==5)
    {

    }
    else if(strcmp(argv[1],"-q")==0 && argc==4)
    {

    }
    else if(strcmp(argv[1],"-d")==0 && argc==4)
    {

    }
    else if(strcmp(argv[1],"-t")==0 && argc==5)
    {

    }
    else if(strcmp(argv[1],"-it")==0 && argc==4)
    {

    }
    else if(strcmp(argv[1],"-s")==0 && argc==4)
    {

    }
    else if(strcmp(argv[1],"-qi")==0 && argc==4)
    {

    }
    else if(strcmp(argv[1],"-qa")==0 && argc==4)
    {

    }
    else if(strcmp(argv[1],"-l")==0 && argc==3)
    {

    }
    else
    {
        exit(1);
    }

    return 0;
}
