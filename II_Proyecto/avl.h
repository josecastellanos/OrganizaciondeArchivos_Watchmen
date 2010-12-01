#ifndef AVL_H
#define AVL_H

#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include "avllist.h"

using namespace std;

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
            strcpy(protocolo,"");
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
        char protocolo[20];
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

const int bs=sizeof(nodo_avl);

class avl
{
    public:
        avl(char *_name, QTextEdit *log);
        void create(int cuantos);
        void add(char *, long id);
        nodo_avl search(char *protocolo);
        QList<long>* mostrarLista(char *protocolo);
        void deleteId(char *protocolo, long id);

    private:
        void addRecursiva(fstream *disco, int i, int pos, char *protocolo, long id);
        nodo_avl searchRecursiva(int n, char *protocolo);
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
        int dondevapunto(char *a);
        fstream disco;
        char *name;        
        avlList *lista;
        QTextEdit *log;
};

#endif // AVL_H
