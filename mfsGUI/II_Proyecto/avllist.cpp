#include "avllist.h"

avlList::avlList(char *_name, QTextEdit *log)
{
    this->log=log;

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

    log->append("- avlList.h: "+QString(_name)+" ha sido abierto\n");

    int len=strlen(_name);
    name=new char[len+1];
    memcpy(name,_name,len);
    name[len+1]='\0';
}

void avlList::create(int cuantos)
{
    disco.open(name,ios::binary | ios::in | ios::out);

    if(cuantos%128!=0)
    {
        cuantos+=128-cuantos%128;
    }

    unsigned char bloque[bsize];
    memset(bloque,0,bsize);

    disco.seekp(0,ios_base::beg);
    disco.write((char *)bloque,bsize);

    disco.seekp(0,ios_base::beg);
    disco.write((char *)&cuantos,sizeof(int));

    unsigned char *mapabits;
    mapabits = (unsigned char *)malloc(cuantos/8);
    memset(mapabits,0,cuantos/8);

    int fin=1+(cuantos/8)/bsize;

    for(int i=0; i<fin; i++)
    {
        int casilla = i / 8 ;
        mapabits[casilla] = mapabits[casilla] | (1 << (7-i%8));
    }

    disco.seekp(bsize,ios_base::beg);
    disco.write((const char *)mapabits,cuantos/8);

    for(int i=0; i<cuantos; i++)
    {
        disco.write((const char*)bloque,bsize);
        disco.flush();
    }

    log->append("- avlList.h: "+QString(name)+" se ha creado con "+QString::number(cuantos)+" bloques\n");

    free(mapabits);

    disco.close();
}

void avlList::add(long id, int &cont, int &raiz, int &tail)
{
    disco.open(name,ios::binary | ios::in | ios::out);

    int cuantos;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&cuantos, sizeof(int));

    mapabits map;

    unsigned char *mapa;
    mapa = (unsigned char *)malloc(cuantos/8);
    memset(mapa,0,cuantos/8);

    disco.seekg(bsize,ios_base::beg);
    disco.read((char *)mapa,cuantos/8);

    if(map.cant_apagados(mapa,cuantos)==0 || exists(id,raiz))
    {
        log->append("- avlList.h: No hay espacio para agregar "+QString::number(id)+" o ya existe\n");
        disco.close();
        return;
    }

    int pos=map.freeblock(mapa,cuantos);
    map.killblock(mapa,pos,cuantos);

    nodo temp;

    temp.id=id;
    temp.pos=pos;

    if(cont==0)
    {
        raiz=pos;
        tail=pos;
    }
    else
    {
        temp.anterior=tail;

        nodo a;

        disco.seekg(tail*bsize,ios_base::beg);
        disco.read((char *)&a,sizeof(nodo));

        a.siguiente=pos;

        disco.seekp(tail*bsize,ios_base::beg);
        disco.write((const char *)&a,sizeof(nodo));

        temp.siguiente=-1;
        tail=pos;
    }

    cont++;

    disco.seekp(pos*bsize,ios_base::beg);
    disco.write((const char *)&temp,sizeof(nodo));

    disco.seekp(bsize,ios_base::beg);
    disco.write((const char *)mapa,cuantos/8);

    free(mapa);

    log->append("- avlList.h: "+QString::number(id)+" ha sido agregado\n");

    disco.close();
}

void avlList::mostrar()
{
    disco.open(name,ios::binary | ios::in | ios::out);

    int cuantos;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&cuantos, sizeof(int));

    int base=(1+(cuantos/8)/bsize);

    for(int i=0; i<10; i++)
    {
        nodo temp;

        disco.seekg((base+i)*bsize, ios_base::beg);
        disco.read((char *)&temp, sizeof(nodo));

        cout<<"------------------------------------"<<endl;
        cout<<"Pos: "<<temp.pos<<endl;
        cout<<"Id: "<<temp.id<<endl;
        cout<<"Siguiente: "<<temp.siguiente<<endl;
        cout<<"Anterior: "<<temp.anterior<<endl;
    }

    disco.close();
}

bool avlList::exists(long id, int &raiz)
{
    nodo temp;

        int sig=raiz;
        while(sig!=-1)
        {
            disco.seekg(sig*bsize,ios_base::beg);
            disco.read((char *)&temp,sizeof(nodo));

            if(id==temp.id)
            {
                return true;
            }

            sig=temp.siguiente;
        }

        return false;
}

//int avlList::posRrn(int rrn, int &cont, int &raiz)
//{
//    if(cont==0)
//    {
//        return -1;
//    }

