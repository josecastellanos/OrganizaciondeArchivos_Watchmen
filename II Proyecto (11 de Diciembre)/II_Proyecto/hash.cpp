#include "hash.h"

hash::hash(char *_name, QTextEdit *log)
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

    log->append("- hash.h: "+name+" ha sido abierto\n");

    int n=name.indexOf('.',0,Qt::CaseSensitive);

    QString hashListname=name.mid(0,n);

    hashListname.append("Colisiones.dat");

    lista = new Colissions((char*)hashListname.toStdString().c_str(),log);
}

void hash::create(int cuantos)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

        for(int i=0; i<cuantos; i++)
        {
            nodoHash temp;
            disco.write((const char*)&temp,sizeof(nodoHash));
            disco.flush();
        }

        log->append("- hash.h: "+name+" se ha creado con "+QString::number(cuantos)+" bloques\n");

        lista->create(cuantos);

        tam = cuantos;

        disco.close();
}

unsigned int hash::makeHash(char *key)
{
    unsigned int val =0;
    int size = strlen(key);
    for(int i=0; i<size; i++)
        val = 37*val + (int)key[i];

    return val%tam;
}

void hash::add(char *info, long id)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    int pos = makeHash(info);

    nodoHash temp;

    disco.seekg(pos*Hsize,ios_base::beg);
    disco.read((char*)&temp,sizeof(nodoHash));

        lista->add(info,id,temp.cuantos,temp.inicio,temp.final);

        disco.seekp(pos*Hsize,ios_base::beg);
        disco.write((const char*)&temp,sizeof(nodoHash));

        log->append("- hash.h: "+QString(info)+" ha sido agregado\n");

    disco.close();
}

nodoHash hash::search(char *info)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    int pos = makeHash(info);

    nodoHash temp;

    disco.seekg(pos*Hsize,ios_base::beg);
    disco.read((char*)&temp,sizeof(nodoHash));

    log->append("- hash.h: Se ha buscado a "+QString(info)+"\n");

    disco.close();

    return temp;
}

QList<long> *hash::mostrarLista(char *info)
{
    nodoHash temp=search(info);

    if(temp.inicio==-1)
    {
        return 0;
    }

    QList<long> *list=new QList<long>();

    for(int i=0; i<temp.cuantos; i++)
    {
        nodoCol x=lista->at(0,temp.cuantos,temp.inicio);

        if(strcmp(x.info,info)==0)
        {
            for(int j=0; j<x.cuantos; j++)
            {
                nodoH n=lista->lista->at(j,x.cuantos,x.inicio);

                list->append(n.id);
            }
        }
    }

    return list;
}

