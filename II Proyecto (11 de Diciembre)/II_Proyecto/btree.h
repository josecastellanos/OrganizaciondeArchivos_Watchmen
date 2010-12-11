#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "avl.h"
#include <QTextEdit>

using namespace std;

const int maxLlaves = 256;
const int maxApuntadores = maxLlaves + 1;

class entry
{
    public:
        entry()
        {
            id=-1;
            rrn=-1;
        }
        long id;
        int rrn;// protocolo
};

class nodoB
{
public:
    nodoB()
    {
        for(int i=0; i<maxApuntadores; i++)
        {
            if(i<maxLlaves)
            {
                llavesPrimarias[i].id=-1;
                llavesPrimarias[i].rrn=-1;
            }
            apuntadores[i]=-1;
        }
        cuantos=0;
    }
    entry llavesPrimarias[maxLlaves];
    int apuntadores[maxApuntadores];
    int cuantos;
};

const int blocksize=sizeof(nodoB);

class btree
{
public:
    btree(char *_name, QTextEdit *log);
    void create(int cuantos);
    void add(long id, int rrn);
    int addRecursiva(unsigned char *m, int cuantos, int pos, long id, int rrn, entry &promo, int &newp);
    entry search(long id);

private:    
    entry searchRecursivo(int pos, long id);
    void split(unsigned char *m, int cuantos, entry key, int rrn, nodoB &temp, entry &promo, int &newp, nodoB &newtemp);
    void clear(entry *a, int *b);
    fstream disco;
    QString name;
    QTextEdit *log;

};

#endif // BTREE_H
