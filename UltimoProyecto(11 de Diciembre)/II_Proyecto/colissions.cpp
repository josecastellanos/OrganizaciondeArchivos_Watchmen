#include "colissions.h"

Colissions::Colissions(char *_name, QTextEdit *log)
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

    log->append("- Colissions.h: "+name+" ha sido abierto\n");

    int n=name.indexOf('.',0,Qt::CaseSensitive);

    QString listname=name.mid(0,n);

    listname.append("List.dat");

    lista = new hashList((char *)listname.toStdString().c_str(),log);
}

void Colissions::create(int cuantos)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    int val=Csize*8;

    if(cuantos%val!=0)
    {
        cuantos+=val-cuantos%val;
    }

    unsigned char bloque[Csize];
    memset(bloque,0,Csize);

    disco.seekp(0,ios_base::beg);
    disco.write((char *)bloque,Csize);

    disco.seekp(0,ios_base::beg);
    disco.write((char *)&cuantos,sizeof(int));

    unsigned char *mapabits;
    mapabits = (unsigned char *)malloc(cuantos/8);
    memset(mapabits,0,cuantos/8);

    int fin=1+(cuantos/8)/Csize;

    for(int i=0; i<fin; i++)
    {
        int casilla = i / 8 ;
        mapabits[casilla] = mapabits[casilla] | (1 << (7-i%8));
    }

    disco.seekp(Csize,ios_base::beg);
    disco.write((const char *)mapabits,cuantos/8);

    for(int i=0; i<cuantos; i++)
    {
        disco.write((const char*)bloque,Csize);
        disco.flush();
    }

    log->append("- Colissions.h: "+name+" se ha creado con "+QString::number(cuantos)+" bloques\n");

    lista->create(cuantos);

    free(mapabits);

    disco.close();
}

void Colissions::add(char *info, long id, int &cont, int &raiz, int &tail)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    int cuantos;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&cuantos, sizeof(int));

    mapabits map;

    unsigned char *mapa;
    mapa = (unsigned char *)malloc(cuantos/8);
    memset(mapa,0,cuantos/8);

    disco.seekg(Csize,ios_base::beg);
    disco.read((char *)mapa,cuantos/8);

    if(map.cant_apagados(mapa,cuantos)==0)
    {
        log->append("- Colissions.h: No hay espacio para agregar "+QString::number(id)+"\n");
        disco.close();
        return;
    }

    int p;
    nodoCol n=GiveMe(info,raiz,p);

    if(strcmp(n.info,"")!=0)
    {
        lista->add(id,n.cuantos,n.inicio,n.final);

        disco.seekp(p*Csize,ios_base::beg);
        disco.write((const char *)&n,sizeof(nodoCol));
    }
    else
    {
        int pos=map.freeblock(mapa,cuantos);
        map.killblock(mapa,pos,cuantos);

        nodoCol temp;

        strcpy(temp.info,info);

        if(cont==0)
        {
            raiz=pos;
            tail=pos;
        }
        else
        {
            temp.anterior=tail;

            nodoCol a;

            disco.seekg(tail*Csize,ios_base::beg);
            disco.read((char *)&a,sizeof(nodoCol));

            a.siguiente=pos;

            disco.seekp(tail*Csize,ios_base::beg);
            disco.write((const char *)&a,sizeof(nodoCol));

            temp.siguiente=-1;
            tail=pos;
        }

        cont++;

        lista->add(id,temp.cuantos,temp.inicio,temp.final);

        disco.seekp(pos*Csize,ios_base::beg);
        disco.write((const char *)&temp,sizeof(nodoCol));
    }

    disco.seekp(Csize,ios_base::beg);
    disco.write((const char *)mapa,cuantos/8);

    free(mapa);

    log->append("- Colissions.h: "+QString(info)+" ha sido agregado o actualizado\n");

    disco.close();
}

//void Colissions::mostrar()
//{
//    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

//    int cuantos;

//    disco.seekg(0,ios_base::beg);
//    disco.read((char *)&cuantos, sizeof(int));

//    int base=(1+(cuantos/8)/Csize);

//    for(int i=0; i<10; i++)
//    {
//        nodoCol temp;

//        disco.seekg((base+i)*Csize, ios_base::beg);
//        disco.read((char *)&temp, sizeof(nodoCol));

//        cout<<"------------------------------------"<<endl;
//        cout<<"Id: "<<temp.id<<endl;
//        cout<<"Siguiente: "<<temp.siguiente<<endl;
//        cout<<"Anterior: "<<temp.anterior<<endl;
//    }

//    disco.close();
//}

//bool Colissions::exists(char *info, int &raiz)
//{
//    nodoCol temp;

//        int sig=raiz;
//        while(sig!=-1)
//        {
//            disco.seekg(sig*Csize,ios_base::beg);
//            disco.read((char *)&temp,sizeof(nodoCol));

//            if(strcmp(temp.info,info)==0)
//            {
//                return true;
//            }

//            sig=temp.siguiente;
//        }

//        return false;
//}

