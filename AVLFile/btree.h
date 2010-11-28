#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <QtAlgorithms>

#include "avl.h"

using namespace std;

const int maxLlaves = 4;
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
    btree(char *_name);
    void create(int cuantos);
    void add(long id, int rrn);
    int addRecursiva(unsigned char *m, int cuantos, int pos, long id, int rrn, entry &promo, int &newp);
    entry searchRecursivo(int pos, long id);
    void mostrar();
    void prueba();

private:
    void split(unsigned char *m, int cuantos, entry key, int rrn, nodoB &temp, entry &promo, int &newp, nodoB &newtemp);
    void sort(entry *arr, int fin);
    void clear(entry *a, int *b);
    fstream disco;
    char *name;
    avl *arbol;
};

#endif // BTREE_H
