#include <string.h>
#include "_c.h"
#include <math.h>

typedef struct
{
    unsigned int disksize;
    unsigned int fes;
    unsigned int tes;
}header;

typedef struct
{
    char filename[60];//Preguntar lo de unsigned
    unsigned int pointer_inodo;
}fe;

typedef struct
{
    char tag[28];
    unsigned int pointer;
}te;

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

typedef struct
{
    unsigned int size_bytes;
    unsigned int pDirecto[12];
    unsigned int pIndirecto;
    unsigned int pIndirectoDoble;
    unsigned int file_entry;
    unsigned int numBloques;
    songinfo tags_cancion;
    char tags[250];
}inodo;

const int bs=1024;
const int fepb=16;
const int tepb=32;
FILE *disco;

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

int freeblock(unsigned char *m, int cuantos)
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

void killblock(unsigned char *m, int cual, int cuantos)
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

int query(char *cancion, char *diskname)
{
    if((disco=fopen(diskname, "wb"))==NULL)
    {
        exit(1);
    }

    header temp;

    fread(&temp,sizeof(header),1,disco);

    int pos=(1+(((int)temp.disksize/8)/bs))*bs;

    fseek(disco,pos,0);

    int i;

    fe temp2;

    for(i=0; i<(int)temp.fes*fepb; i++)
    {
        fread(&temp2,sizeof(fe),1,disco);

        if(strcmp(temp2.filename,cancion)==0)
        {
            break;
        }
    }

    return temp2.pointer_inodo;

    fclose(disco);
}

void queryinodo(int pos, char *diskname)
{
    if((disco=fopen(diskname, "wb"))==NULL)
    {
        exit(1);
    }

    inodo n;

    fseek(disco,pos,0);

    fread(&n,sizeof(inodo),1,disco);

    fe temp;

    fseek(disco,n.file_entry,0);

    fread(&temp,sizeof(fe),1,disco);

    printf("QUERYINODO:");
    printf("Nombre del archivo: %s#",temp.filename);

    printf("Titulo: %s#",n.tags_cancion.title);
    printf("Artista: %s#",n.tags_cancion.artist);
    printf("Album: %s#",n.tags_cancion.album);
    printf("Año: %s#",n.tags_cancion.year);
    printf("Comentario: %s#",n.tags_cancion.comment);
    printf("Genero: %s#",n.tags_cancion.genre);

    printf("Size del archivo: %d#",n.size_bytes);

    //printf(": %s",temp.filename);//Utilizar -it

    printf("Numero de bloques: %d",n.numBloques);

    fclose(disco);
}

