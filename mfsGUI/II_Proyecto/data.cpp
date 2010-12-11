#include "data.h"

data::data(char *_name)
{
    disco.open(_name,ios::binary | ios::in);
    if(!disco)
    {
        disco.open(_name,ios::binary | ios::out);
    }
    else
    {
        disco.open(_name,ios::binary | ios::in | ios::out);
    }
    disco.close();

    int len=strlen(_name);
    name=new char[len+1];
    memcpy(name,_name,len);
    name[len+1]='\0';    

    int n=dondevapunto(name);

    int a=n+10;
    int b=n+8;

    char *bname=new char[a+1];
    char *avlname=new char[b+1];

    memcpy(bname,name,n);
    strcat(bname,"_btree.dat");
    bname[a+1]='\0';

    memcpy(avlname,name,n);
    strcat(avlname,"_avl.dat");
    avlname[b+1]='\0';

    arbolB=new btree(bname);
    arbolAVL=new avl(avlname);
}

int data::dondevapunto(char *a)
{
    for(int i=0; i<strlen(a); i++)
    {
        if(a[i]=='.')
        {
            return i;
        }
    }
}

void data::create(int cuantos)//Modificar cuando este lo de hash
{
    disco.open(name,ios::binary | ios::in | ios::out);

    if(cuantos%2560!=0)
    {
        cuantos+=2560-cuantos%2560;
    }

    unsigned char bloque[blocks];
    memset(bloque,0,blocks);

    disco.seekp(0,ios_base::beg);
    disco.write((char *)bloque,blocks);

    disco.seekp(0,ios_base::beg);
    disco.write((char *)&cuantos,sizeof(int));

    unsigned char *mapabits;
    mapabits = (unsigned char *)malloc(cuantos/8);
    memset(mapabits,0,cuantos/8);

    int fin=1+(cuantos/8)/blocks;

    for(int i=0; i<fin; i++)
    {
        int casilla = i / 8 ;
        mapabits[casilla] = mapabits[casilla] | (1 << (7-i%8));
    }

    disco.seekp(blocks,ios_base::beg);
    disco.write((const char *)mapabits,cuantos/8);

    for(int i=0; i<cuantos; i++)
    {
        disco.write((const char*)bloque,blocks);
        disco.flush();
    }

    arbolB->create(cuantos);
    arbolAVL->create(cuantos);

    disco.close();
}

void data::add(long id, double time, char *source, char *destino, char *protocolo, char *info)
{
    disco.open(name,ios::binary | ios::in | ios::out);

    int cuantos;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&cuantos,sizeof(int));

    mapabits map;

    unsigned char *mapabits;
    mapabits = (unsigned char *)malloc(cuantos/8);
    memset(mapabits,0,cuantos/8);

    disco.seekg(blocks,ios_base::beg);
    disco.read((char *)mapabits,cuantos/8);

    if(map.cant_apagados(mapabits,cuantos)==0 && arbolB->search(id).rrn!=-1)//Poner lo de buscar en el arbol B para ver si existe
    {
        disco.close();
        return;
    }

    int pos=map.freeblock(mapabits,cuantos);
    map.killblock(mapabits,pos,cuantos);

    nodo_data temp;

    temp.id=id;
    temp.time=time;

    strcpy(temp.source,source);
    strcpy(temp.destino,destino);
    strcpy(temp.protocolo,protocolo);
    strcpy(temp.info,info);

    disco.seekp(pos*blocks,ios_base::beg);
    disco.write((const char *)&temp,sizeof(nodo_data));

    disco.seekp(blocks,ios_base::beg);
    disco.write((const char *)mapabits,cuantos/8);

    arbolB->add(id,pos);
    arbolAVL->add(source,id,pos);

    disco.close();
}

void data::deleteRecord(long id)
{
    disco.open(name,ios::binary | ios::in | ios::out);

    int cuantos;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&cuantos,sizeof(int));

    mapabits map;

    unsigned char *mapabits;
    mapabits = (unsigned char *)malloc(cuantos/8);
    memset(mapabits,0,cuantos/8);

    disco.seekg(blocks,ios_base::beg);
    disco.read((char *)mapabits,cuantos/8);

    int rrn=arbolB->search(id).rrn;

    if(rrn==-1)
    {
        disco.close();
        return;
    }

    nodo_data temp;

    disco.seekg(rrn*blocks,ios_base::beg);
    disco.read((char *)&temp,sizeof(nodo_data));

    map.killblock(mapabits,rrn,cuantos);

    disco.seekp(blocks,ios_base::beg);
    disco.write((const char *)mapabits,cuantos/8);

    arbolAVL->deleteId(temp.source,id);

    disco.close();
}

void data::search(char *source)
{
    arbolAVL->mostrarLista(source);
}
