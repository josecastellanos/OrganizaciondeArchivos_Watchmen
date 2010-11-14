#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

int query(char *cancion, FILE *disco)
{
    fseek(disco,0,0);

    header temp;

    fread(&temp,sizeof(header),1,disco);

    int pos=(1+(((int)temp.disksize/8)/bs))*bs;

    fseek(disco,pos,0);

    int i;
    int ver=0;

    fe temp2;

    for(i=0; i<(int)temp.fes*fepb; i++)
    {
        fread(&temp2,sizeof(fe),1,disco);

        if(strcmp(temp2.filename,cancion)==0)
        {
            ver=1;
            break;
        }
    }

    if(ver==0)
    {
        return -1;
    }

    return temp2.pointer_inodo;
}

int existeCancion(char *cancion, FILE *disco)
{
    fseek(disco,0,0);

    header temp;

    fread(&temp,sizeof(header),1,disco);

    int pos=(1+((int)temp.disksize/8)/bs)*bs;

    fseek(disco,pos,0);

    int i;

    fe temp2;

    for(i=0; i<(int)temp.fes*fepb; i++)
    {
        fread(&temp2,sizeof(fe),1,disco);

        if(strcmp(temp2.filename,cancion)==0)
        {
            return i;
        }
    }

    return -1;
}

void verTags(char *cancion,FILE *disco)
{
    int pos=existeCancion(cancion,disco);

    if(pos==-1)
    {
        printf("TAGSCANCION:No existe la cancion %s en el disco",cancion);
        exit(1);
    }

    fseek(disco,query(cancion,disco)*bs,0);

    inodo n;

    fread(&n,sizeof(inodo),1,disco);

    printf("%s",n.tags);

    fclose(disco);
}

void queryinodo(int pos, FILE *disco)
{
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

    printf("Tags: ");

    verTags(temp.filename,disco);

    printf("#");

    printf("Numero de bloques: %d",n.numBloques);
}

int existeTag(char *tag, FILE *disco)
{
    fseek(disco,0,0);

    header temp;

    fread(&temp,sizeof(header),1,disco);

    int pos=(1+(((int)temp.disksize/8)/bs)+(int)temp.fes)*bs;

    fseek(disco,pos,0);

    int i;

    te temp2;

    for(i=0; i<(int)temp.tes*tepb; i++)
    {
        fread(&temp2,sizeof(te),1,disco);

        if(strcmp(temp2.tag,tag)==0)
        {
            return i;
        }
    }

    return -1;
}

int existeInodo(int arr[], int pointer)
{
    int i;

    for(i=0; i<256; i++)
    {
        if(arr[i]==pointer)
        {
            return 1;
        }
    }

    return 0;
}

