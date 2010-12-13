#ifndef HASH_H
#define HASH_H

#include "string.h"
#include <fstream>
#include "colissions.h"

using namespace std;

class nodoHash
{
public:
    nodoHash()
    {
        inicio=-1;
        final=-1;
        cuantos=0;
    }

    int inicio;
    int final;
    int cuantos;
};

const int Hsize = sizeof(nodoHash);

class hash
{
public:
    hash(char *_name, QTextEdit *log);
    void create(int cuantos);
    unsigned int makeHash(char *key);
    void add(char *info,long id);
    nodoHash search(char *info);
    QList<long>* mostrarLista(char *info);

private:
    fstream disco;
    QString name;
    Colissions *lista;
    QTextEdit *log;
    int tam;

};



#endif // HASH_H
