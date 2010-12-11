#include "datafile.h"

dataFile::dataFile(char *_name, QTextEdit *log)
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

    log->append("- dataFile.h: "+QString(_name)+" ha sido abierto\n");

    name=QString(_name);

    int n=name.indexOf('.',0,Qt::CaseSensitive);

    QString bname=name.mid(0,n);
    QString avlname=name.mid(0,n);
    QString hash1=name.mid(0,n);
    QString hash2=name.mid(0,n);

    bname.append("_btree.dat");
    avlname.append("_avl.dat");
    hash1.append("_hashDestino.dat");
    hash2.append("_hashSource.dat");

    arbolB=new btree((char*)bname.toStdString().c_str(),log);
    arbolAVL=new avl((char*)avlname.toStdString().c_str(),log);
    hashDestino=new hash((char*)hash1.toStdString().c_str(),log);
    hashSource=new hash((char*)hash2.toStdString().c_str(),log);
}


void dataFile::create(int cuantos)//Modificar cuando este lo de hash
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

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

    log->append("- dataFile.h: "+QString(name)+" se ha creado con "+QString::number(cuantos)+" bloques\n");

    arbolB->create(cuantos);
    arbolAVL->create(cuantos);
    hashDestino->create(cuantos);
    hashSource->create(cuantos);

    free(mapabits);

    disco.close();
}

void dataFile::add(long id, double time, char *source, char *destino, char *protocolo, char *info)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

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
    arbolAVL->add(protocolo,id);
    hashDestino->add(destino,id);
    hashSource->add(source,id);

    free(mapabits);

    disco.close();
}

//void dataFile::deleteRecord(long id)
//{
//    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

//    int cuantos;

//    disco.seekg(0,ios_base::beg);
//    disco.read((char *)&cuantos,sizeof(int));

//    mapabits map;

//    unsigned char *mapabits;
//    mapabits = (unsigned char *)malloc(cuantos/8);
//    memset(mapabits,0,cuantos/8);

//    disco.seekg(blocks,ios_base::beg);
//    disco.read((char *)mapabits,cuantos/8);

//    int rrn=arbolB->search(id).rrn;

//    if(rrn==-1)
//    {
//        disco.close();
//        return;
//    }

//    nodo_data temp;

//    disco.seekg(rrn*blocks,ios_base::beg);
//    disco.read((char *)&temp,sizeof(nodo_data));

//    map.killblock(mapabits,rrn,cuantos);

//    disco.seekp(blocks,ios_base::beg);
//    disco.write((const char *)mapabits,cuantos/8);

//    arbolAVL->deleteId(temp.source,id);

//    free(mapabits);

//    disco.close();
//}

void dataFile::search(char *info, QList<long> *list, QTableWidget *tb)
{
    if(list!=0)
    {
        tb->clearContents();

        for(int i=0; i<tb->rowCount();i++)
            tb->removeRow(i);

        for(int i=0; i<list->count(); i++)
        {
            entry e=arbolB->search(list->at(i));

            nodo_data n;

            disco.seekg(e.rrn*blocks,ios_base::beg);
            disco.read((char *)&n, sizeof(nodo_data));

            tb->insertRow(i);
            tb->setItem(i,0,new QTableWidgetItem(QString::number(n.id)));
            tb->setItem(i,1,new QTableWidgetItem(QString::number(n.time)));
            tb->setItem(i,2,new QTableWidgetItem(QString(n.source)));
            tb->setItem(i,3,new QTableWidgetItem(QString(n.destino)));
            tb->setItem(i,4,new QTableWidgetItem(QString(n.protocolo)));
            tb->setItem(i,5,new QTableWidgetItem(QString(n.info)));
        }
    }
    else
    {
        log->append("- dataFile.h: "+QString(info)+" no se ha encontrado\n");
    }
}

void dataFile::searchAVL(char *protocolo, QTableWidget *tb)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    QList<long> *list=arbolAVL->mostrarLista(protocolo);

    search(protocolo,list,tb);

    disco.close();
}

void dataFile::searchHashDestino(char *destino, QTableWidget *tb)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    QList<long> *list=hashDestino->mostrarLista(destino);

    search(destino,list,tb);

    disco.close();
}

void dataFile::searchHashSource(char *source, QTableWidget *tb)
{
    disco.open(name.toStdString().c_str(),ios::binary | ios::in | ios::out);

    QList<long> *list=hashSource->mostrarLista(source);

    search(source,list,tb);

    disco.close();
}