nodoCol Colissions::GiveMe(char *info, int &raiz, int &pos)
{
    nodoCol temp;

        int sig=raiz;
        while(sig!=-1)
        {
            disco.seekg(sig*Csize,ios_base::beg);
            disco.read((char *)&temp,sizeof(nodoCol));

            if(strcmp(temp.info,info)==0)
            {
                pos=sig;
                return temp;
            }

            sig=temp.siguiente;
        }

        return temp;
}

//int Colissions::posRrn(int rrn, int &cont, int &raiz)
//{
//    if(cont==0)
//    {
//        return -1;
//    }

//    int c=0;
//    int sig=raiz;

//    while(sig!=-1)
//    {
//        nodoCol temp;

//        disco.seekg(sig*Csize,ios_base::beg);
//        disco.read((char *)&temp,sizeof(nodoCol));

//        if(temp.rrn==rrn)
//        {
//            return c;
//        }
//        sig=temp.siguiente;
//        c++;
//    }

//    return -1;
//}

//int Colissions::pos(int rrn, int &cont, int &raiz)
//{
//    if(cont==0)
//    {
//        return -1;
//    }

//    int c=0;
//    int sig=raiz;

//    while(sig!=-1)
//    {
//        nodoCol temp;

//        disco.seekg(sig*Csize,ios_base::beg);
//        disco.read((char *)&temp,sizeof(nodoCol));

//        if(temp.rrn==rrn && !temp.disponible)
//        {
//            return c;
//        }
//        sig=temp.siguiente;
//        c++;
//    }

//    return -1;
//}

nodoCol Colissions::at(int pos, int &cont, int &raiz)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    nodoCol temp;

    if(pos<cont)
    {
        int c=0;
        int sig=raiz;
        while(sig!=-1)
        {
            disco.seekg(sig*Csize,ios_base::beg);
            disco.read((char *)&temp,sizeof(nodoCol));

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

//void Colissions::deleteRrn(int pos, int &cont, int &raiz, int &tail)
//{
//    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

//    int cuantos;

//    disco.seekg(0,ios_base::beg);
//    disco.read((char *)&cuantos,sizeof(int));

//    mapabits map;

//    unsigned char *mapa;
//    mapa = (unsigned char *)malloc(cuantos/8);
//    memset(mapa,0,cuantos/8);

//    disco.seekg(Csize,ios_base::beg);
//    disco.read((char *)mapa,cuantos/8);

//    if(pos<cuantos)
//    {
//        int c=0;
//        int s=raiz;
//        while(s!=-1)
//        {
//            nodoCol temp;

//            disco.seekg(s*Csize,ios_base::beg);
//            disco.read((char *)&temp, sizeof(nodoCol));

//            if(c==pos)
//            {
//                int sig, ant;
//                sig=temp.siguiente;
//                ant=temp.anterior;
//                if(pos==0 && cont==1)
//                {
//                    nodoCol a;

//                    disco.seekg(raiz*Csize, ios_base::beg);
//                    disco.read((char *)&a, sizeof(nodoCol));

//                    map.killblock(mapa,a.pos,cuantos);

//                    raiz=-1;
//                    tail=-1;
//                }
//                else if(pos==0)
//                {
//                    nodoCol a;

//                    disco.seekg(raiz*Csize, ios_base::beg);
//                    disco.read((char *)&a, sizeof(nodoCol));

//                    map.killblock(mapa,a.pos,cuantos);

//                    nodoCol b;

//                    disco.seekg(sig*Csize, ios_base::beg);
//                    disco.read((char *)&b, sizeof(nodoCol));

//                    b.anterior=-1;

//                    raiz=sig;

//                    disco.seekp(sig*Csize, ios_base::beg);
//                    disco.write((const char *)&b, sizeof(nodoCol));
//                }
//                else if(pos>0 && pos<(cont-1))
//                {
//                    map.killblock(mapa,temp.pos,cuantos);

//                    nodoCol a;

//                    disco.seekg(sig*Csize, ios_base::beg);
//                    disco.read((char *)&a, sizeof(nodoCol));

//                    a.anterior=ant;

//                    nodoCol b;

//                    disco.seekg(ant*Csize, ios_base::beg);
//                    disco.read((char *)&b, sizeof(nodoCol));

//                    b.siguiente=sig;

//                    disco.seekp(sig*Csize, ios_base::beg);
//                    disco.write((const char *)&a, sizeof(nodoCol));

//                    disco.seekp(ant*Csize, ios_base::beg);
//                    disco.write((const char *)&b, sizeof(nodoCol));
//                }
//                else
//                {
//                    map.killblock(mapa,temp.pos,cuantos);

//                    tail=s;

//                    nodoCol b;

//                    disco.seekg(ant*Csize, ios_base::beg);
//                    disco.read((char *)&b, sizeof(nodoCol));

//                    b.siguiente=-1;
//                }
//                cont--;

//                break;
//            }

//            s=temp.siguiente;
//            c++;
//        }
//    }

//    disco.seekp(Csize,ios_base::beg);
//    disco.write((const char *)mapa,cuantos/8);

//    free(mapa);

//    disco.close();
//}
