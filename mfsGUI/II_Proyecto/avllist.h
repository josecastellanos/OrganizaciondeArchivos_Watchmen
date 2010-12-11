#ifndef AVLLIST_H
#define AVLLIST_H

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include "mapabits.h"
#include <QTextEdit>

using namespace std;

const int bsize=16;

class nodo
{
    public:
        nodo()
        {
            id=0;
            siguiente=-1;
            anterior=-1;
            pos=-1;
        }
        long id;
        int pos;
        int siguiente;
        int anterior;
};

class avlList
{
    public:
        avlList(char *_name, QTextEdit *log);
        void create(int cuantos);
        void add(long id, int &cont, int &raiz, int &tail);
        nodo at(int pos, int &cont, int &raiz);
        void deleteRrn(int pos, int &cont, int &raiz, int &tail);
        bool exists(long id, int &raiz);
        void mostrar();

    private:
        char *name;
        fstream disco;
        QTextEdit *log;
};

#endif // AVLLIST_H