void putTag(char *tag,char *cancion,char *diskname)
{
    if((disco=fopen(diskname, "rb+"))==NULL)
    {
        printf("TAG:No se pudo abrir %s",diskname);
        exit(1);
    }

    int ver=existeCancion(cancion,disco);

    if(ver==-1)
    {
        printf("TAG:No existe la cancion %s",cancion);
        exit(1);
    }

    fseek(disco,0,0);

    header temp;

    fread(&temp,sizeof(header),1,disco);

    fseek(disco,bs,0);

    unsigned char *mapabits;
    mapabits = (unsigned char *)malloc(((int)temp.disksize/8));

    fread(mapabits,((int)temp.disksize/8),1,disco);

    int length=(int)temp.disksize;

    int i, tepos;

    te temp2;

    int pos=existeTag(tag,disco);

    fseek(disco,(1+((int)temp.disksize/8)/bs+(int)temp.fes)*bs,0);

    if(pos==-1)
    {
        if(cant_apagados(mapabits,length)<1)
        {
                printf("TAG:No existe espacio en %s",diskname);
                exit(1);
        }

        for(i=0; i<(int)temp.tes*tepb; i++)
        {
            tepos=ftell(disco);

            fread(&temp2,sizeof(te),1,disco);

            if(strcmp(temp2.tag,"")==0)
            {
                int pointer=query(cancion,disco);

                fseek(disco,pointer*bs,0);

                inodo n;

                fread(&n,sizeof(inodo),1,disco);

                int l=strlen(n.tags);

                if(l==0)
                {
                    l=250;
                }
                else
                {
                    l=250-l;
                }

                if(l<((int)strlen(tag)+1))
                {
                    printf("TAG:No se pudo taggear %s",cancion);
                    exit(1);
                }

                strcat(n.tags,tag);
                strcat(n.tags,",");

                fseek(disco,pointer*bs,0);

                fwrite(&n,sizeof(inodo),1,disco);

                int a=freeblock(mapabits,length);
                killblock(mapabits,a,length);

                strcpy(temp2.tag,tag);
                temp2.pointer=a;

                int arr[256];

                memset(arr,0,bs);

                arr[0]=pointer;//

                fseek(disco,a*bs,0);
                fwrite(arr,bs,1,disco);

                fseek(disco,tepos,0);
                fwrite(&temp2,sizeof(te),1,disco);

                break;
            }
        }
    }
    else
    {
        int b=(1+((int)temp.disksize/8)/bs+(int)temp.fes)*bs+pos*sizeof(te);

        fseek(disco,b,0);

        fread(&temp2,sizeof(te),1,disco);

        fseek(disco,temp2.pointer*bs,0);

        int arr[256];

        fread(arr,bs,1,disco);

        int i;

        int pointer=query(cancion,disco);//

        for(i=0; i<256; i++)
        {
            if(arr[i]==0)
            {
                fseek(disco,pointer*bs,0);

                inodo n;

                fread(&n,sizeof(inodo),1,disco);

                int l=strlen(n.tags);

                if(l==0)
                {
                    l=250;
                }
                else
                {
                    l=250-l;
                }

                if((l<((int)strlen(tag)+1)) && (existeInodo(arr,pointer)!=0))
                {
                    printf("TAG:No se pudo taggear %s",cancion);
                    exit(1);
                }

                strcat(n.tags,tag);
                strcat(n.tags,",");

                arr[i]=pointer;

                fseek(disco,arr[i]*bs,0);

                fwrite(&n,sizeof(inodo),1,disco);

                break;
            }
        }

        fseek(disco,temp2.pointer*bs,0);

        fwrite(arr,bs,1,disco);
    }

    fseek(disco,bs,0);

    fwrite(mapabits,((int)temp.disksize/8),1,disco);

    fclose(disco);

    free(mapabits);
}

void destaguear(char *cancion,FILE *disco)
{
    int ver=existeCancion(cancion,disco);

    if(ver==-1)
    {
        exit(1);
    }

    fseek(disco,0,0);

    header temp;

    fread(&temp,sizeof(header),1,disco);

    int i, tepos;

    fseek(disco,(1+((int)temp.disksize/8)/bs+(int)temp.fes)*bs,0);

    te temp2;

    for(i=0; i<(int)temp.tes*tepb; i++)
    {
        tepos=ftell(disco);

        fread(&temp2,sizeof(te),1,disco);

        if(strcmp(temp2.tag,"")!=0)
        {
            fseek(disco,temp2.pointer*bs,0);

            int arr[256];

            fread(arr,bs,1,disco);

            int pointer=query(cancion,disco);

            if(existeInodo(arr,pointer)==0)
            {
                continue;
            }

            int j;

            for(j=0; j<256; j++)
            {
                if(arr[j]==pointer)
                {
                    arr[j]=0;
                }
            }

            fseek(disco,temp2.pointer*bs,0);

            fwrite(arr,bs,1,disco);

            fseek(disco,tepos+sizeof(te),0);
        }
    }
}

int contiene(char *cadena, char a)
{
    int i;
    for(i=0; i<strlen(cadena); i++)
    {
        if(cadena[i]==a)
        {
            return 1;
        }
    }
    return 0;
}

