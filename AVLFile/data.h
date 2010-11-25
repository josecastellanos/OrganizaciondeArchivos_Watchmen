#ifndef DATA_H
#define DATA_H

#include "avl.h"

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

class data
{
    public:
        data(char *_name);
        void create(int cuantos);
        void add(long int id, double time, char *source, char *destino, char *protocolo, char *info);
        void deleteRecord(long int id);
        fstream disco;
        char *name;

};

#endif // DATA_H
