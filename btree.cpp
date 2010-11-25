#include "btree.h"

btree::btree(char *_name)
{
    disco.open(_name,ios_base::binary | ios_base::in);
    if(!disco)
    {
        disco.open(_name,ios_base::binary | ios_base::out);
    }
    else
    {
        disco.open(_name, ios_base::binary | ios_base::in | ios_base::out);
    }

    disco.close();
    int len=strlen(_name);
    name=new char[len+1];
    memcpy(name,_name,len);
    name[len+1]='\0';
}

void btree::create(int cuantos)
{
    disco.open(name,ios_base::binary | ios_base::in | ios_base::out);

    header h;

    int val = blocksize*8;

    if(cuantos%val!=0)
    {
        cuantos+=val-cuantos%val;
    }

    h.total = cuantos;

    unsigned char bloque[blocksize];
    memset(bloque,0,blocksize);

    disco.seekg(0,ios_base::beg);//para los bloques
    disco.write((const char*)&bloque,blocksize);

    disco.seekp(0,ios_base::beg);//del header
    disco.write((const char*)&h,sizeof(header));

    unsigned char *mapabits;
    mapabits = (unsigned char *)malloc(h.total/8);
    memset(mapabits,0,h.total/8);

    int final=1+(h.total/8)/blocksize;

    for(int i=0; i<final; i++)
    {
        int c = i / 8 ;
        mapabits[c] = mapabits[c] | (1 << (7-i%8));
    }

    disco.seekp(blocksize,ios_base::beg);//set bits
    disco.write((const char*)mapabits,h.total/8);

    for(int i=0; i<h.total; i++)
    {
        disco.write((const char*)bloque,blocksize);
        disco.flush();
    }

    disco.close();
}

void btree::add(int id, int rrn)
{
    disco.open(name,ios_base::binary | ios_base::in | ios_base::out);

    header h;

    disco.seekg(0,ios_base::beg);
    disco.read((char*)&h,sizeof(header));

    if(h.cuantos==h.total)
    {
        cout<<"Full";
        return;
    }

    mapabits map;

    unsigned char *mapabits;
    mapabits = (unsigned char*)malloc(h.total/8);
    memset(mapabits,0,h.total/8);

    disco.seekg(blocksize,ios_base::beg);
    disco.read((char*)mapabits,h.total/8);

    if(h.raiz==-1)//Primera vez no hay Raiz
    {
        int pos = map.freeblock(mapabits,h.total);
        map.killblock(mapabits,pos,h.total);

        nodoB temp;

        h.raiz = pos;
        temp.llavesPrimarias[0].id = id;
        temp.llavesPrimarias[0].rrn = rrn;
        temp.cuantos++;

        disco.seekp(pos*blocksize,ios_base::beg);
        disco.write((const char*)&temp,sizeof(nodoB));
    }
    else
    {
        addRecursiva(mapabits, h.total, h.raiz, id, rrn);
        //si existe agregarlo(Falta Funca Agregar), si no Return;
        // el Search Me gusto este mira http://cs-netlab-01.lynchburg.edu/courses/DataStII/BTreeOps.htm
    }

    // si llego aqui se agrego
    h.cuantos++;

    // termino Escribamos header y mB
    disco.seekp(0,ios_base::beg);
    disco.write((const char*)&h,sizeof(header));

    disco.seekp(blocksize,ios_base::beg);
    disco.write((const char*)mapabits,h.total/8);

    disco.close();
}

void btree::addRecursiva(unsigned char *m, int cuantos, int pos, int id, int rrn)
{
    nodoB temp;

    disco.seekg(pos*blocksize, ios_base::beg);
    disco.read((char *)&temp, sizeof(nodoB));

    if(temp.apuntadores[0]==-1 && temp.cuantos < maxLlaves)
    {
        temp.llavesPrimarias[temp.cuantos].id=id;
        temp.llavesPrimarias[temp.cuantos].rrn=rrn;

        sort(temp.llavesPrimarias, temp.apuntadores, temp.cuantos);
    }
    else if(temp.apuntadores[0]!=-1)
    {
        if(id < temp.llavesPrimarias[temp.cuantos-1].id)
        {
            if(temp.apuntadores[temp.cuantos-1]!=-1)
            {
                addRecursiva(m,cuantos,temp.apuntadores[temp.cuantos-1],id,rrn);
            }
            else
            {
                mapabits map;

                int i=map.freeblock(m,cuantos);
                map.killblock(m,i,cuantos);

                temp.apuntadores[temp.cuantos-1]=i;

                addRecursiva(m,cuantos,i,id,rrn);
            }
        }
        else if(id > temp.llavesPrimarias[temp.cuantos-1].id)
        {
            if(temp.apuntadores[temp.cuantos]!=-1)
            {
                addRecursiva(m,cuantos,temp.apuntadores[temp.cuantos],id,rrn);
            }
            else
            {
                mapabits map;

                int i=map.freeblock(m,cuantos);
                map.killblock(m,i,cuantos);

                temp.apuntadores[temp.cuantos]=i;

                addRecursiva(m,cuantos,i,id,rrn);
            }
        }
    }
    else if(temp.cuantos==maxLlaves)
    {
        int i;

        for(i=0; i<temp.cuantos; i++)
        {
            if(id < temp.llavesPrimarias[i].id)
            {
                i++;
                break;
            }
        }

        int j=(temp.cuantos+1)/2;

        entry *a=temp.llavesPrimarias;
        int *b=temp.apuntadores;
        temp.cuantos=1;

        clear(temp.llavesPrimarias, temp.apuntadores);

        temp.llavesPrimarias[0].id=id;
        temp.llavesPrimarias[0].rrn=rrn;
    }

    disco.seekp(pos*blocksize, ios_base::beg);
    disco.write((const char *)&temp, sizeof(nodoB));
}

void btree::clear(entry *a, int *b)
{
    for(int i=0; i<maxApuntadores; i++)
    {
        if(i<maxLlaves)
        {
            a[i].id=-1;
            a[i].rrn=-1;
        }
        b[i]=-1;
    }
}

void btree::sort(entry *arr, int *aps, int fin)
{
    for(int i=0; i<(fin-1); i++)
    {
        entry ii=arr[i];
        for(int j=i+1; j<fin; j++)
        {
            entry jj=arr[j];
            if(arr[i].id > arr[j].id)
            {
                arr[i]=jj;
                arr[j]=ii;
            }
        }
    }
}