//    int c=0;
//    int sig=raiz;

//    while(sig!=-1)
//    {
//        nodo temp;

//        disco.seekg(sig*bsize,ios_base::beg);
//        disco.read((char *)&temp,sizeof(nodo));

//        if(temp.rrn==rrn)
//        {
//            return c;
//        }
//        sig=temp.siguiente;
//        c++;
//    }

//    return -1;
//}

//int avlList::pos(int rrn, int &cont, int &raiz)
//{
//    if(cont==0)
//    {
//        return -1;
//    }

//    int c=0;
//    int sig=raiz;

//    while(sig!=-1)
//    {
//        nodo temp;

//        disco.seekg(sig*bsize,ios_base::beg);
//        disco.read((char *)&temp,sizeof(nodo));

//        if(temp.rrn==rrn && !temp.disponible)
//        {
//            return c;
//        }
//        sig=temp.siguiente;
//        c++;
//    }

//    return -1;
//}

nodo avlList::at(int pos, int &cont, int &raiz)
{
    if(disco.is_open())
    {
        disco.close();
    }

    disco.open(name, ios::binary | ios::in | ios::out);

    nodo temp;

    if(pos<cont)
    {
        int c=0;
        int sig=raiz;
        while(sig!=-1)
        {
            disco.seekg(sig*bsize,ios_base::beg);
            disco.read((char *)&temp,sizeof(nodo));

            if(c==pos)
            {
                break;
            }

            sig=temp.siguiente;
            c++;
        }
    }

    disco.close();

    return temp;
}

void avlList::deleteRrn(int pos, int &cont, int &raiz, int &tail)
{
    disco.open(name, ios::binary | ios::in | ios::out);

    int cuantos;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&cuantos,sizeof(int));

    mapabits map;

    unsigned char *mapa;
    mapa = (unsigned char *)malloc(cuantos/8);
    memset(mapa,0,cuantos/8);

    disco.seekg(bsize,ios_base::beg);
    disco.read((char *)mapa,cuantos/8);

    if(pos<cuantos)
    {
        int c=0;
        int s=raiz;
        while(s!=-1)
        {
            nodo temp;

            disco.seekg(s*bsize,ios_base::beg);
            disco.read((char *)&temp, sizeof(nodo));

            if(c==pos)
            {
                int sig, ant;
                sig=temp.siguiente;
                ant=temp.anterior;
                if(pos==0 && cont==1)
                {
                    nodo a;

                    disco.seekg(raiz*bsize, ios_base::beg);
                    disco.read((char *)&a, sizeof(nodo));

                    map.killblock(mapa,a.pos,cuantos);

                    raiz=-1;
                    tail=-1;
                }
                else if(pos==0)
                {
                    nodo a;

                    disco.seekg(raiz*bsize, ios_base::beg);
                    disco.read((char *)&a, sizeof(nodo));

                    map.killblock(mapa,a.pos,cuantos);

                    nodo b;

                    disco.seekg(sig*bsize, ios_base::beg);
                    disco.read((char *)&b, sizeof(nodo));

                    b.anterior=-1;

                    raiz=sig;

                    disco.seekp(sig*bsize, ios_base::beg);
                    disco.write((const char *)&b, sizeof(nodo));
                }
                else if(pos>0 && pos<(cont-1))
                {
                    map.killblock(mapa,temp.pos,cuantos);

                    nodo a;

                    disco.seekg(sig*bsize, ios_base::beg);
                    disco.read((char *)&a, sizeof(nodo));

                    a.anterior=ant;

                    nodo b;

                    disco.seekg(ant*bsize, ios_base::beg);
                    disco.read((char *)&b, sizeof(nodo));

                    b.siguiente=sig;

                    disco.seekp(sig*bsize, ios_base::beg);
                    disco.write((const char *)&a, sizeof(nodo));

                    disco.seekp(ant*bsize, ios_base::beg);
                    disco.write((const char *)&b, sizeof(nodo));
                }
                else
                {
                    map.killblock(mapa,temp.pos,cuantos);

                    tail=s;

                    nodo b;

                    disco.seekg(ant*bsize, ios_base::beg);
                    disco.read((char *)&b, sizeof(nodo));

                    b.siguiente=-1;
                }
                cont--;

                break;
            }

            s=temp.siguiente;
            c++;
        }
    }

    disco.seekp(bsize,ios_base::beg);
    disco.write((const char *)mapa,cuantos/8);

    free(mapa);

    disco.close();
}