int main(int argc, char *argv[])
{
    if(strcmp(argv[1],"-c")==0 && argc==6)
    {
        if((disco=fopen(argv[5], "wb"))==NULL)
        {
            exit(1);
        }

        header temp;

        int dsize=atoi(argv[2]);

        if(dsize%8!=0)
        {
            dsize+=dsize%8;
        }

        temp.disksize=dsize;
        temp.fes=atoi(argv[3]);
        temp.tes=atoi(argv[4]);        

        unsigned char bloque[bs];
        memset(bloque,0,bs);

        fwrite(bloque,bs,1,disco);

        fseek(disco,0,0);

        fwrite(&temp,sizeof(header),1,disco);

        fseek(disco,bs,0);

        unsigned char *mapabits;
        mapabits = (unsigned char *)malloc((int)temp.disksize/8);
        memset(mapabits,0,(int)temp.disksize/8);

        int fin=(1+(((int)temp.disksize/8)/bs)+(int)temp.fes+(int)temp.tes);
        int i;

        for(i=0; i<fin; i++)
        {
            int casilla = i / 8 ;
            mapabits[casilla] = mapabits[casilla] | (1 << (7-i%8));
        }

        fwrite(mapabits,((int)temp.disksize/8),1,disco);

        fe temp2;

        strcpy(temp2.filename,"");
        temp2.pointer_inodo=-1;

        for(i=0; i<(int)temp.fes*fepb; i++)
        {
            fwrite(&temp2,sizeof(fe),1,disco);
        }

        te temp3;

        strcpy(temp3.tag,"");
        temp3.pointer=-1;

        for(i=0; i<(int)temp.tes*tepb; i++)
        {
            fwrite(&temp3,sizeof(te),1,disco);
        }

        fin=((int)temp.disksize-(1+(int)(temp.disksize/8)/bs+(int)temp.fes+(int)temp.tes));

        for(i=0; i<fin; i++)
        {
            fwrite(bloque,bs,1,disco);
        }

        fclose(disco);

        free(mapabits);

        printf("CREAR:%s se ha creado correctamente",argv[5]);
    }
    else if(strcmp(argv[1],"-a")==0 && argc==4)
    {
        FILE *mp3;

        int cont=0;

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

        if(size>=68157440)
        {
            exit(1);
            printf("ADD:El archivo %s excede el size establecido",argv[2]);
        }

        header temp;

        fread(&temp,sizeof(header),1,disco);

        fseek(disco,bs,0);

        unsigned char *mapabits;
        mapabits = (unsigned char *)malloc(((int)temp.disksize/8));

        fread(mapabits,((int)temp.disksize/8),1,disco);

        int cant_data=size/bs;
        if(size%bs!=0)
        {
            cant_data++;
        }

        int cant_extra=1;
        if(cant_data>12)
        {
            cant_extra++;
            if(cant_data>268)
            {
                int data=1+((cant_data-268)/256);
                if((cant_data-268)%256!=0)
                {
                    data++;
                }
                cant_extra+=data;
            }
        }

        int length=(int)temp.disksize;

        int i;

        for(i=0; i<(int)temp.fes*fepb; i++)
        {
            fe temp2;

            int fepos=ftell(disco);

            fread(&temp2,sizeof(fe),1,disco);

            if(strcmp(temp2.filename,"")==0)
            {
                if((cant_data+cant_extra)<=cant_apagados(mapabits,length))
                {
                    int free=freeblock(mapabits,length);

                    strcpy(temp2.filename,argv[2]);
                    temp2.pointer_inodo=free;

                    killblock(mapabits,free,length);

                    inodo n;

                    n.file_entry=fepos;
                    n.numBloques=cant_data;
                    n.size_bytes=size;
                    strcpy(n.tags,"");

                    fseek(mp3,size-128,0);

                    fread(&n.tags_cancion, sizeof(songinfo), 1, mp3);

                    fseek(mp3,0,0);

                    int j;

                    for(j=0; j<12; j++)
                    {
                        if(cont!=cant_data)
                        {
                            int a=freeblock(mapabits,length);
                            killblock(mapabits,a,length);

                            n.pDirecto[j]=a;

                            fseek(disco,a*bs,0);

                            unsigned char bloque[bs];

                            fread(bloque,bs,1,mp3);
                            fwrite(bloque,bs,1,disco);
                            cont++;
                        }
                        else
                        {
                            break;
                        }
                    }

                    if(cant_data>12)//Primer indirecto simple
                    {
                        int c=freeblock(mapabits,length);
                        killblock(mapabits,c,length);

                        n.pIndirecto=c;

                        int b[256];

                        for(j=0; j<256; j++)
                        {
                            if(cont!=cant_data)
                            {
                                int a=freeblock(mapabits,length);
                                killblock(mapabits,a,length);

                                b[j]=a;

                                fseek(disco,a*bs,0);

                                unsigned char bloque[bs];

                                fread(bloque,bs,1,mp3);
                                fwrite(bloque,bs,1,disco);
                                cont++;
                            }
                            else
                            {
                                break;
                            }
                        }

                        fseek(disco,c*bs,0);

                        fwrite(b,bs,1,disco);
                    }

                    if(cant_data>268)//Indirectos dobles
                    {
                        int c=freeblock(mapabits,length);
                        killblock(mapabits,c,length);

                        n.pIndirectoDoble=c;

                        int b[256];

                        int k;

                        for(j=0; j<256; j++)
                        {
                            if(cont==cant_data)
                            {
                                break;
                            }

                            int d=freeblock(mapabits,length);
                            killblock(mapabits,d,length);

                            b[j]=d;

                            int e[256];

                            for(k=0; k<256; k++)
                            {
                                if(cont!=cant_data)
                                {
                                    int a=freeblock(mapabits,length);
                                    killblock(mapabits,a,length);

                                    e[k]=a;

                                    fseek(disco,a*bs,0);

                                    unsigned char bloque[bs];

                                    fread(bloque,bs,1,mp3);
                                    fwrite(bloque,bs,1,disco);
                                    cont++;
                                }
                                else
                                {
                                    break;
                                }
                            }

                            fseek(disco,d*bs,0);

                            fwrite(e,bs,1,disco);
                        }

                        fseek(disco,c*bs,0);

                        fwrite(b,bs,1,disco);
                    }

                    fseek(disco,free*bs,0);

                    fwrite(&n,sizeof(inodo),1,disco);

                    fseek(disco,fepos,0);

                    fwrite(&temp2,sizeof(fe),1,disco);

                    fseek(disco,bs,0);

                    fwrite(mapabits,((int)temp.disksize/8),1,disco);

                    break;
                }
                else
                {
                    exit(1);
                }
            }
        }

        fclose(disco);
        fclose(mp3);

        free(mapabits);

        printf("ADD:El archivo %s se ha agregado correctamente",argv[2]);
    }
    else if(strcmp(argv[1],"-e")==0 && argc==5)
    {
        FILE *mp3;

        int cont=0;

        if((disco=fopen(argv[4], "rb+"))==NULL)
        {
            exit(1);
        }

        if((mp3=fopen(argv[3], "wb"))==NULL)
        {
            exit(1);
        }

        header temp;

        fread(&temp,sizeof(header),1,disco);

        fseek(disco,(1+(((int)temp.disksize/8)/bs))*bs,0);

        int i;

        for(i=0; i<(int)temp.fes*fepb; i++)
        {
            fe temp2;

            fread(&temp2,sizeof(fe),1,disco);

            if(strcmp(temp2.filename,argv[2])==0)
            {
                fseek(disco,temp2.pointer_inodo*bs,0);

                inodo n;

                fread(&n,sizeof(inodo),1,disco);

                int fin;

                    if(n.numBloques>=12)
                    {
                        fin=12;
                    }
                    else
                    {
                        fin=n.numBloques;
                    }

                    int j;

                    for(j=0; j<fin; j++)
                    {                        
                        fseek(disco,n.pDirecto[j]*bs,0);

                        unsigned char bloque[bs];

                        if(cont!=((int)n.numBloques-1))
                        {
                            fread(bloque,bs,1,disco);
                            fwrite(bloque,bs,1,mp3);
                            cont++;
                        }
                        else
                        {
                            int x=n.size_bytes%bs;

                            fread(bloque,bs,1,disco);

                            unsigned char *bloque2=(unsigned char *)malloc(x);
                            memcpy(bloque2,bloque,x);

                            fwrite(bloque2,x,1,mp3);
                            free(bloque2);
                            break;
                        }
                    }

                    if(n.numBloques>=256)
                    {
                        fin=256;
                    }
                    else
                    {
                        fin=(n.numBloques-12);
                    }

                    if(fin!=0)
                    {
                        fseek(disco,n.pIndirecto*bs,0);

                        int b[256];

                        fread(b,bs,1,disco);

                        for(j=0; j<fin; j++)
                        {                            
                            fseek(disco,b[j]*bs,0);

                            unsigned char bloque[bs];

                            if(cont!=((int)n.numBloques-1))
                            {
                                fread(bloque,bs,1,disco);
                                fwrite(bloque,bs,1,mp3);
                                cont++;
                            }
                            else
                            {
                                int x=n.size_bytes%bs;

                                fread(bloque,bs,1,disco);

                                unsigned char *bloque2=(unsigned char *)malloc(x);
                                memcpy(bloque2,bloque,x);

                                fwrite(bloque2,x,1,mp3);
                                free(bloque2);
                                break;
                            }
                        }
                    }

                    if((int)n.numBloques>268)
                    {
                        int fin_j=((n.numBloques-268)/256);
                        if((n.numBloques-268)%256!=0)
                        {
                            fin_j++;
                        }

                        int fin_k=256;

                        fseek(disco,n.pIndirectoDoble*bs,0);

                        int b[256];

                        fread(b,bs,1,disco);

                        int k;

                        for(j=0; j<fin_j; j++)
                        {
                            if(j==(fin_j-1))
                            {
                                fin_k=(n.numBloques-268)%256;
                            }

                            fseek(disco,b[j]*bs,0);

                            int e[256];

                            fread(e,bs,1,disco);

                            for(k=0; k<fin_k; k++)
                            {
                                fseek(disco,e[k]*bs,0);

                                unsigned char bloque[bs];

                                if(cont!=(int)(n.numBloques-1))
                                {
                                    fread(bloque,bs,1,disco);
                                    fwrite(bloque,bs,1,mp3);
                                    cont++;
                                }
                                else
                                {
                                    int x=n.size_bytes%bs;

                                    fread(bloque,bs,1,disco);

                                    unsigned char *bloque2=(unsigned char *)malloc(x);
                                    memcpy(bloque2,bloque,x);

                                    fwrite(bloque2,x,1,mp3);
                                    free(bloque2);
                                    break;
                                }                                
                            }
                        }
                    }
                    break;
                }

        }

        fclose(disco);
        fclose(mp3);

        printf("EXPORTAR:%s se ha exportado correctamente",argv[2]);
    }
    else if(strcmp(argv[1],"-q")==0 && argc==4)
    {
        printf("QUERY:El numero de inodo de %s es %d",argv[2],query(argv[2],argv[3]));
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
        queryinodo(atoi(argv[2]),argv[3]);
    }
    else if(strcmp(argv[1],"-qa")==0 && argc==4)
    {
        if((disco=fopen(argv[3], "wb"))==NULL)
        {
            exit(1);
        }

        header temp;

        fread(&temp,sizeof(header),1,disco);

        int pos=(1+(((int)temp.disksize/8)/bs))*bs;

        fseek(disco,pos,0);

        int pointer_inodo=query(argv[2],argv[3]);

        printf("QUERYFILE:$");

        queryinodo(pointer_inodo,argv[3]);
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
