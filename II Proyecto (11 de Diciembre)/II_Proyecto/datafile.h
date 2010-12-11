#ifndef DATAFILE_H
#define DATAFILE_H

#include "avl.h"
#include "btree.h"
#include "QString"
#include "QStringList"
#include <QTableWidget>
#include "hash.h"

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
        //void deleteRecord(long int id);
        void searchAVL(char *protocolo, QTableWidget *tb);
        void searchHashDestino(char *destino, QTableWidget *tb);
        void searchHashSource(char *source, QTableWidget *tb);

    private:        
        void search(char *info, QList<long> *list, QTableWidget *tb);
        fstream disco;
        QString name;
        btree *arbolB;
        avl *arbolAVL;
        hash *hashDestino;
        hash *hashSource;
        QTextEdit *log;

};

#endif // DATAFILE_H
