#include "hashlist.h"

hashList::hashList(char *_name, QTextEdit *log)
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

    name=QString(_name);

    log->append("- hashList.h: "+name+" ha sido abierto\n");
}

void hashList::create(int cuantos)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    if(cuantos%96!=0)
    {
        cuantos+=96-cuantos%96;
    }

    unsigned char bloque[bSize];
    memset(bloque,0,bSize);

    disco.seekp(0,ios_base::beg);
    disco.write((char *)bloque,bSize);

    disco.seekp(0,ios_base::beg);
    disco.write((char *)&cuantos,sizeof(int));

    unsigned char *mapabits;
    mapabits = (unsigned char *)malloc(cuantos/8);
    memset(mapabits,0,cuantos/8);

    int fin=1+(cuantos/8)/bSize;

    for(int i=0; i<fin; i++)
    {
        int casilla = i / 8 ;
        mapabits[casilla] = mapabits[casilla] | (1 << (7-i%8));
    }

    disco.seekp(bSize,ios_base::beg);
    disco.write((const char *)mapabits,cuantos/8);

    for(int i=0; i<cuantos; i++)
    {
        disco.write((const char*)bloque,bSize);
        disco.flush();
    }

    log->append("- hashList.h: "+name+" se ha creado con "+QString::number(cuantos)+" bloques\n");

    free(mapabits);

    disco.close();
}

void hashList::add(long id, int &cont, int &raiz, int &tail)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    int cuantos;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&cuantos, sizeof(int));

    mapabits map;

    unsigned char *mapa;
    mapa = (unsigned char *)malloc(cuantos/8);
    memset(mapa,0,cuantos/8);

    disco.seekg(bSize,ios_base::beg);
    disco.read((char *)mapa,cuantos/8);

    if(map.cant_apagados(mapa,cuantos)==0 || exists(id,raiz))
    {
        log->append("- hashList.h: No hay espacio para agregar "+QString::number(id)+" o ya existe\n");
        disco.close();
        return;
    }

    int pos=map.freeblock(mapa,cuantos);
    map.killblock(mapa,pos,cuantos);

    nodoH temp;

    temp.id=id;

    if(cont==0)
    {
        raiz=pos;
        tail=pos;
    }
    else
    {
        temp.anterior=tail;

        nodoH a;

        disco.seekg(tail*bSize,ios_base::beg);
        disco.read((char *)&a,sizeof(nodoH));

        a.siguiente=pos;

        disco.seekp(tail*bSize,ios_base::beg);
        disco.write((const char *)&a,sizeof(nodoH));

        temp.siguiente=-1;
        tail=pos;
    }

    cont++;

    disco.seekp(pos*bSize,ios_base::beg);
    disco.write((const char *)&temp,sizeof(nodoH));

    disco.seekp(bSize,ios_base::beg);
    disco.write((const char *)mapa,cuantos/8);

    free(mapa);

    log->append("- hashList.h: "+QString::number(id)+" ha sido agregado\n");

    disco.close();
}

void hashList::mostrar()
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    int cuantos;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&cuantos, sizeof(int));

    int base=(1+(cuantos/8)/bSize);

    for(int i=0; i<10; i++)
    {
        nodoH temp;

        disco.seekg((base+i)*bSize, ios_base::beg);
        disco.read((char *)&temp, sizeof(nodoH));

        cout<<"------------------------------------"<<endl;
        cout<<"Id: "<<temp.id<<endl;
        cout<<"Siguiente: "<<temp.siguiente<<endl;
        cout<<"Anterior: "<<temp.anterior<<endl;
    }

    disco.close();
}

bool hashList::exists(long id, int &raiz)
{
    nodoH temp;

        int sig=raiz;
        while(sig!=-1)
        {
            disco.seekg(sig*bSize,ios_base::beg);
            disco.read((char *)&temp,sizeof(nodoH));

            if(id==temp.id)
            {
                return true;
            }

            sig=temp.siguiente;
        }

        return false;
}

//int hashList::posRrn(int rrn, int &cont, int &raiz)
//{
//    if(cont==0)
//    {
//        return -1;
//    }

//    int c=0;
//    int sig=raiz;

//    while(sig!=-1)
//    {
//        nodoH temp;

//        disco.seekg(sig*bSize,ios_base::beg);
//        disco.read((char *)&temp,sizeof(nodoH));

