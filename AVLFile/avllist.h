#ifndef AVLLIST_H
#define AVLLIST_H

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include "mapabits.h"

using namespace std;

const int bsize=20;

class nodo
{
    public:
        nodo()
        {
            rrn=0;
            siguiente=-1;
            anterior=-1;
            pos=-1;
        }
        int rrn;
        int pos;
        int siguiente;
        int anterior;
};

class avlList
{
    public:
        avlList(char *_name);
        void create(int cuantos);
        void add(int rrn, int &cont, int &raiz, int &tail);
        nodo at(int pos, int &cont, int &raiz);
        void deleteRrn(int rrn, int &cont, int &raiz, int &tail);
        bool exists(int rrn, int &raiz);
        void mostrar();
    private:
        char *name;
        fstream disco;
};

#endif // AVLLIST_H
