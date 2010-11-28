#ifndef AVL_H
#define AVL_H

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include "avllist.h"

using namespace std;

const int bs=52;

class header
{
    public:
        header()
        {
            raiz=-1;
            cuantos=0;
            total=0;
        }
        int raiz;
        int cuantos;
        int total;
};

class nodo_avl
{
    public:
        nodo_avl()
        {
            strcpy(source,"");
            hijoIzq=-1;
            hijoDer=-1;
            padre=-1;
            altura=-1;
            FE=0;
            pos=-1;
            inicio=-1;
            final=-1;
            cuantos=0;
        }
        char source[15];
        int inicio;
        int final;
        int cuantos;
        int hijoIzq;
        int hijoDer;
        int padre;
        int altura;
        short FE;
        int pos;
};

class avl
{
    public:
        avl(char *_name);
        void create(int cuantos);
        void add(char *source, long id, int rrn);
        nodo_avl search(char *source);
        void mostrar();

    private:
        void addRecursiva(fstream *disco, int i, int pos, char *source, long id, int rrn);
        nodo_avl searchRecursiva(int n, char *source);
        void postOrdenAltura(int n);
        int altura(int pos);
        int updateAltura(fstream *disco, int pos);
        int max(int i, int j);
        void preOrdenFE(int n);
        void updateFE(nodo_avl *temp, int hi, int hd);
        bool esBalanceado(int pos);
        void balancear(int pos);
        void RSI(int n);
        void RSD(int n);
        void RDI(int n);
        void RDD(int n);
        void deleteKey(int cuenta);
        void deleteKeyRecursiva(nodo_avl n);
        nodo_avl getMayor(int n);
        nodo_avl getMenor(int n);
        fstream disco;
        char *name;        
        avlList *lista;
};

#endif // AVL_H