//        if(temp.rrn==rrn)
//        {
//            return c;
//        }
//        sig=temp.siguiente;
//        c++;
//    }

//    return -1;
//}

//int hashList::pos(int rrn, int &cont, int &raiz)
//{
//    if(cont==0)
//    {
//        return -1;
//    }

//    int c=0;
//    int sig=raiz;

//    while(sig!=-1)
//    {
//        nodoH temp;

//        disco.seekg(sig*bSize,ios_base::beg);
//        disco.read((char *)&temp,sizeof(nodoH));

//        if(temp.rrn==rrn && !temp.disponible)
//        {
//            return c;
//        }
//        sig=temp.siguiente;
//        c++;
//    }

//    return -1;
//}

nodoH hashList::at(int pos, int &cont, int &raiz)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    nodoH temp;

    if(pos<cont)
    {
        int c=0;
        int sig=raiz;
        while(sig!=-1)
        {
            disco.seekg(sig*bSize,ios_base::beg);
            disco.read((char *)&temp,sizeof(nodoH));

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

//void hashList::deleteRrn(int pos, int &cont, int &raiz, int &tail)
//{
//    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

//    int cuantos;

//    disco.seekg(0,ios_base::beg);
//    disco.read((char *)&cuantos,sizeof(int));

//    mapabits map;

//    unsigned char *mapa;
//    mapa = (unsigned char *)malloc(cuantos/8);
//    memset(mapa,0,cuantos/8);

//    disco.seekg(bSize,ios_base::beg);
//    disco.read((char *)mapa,cuantos/8);

//    if(pos<cuantos)
//    {
//        int c=0;
//        int s=raiz;
//        while(s!=-1)
//        {
//            nodoH temp;

//            disco.seekg(s*bSize,ios_base::beg);
//            disco.read((char *)&temp, sizeof(nodoH));

//            if(c==pos)
//            {
//                int sig, ant;
//                sig=temp.siguiente;
//                ant=temp.anterior;
//                if(pos==0 && cont==1)
//                {
//                    nodoH a;

//                    disco.seekg(raiz*bSize, ios_base::beg);
//                    disco.read((char *)&a, sizeof(nodoH));

//                    map.killblock(mapa,a.pos,cuantos);

//                    raiz=-1;
//                    tail=-1;
//                }
//                else if(pos==0)
//                {
//                    nodoH a;

//                    disco.seekg(raiz*bSize, ios_base::beg);
//                    disco.read((char *)&a, sizeof(nodoH));

//                    map.killblock(mapa,a.pos,cuantos);

//                    nodoH b;

//                    disco.seekg(sig*bSize, ios_base::beg);
//                    disco.read((char *)&b, sizeof(nodoH));

//                    b.anterior=-1;

//                    raiz=sig;

//                    disco.seekp(sig*bSize, ios_base::beg);
//                    disco.write((const char *)&b, sizeof(nodoH));
//                }
//                else if(pos>0 && pos<(cont-1))
//                {
//                    map.killblock(mapa,temp.pos,cuantos);

//                    nodoH a;

//                    disco.seekg(sig*bSize, ios_base::beg);
//                    disco.read((char *)&a, sizeof(nodoH));

//                    a.anterior=ant;

//                    nodoH b;

//                    disco.seekg(ant*bSize, ios_base::beg);
//                    disco.read((char *)&b, sizeof(nodoH));

//                    b.siguiente=sig;

//                    disco.seekp(sig*bSize, ios_base::beg);
//                    disco.write((const char *)&a, sizeof(nodoH));

//                    disco.seekp(ant*bSize, ios_base::beg);
//                    disco.write((const char *)&b, sizeof(nodoH));
//                }
//                else
//                {
//                    map.killblock(mapa,temp.pos,cuantos);

//                    tail=s;

//                    nodoH b;

//                    disco.seekg(ant*bSize, ios_base::beg);
//                    disco.read((char *)&b, sizeof(nodoH));

//                    b.siguiente=-1;
//                }
//                cont--;

//                break;
//            }

//            s=temp.siguiente;
//            c++;
//        }
//    }

//    disco.seekp(bSize,ios_base::beg);
//    disco.write((const char *)mapa,cuantos/8);

//    free(mapa);

//    disco.close();
//}