void main(int argc, char *argv[])
{
    if(strcmp(argv[1],"-c")==0 && argc==6)
    {
        if((disco=fopen(argv[5], "wb"))==NULL)
        {
            printf("CREAR:No se pudo abri el disco %s",argv[5]);
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
            printf("ADD:No se pudo abri el disco %s",argv[3]);
            exit(1);
        }

        char *dir=argv[2];

        if(contiene(argv[2],'/')!=0)
        {
            argv[2]=strrchr(argv[2],(int)'/');
            argv[2]=strchr(argv[2],(int)argv[2][1]);
        }
        else if(contiene(argv[2],'\\')!=0)
        {
            argv[2]=strrchr(argv[2],(int)'\\');
            argv[2]=strchr(argv[2],(int)argv[2][1]);
        }

        int ver=existeCancion(argv[2],disco);

        if(ver!=-1)
        {
            printf("ADD:Ya existe la cancion %s",argv[2]);
            exit(1);
        }

        if((mp3=fopen(dir, "rb"))==NULL)
        {
            printf("ADD:No se pudo abrir el archivo %s",dir);
            exit(1);
        }

        fseek(mp3,0,2);
        int size=ftell(mp3);

        if(size>=68157440)
        {
            exit(1);
            printf("ADD:El archivo %s excede el size establecido",argv[2]);
        }

        fseek(disco,0,0);

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
                    printf("ADD:No existe espacio en el disco %s",argv[3]);
                    exit(1);
                }
            }
        }

        fclose(disco);
        fclose(mp3);

        free(mapabits);

        char *tag="archivo";

        putTag(tag,argv[2],argv[3]);

        printf("ADD:El archivo %s se ha agregado correctamente",argv[2]);

        //Preeeeeegggggguuuuuuuuunnnnnnnttttaaaaaaaarrrrrrrrrrrrrrr!!!!!!!!!!!!!!!!!!!!!!
    }
    else if(strcmp(argv[1],"-e")==0 && argc==5)
    {
        FILE *mp3;

        int cont=0;

        if((disco=fopen(argv[4], "rb+"))==NULL)
        {
            printf("EXPORTAR:No se pudo abri el disco %s",argv[4]);
            exit(1);
        }

        int ver=existeCancion(argv[2],disco);

        if(ver==-1)
        {
            printf("EXPORTAR:No existe la cancion %s",argv[2]);
            exit(1);
        }

        if((mp3=fopen(argv[3], "wb"))==NULL)
        {
            printf("EXPORTAR:No se pudo abrir el archivo %s",argv[3]);
            exit(1);
        }

        fseek(disco,0,0);

        header temp;

        fread(&temp,sizeof(header),1,disco);

        fseek(disco,(1+(((int)temp.disksize/8)/bs))*bs+ver*sizeof(fe),0);

        fe temp2;

        fread(&temp2,sizeof(fe),1,disco);

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

        fin=0;

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

        fclose(disco);
        fclose(mp3);

        printf("EXPORTAR:%s se ha exportado correctamente",argv[2]);
    }
    else if(strcmp(argv[1],"-q")==0 && argc==4)
    {
        if((disco=fopen(argv[3], "rb"))==NULL)
        {
            printf("QUERY:No se pudo abri el disco %s",argv[3]);
            exit(1);
        }

        int pos=query(argv[2],disco);

        if(pos!=-1)
        {
            printf("QUERY:El numero de inodo de %s es %d",argv[2],pos);
        }
        else
        {
            printf("QUERY:No existe numero de inodo para %s",argv[2]);
        }

        fclose(disco);
    }
    else if(strcmp(argv[1],"-d")==0 && argc==4)
    {
        int cont=0;

        if((disco=fopen(argv[3], "rb+"))==NULL)
        {
            printf("DELETE:No se pudo abri el disco %s",argv[3]);
            exit(1);
        }

        int ver=existeCancion(argv[2],disco);

        if(ver==-1)
        {
            printf("DELETE:No existe la cancion %s",argv[2]);
            exit(1);
        }

        destaguear(argv[2],disco);

        fseek(disco,0,0);

        header temp;

        fread(&temp,sizeof(header),1,disco);

        fseek(disco,bs,0);

        unsigned char *mapabits;
        mapabits = (unsigned char *)malloc(((int)temp.disksize/8));

        fread(mapabits,((int)temp.disksize/8),1,disco);

        int length=(int)temp.disksize;

        unsigned char bloque[bs];
        memset(bloque,0,bs);

        fseek(disco,(1+((int)temp.disksize/8)/bs)*bs+ver*sizeof(fe),0);

        fe temp2;

        int fepos=ftell(disco);

        fread(&temp2,sizeof(fe),1,disco);

        killblock(mapabits,temp2.pointer_inodo,length);

        fseek(disco,temp2.pointer_inodo*bs,0);

        inodo n;

        fread(&n,sizeof(inodo),1,disco);

        int j;

        for(j=0; j<12; j++)
        {
            if(cont!=(int)n.numBloques)
            {
                killblock(mapabits,n.pDirecto[j],length);

                fseek(disco,n.pDirecto[j]*bs,0);

                fwrite(bloque,bs,1,disco);
                cont++;
            }
            else
            {
                break;
            }
        }

        if(n.numBloques>12)//Primer indirecto simple
        {
            killblock(mapabits,n.pIndirecto,length);

            fseek(disco,n.pIndirecto*bs,0);

            int b[256];

            fread(b,bs,1,disco);

            for(j=0; j<256; j++)
            {
                if(cont!=(int)n.numBloques)
                {
                    killblock(mapabits,b[j],length);

                    fseek(disco,b[j]*bs,0);

                    fwrite(bloque,bs,1,disco);
                    cont++;
                }
                else
                {
                    break;
                }
            }

            fseek(disco,n.pIndirecto*bs,0);

            fwrite(bloque,bs,1,disco);
        }

        if(n.numBloques>268)//Indirectos dobles
        {
            killblock(mapabits,n.pIndirectoDoble,length);

            fseek(disco,n.pIndirectoDoble*bs,0);

            int b[256];

            fread(b,bs,1,disco);

            int k;

            for(j=0; j<256; j++)
            {
                if(cont==((int)n.numBloques-1))
                {
                    break;
                }

                killblock(mapabits,b[j],length);

                fseek(disco,b[j]*bs,0);

                int e[256];

                fread(e,bs,1,disco);

                for(k=0; k<256; k++)
                {
                    if(cont!=(int)n.numBloques)
                    {
                        killblock(mapabits,e[k],length);

                        fseek(disco,e[k]*bs,0);

                        fwrite(bloque,bs,1,disco);
                        cont++;
                    }
                    else
                    {
                        break;
                    }
                }

                fseek(disco,b[j]*bs,0);

                fwrite(bloque,bs,1,disco);
            }

            fseek(disco,n.pIndirectoDoble*bs,0);

            fwrite(bloque,bs,1,disco);
        }

        fseek(disco,temp2.pointer_inodo*bs,0);

        fwrite(bloque,bs,1,disco);

        fseek(disco,fepos,0);

        strcpy(temp2.filename,"");
        temp2.pointer_inodo=-1;

        fwrite(&temp2,sizeof(fe),1,disco);

        fseek(disco,bs,0);

        fwrite(mapabits,((int)temp.disksize/8),1,disco);

        fclose(disco);

        free(mapabits);

        printf("DELETE:El archivo %s se ha eliminado correctamente",argv[2]);
    }
    else if(strcmp(argv[1],"-t")==0 && argc==5)
    {
        putTag(argv[2],argv[3],argv[4]);

        printf("TAG:El archivo %s ha sido taggeado con %s",argv[3],argv[2]);
    }
    else if(strcmp(argv[1],"-it")==0 && argc==4)
    {
        if((disco=fopen(argv[3], "rb"))==NULL)
        {
            printf("TAGSCANCION:No se pudo abri el disco %s",argv[3]);
            exit(1);
        }

        printf("TAGSCANCION:");

        verTags(argv[2],disco);
    }
    else if(strcmp(argv[1],"-s")==0 && argc==4)
    {
        if((disco=fopen(argv[3], "rb"))==NULL)
        {
            printf("SEARCHTAG:No se pudo abri el disco %s",argv[3]);
            exit(1);
        }

        header temp;

        fread(&temp,sizeof(header),1,disco);

        te temp2;

        int pos=existeTag(argv[2],disco);

        printf("SEARCHTAG:");

        if(pos!=-1)
        {
            fseek(disco,(1+((int)temp.disksize/8)/bs+(int)temp.fes)*bs+pos*sizeof(te),0);

            fread(&temp2,sizeof(te),1,disco);

            fseek(disco,temp2.pointer*bs,0);

            int arr[256];

            fread(arr,bs,1,disco);

            int j;

            for(j=0; j<256; j++)
            {
                if(arr[j]!=0)
                {
                    printf("%d,",arr[j]*bs);
                }
            }
        }
        else
        {
            printf("SEARCHTAG:No existe el tag %s",argv[2]);
            exit(1);
        }

        fclose(disco);
    }
    else if(strcmp(argv[1],"-qi")==0 && argc==4)
    {
        if((disco=fopen(argv[3],"rb"))==NULL)
        {
            printf("QUERYINODO:No se pudo abrir el disco %s",argv[3]);
            exit(1);
        }

        queryinodo(atoi(argv[2]),disco);

        fclose(disco);
    }
    else if(strcmp(argv[1],"-qa")==0 && argc==4)
    {
        if((disco=fopen(argv[3], "rb"))==NULL)
        {
            printf("QUERYFILE:No se pudo abrir el disco %s",argv[2]);
            exit(1);
        }

        int pointer_inodo=query(argv[2],disco);

        if(pointer_inodo!=-1)
        {
            printf("QUERYFILE:$");
            queryinodo(pointer_inodo*bs,disco);
        }
        else
        {
            printf("QUERYFILE:No existe numero de inodo para %s",argv[2]);
        }

        fclose(disco);
    }
    else if(strcmp(argv[1],"-l")==0 && argc==3)
    {
        if((disco=fopen(argv[2], "rb"))==NULL)
        {
            printf("LISTARTAGS:No se pudo abrir el disco %s",argv[2]);
            exit(1);
        }

        header temp;

        fread(&temp,sizeof(header),1,disco);

        fseek(disco,(1+((int)temp.disksize/8)/bs+(int)temp.fes)*bs,0);

        int i;

        int cont=0;

        te temp2;

        printf("LISTARTAGS:");

        for(i=0; i<(int)temp.tes*tepb; i++)
        {
            fread(&temp2,sizeof(te),1,disco);

            if(strcmp(temp2.tag,"")!=0)
            {
                cont++;
            }
        }

        fseek(disco,(1+((int)temp.disksize/8)/bs+(int)temp.fes)*bs,0);

        char** arr;
        arr=(char**)malloc(cont*sizeof(char*));

        for(i=0; i<cont; i++)
        {
            arr[i]=(char*)malloc(sizeof(char)*28);
        }

        cont=0;

        for(i=0; i<(int)temp.tes*tepb; i++)
        {
            fread(&temp2,sizeof(te),1,disco);

            if(strcmp(temp2.tag,"")!=0)
            {
                strcpy(arr[cont],temp2.tag);

                cont++;
            }
        }

        int j;

        for(i=0; i<cont-1; i++)
        {
            for(j=i+1; j<cont; j++)
            {
                if(strcmp(arr[i],arr[j])>0)
                {
                    char *temp;
                    strcpy(temp,arr[i]);
                    strcpy(arr[i],arr[j]);
                    strcpy(arr[j],temp);
                }
            }
        }

        for(i=0; i<cont; i++)
        {
            printf("%s,",arr[i]);
        }

        fclose(disco);
    }
    else
    {
        printf("Comando no admitido");
        exit(1);
    }
}
