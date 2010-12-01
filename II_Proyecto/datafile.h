#ifndef DATAFILE_H
#define DATAFILE_H

#include "avl.h"
#include "btree.h"
#include "QString"
#include "QStringList"
#include <QTableWidget>

const int blocks=320;

class nodo_data
{
    public:
        nodo_data()
        {
            id=-1;
            time=-1;
            strcpy(source,"");
            strcpy(destino,"");
            strcpy(protocolo,"");
            strcpy(info,"");
        }
        long int id;
        double time;
        char source[15];
        char destino[15];
        char protocolo[20];
        char info[255];
};

class dataFile
{
    public:
        dataFile(char *_name, QTextEdit *log);
        void create(int cuantos);
        void add(long int id, double time, char *source, char *destino, char *protocolo, char *info);
        void deleteRecord(long int id);
        void search(char *protocolo, QTableWidget *tb);

    private:
        int dondevapunto(char *a);
        fstream disco;
        char *name;
        btree *arbolB;
        avl *arbolAVL;
        QTextEdit *log;

};

#endif // DATAFILE_H
