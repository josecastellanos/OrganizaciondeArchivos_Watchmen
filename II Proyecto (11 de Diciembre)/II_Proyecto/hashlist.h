#ifndef HASHLIST_H
#define HASHLIST_H

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include "mapabits.h"
#include <QTextEdit>

using namespace std;

const int bSize=12;

class nodoH
{
    public:
        nodoH()
        {
            id=0;
            siguiente=-1;
            anterior=-1;
        }
        long id;
        int siguiente;
        int anterior;
};

class hashList
{
    public:
        hashList(char *_name, QTextEdit *log);
        void create(int cuantos);
        void add(long id, int &cont, int &raiz, int &tail);
        nodoH at(int pos, int &cont, int &raiz);
        //void deleteRrn(int pos, int &cont, int &raiz, int &tail);
        bool exists(long id, int &raiz);
        void mostrar();

    private:
        QString name;
        fstream disco;
        QTextEdit *log;
};

#endif // HASHLIST_H
