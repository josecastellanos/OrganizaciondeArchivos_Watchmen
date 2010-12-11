#ifndef COLISSIONS_H
#define COLISSIONS_H

#include "hashlist.h"

class nodoCol
{
    public:
        nodoCol()
        {
            strcpy(info,"");
            inicio=-1;
            final=-1;
            cuantos=0;
            siguiente=-1;
            anterior=-1;
        }
        char info[15];
        int inicio;
        int final;
        int cuantos;
        int siguiente;
        int anterior;
};

const int Csize = sizeof(nodoCol);

class Colissions
{
public:
    Colissions(char *_name, QTextEdit *log);
    void create(int cuantos);
    void add(char *info, long id, int &cont, int &raiz, int &tail);
    nodoCol at(int pos, int &cont, int &raiz);
    //void deleteRrn(int pos, int &cont, int &raiz, int &tail);
    nodoCol GiveMe(char *info, int &raiz, int &pos);    
    hashList *lista;

private:
    fstream disco;
    QString name;
    QTextEdit *log;
    int tam;

};

#endif // COLISSIONS_H
