#ifndef DATA_ALUMNO_H
#define DATA_ALUMNO_H

#include "avl.h"

class alumno
{
    public :
        alumno()
        {
            cuenta=0;
            strcpy(nombre,"");
            strcpy(carrera,"");
            nextfree=0;
        }
        int cuenta;
        char nombre[60];
        char carrera[5];
        int nextfree;
};

class data_alumno
{
    public:
        data_alumno(char *name);
        void create(int cuantos);
        void close();
        int freepos();
        void updateHeader(int value);
        void add(int cuenta, char *nombre, char *carrera);
        fstream disco;
        char *name;
        avl *llavesPrimarias;
        bool validoCrear, validoLeer, validoUpdate, validoEscribir;
};

#endif // DATA_ALUMNO_H
