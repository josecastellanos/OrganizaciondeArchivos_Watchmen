#include "avl.h"

avl::avl(char *_name, QTextEdit *log)
{
    this->log=log;

    disco.open(_name,ios::binary | ios::in);
    if(!disco)
    {
        disco.open(_name,ios::binary | ios::out);
    }
    else
    {
        disco.open(_name,ios::binary | ios::in | ios::out);
    }
    disco.close();

    name=QString(_name);

    log->append("- avl.h: "+name+" ha sido abierto\n");

    int n=name.indexOf('.',0,Qt::CaseSensitive);

    QString avlistname=name.mid(0,n);

    avlistname.append("List.dat");

    lista = new avlList((char *)avlistname.toUtf8().constData(),log);
}

void avl::mostrar()
{
    header h;

    disco.open((char *)name.toUtf8().constData(),ios::binary | ios::in | ios::out);

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&h,sizeof(header));

    int ini=1+(h.total/8)/bs;
    int fin=ini+h.cuantos;

    for(int i=ini; i<fin; i++)
    {
        nodo_avl n;

        disco.seekg(i*bs,ios_base::beg);
        disco.read((char *)&n,sizeof(nodo_avl));

        log->append("Protocolo: "+QString(n.protocolo));
        log->append("HijoIzq: "+QString::number(n.hijoIzq));
        log->append("HijoDer: "+QString::number(n.hijoDer));
        log->append("Altura: "+QString::number(n.altura));
        log->append("Padre: "+QString::number(n.padre));
        log->append("FE: "+QString::number(n.FE));
        log->append("Pos: "+QString::number(n.pos));
        log->append("-------------------------------------------");
    }

    disco.close();
}

void avl::create(int cuantos)
{
    disco.open((char *)name.toUtf8().constData(),ios::binary | ios::in | ios::out);

        header h;

        int val=bs*8;

        if(cuantos%val!=0)
        {
            cuantos+=val-cuantos%val;
        }

        h.total=cuantos;

        unsigned char bloque[bs];
        memset(bloque,0,bs);

        disco.seekp(0,ios_base::beg);
        disco.write((const char *)bloque,bs);

        disco.seekp(0,ios_base::beg);
        disco.write((const char*)&h,sizeof(header));

        unsigned char *mapabits;
        mapabits = (unsigned char *)malloc(h.total/8);
        memset(mapabits,0,h.total/8);

        int fin=1+(h.total/8)/bs;

        for(int i=0; i<fin; i++)
        {
            int casilla = i / 8 ;
            mapabits[casilla] = mapabits[casilla] | (1 << (7-i%8));
        }

        disco.seekp(bs,ios_base::beg);
        disco.write((const char *)mapabits,h.total/8);

        for(int i=0; i<h.total; i++)
        {
            disco.write((const char*)bloque,bs);
            disco.flush();
        }

        log->append("- avl.h: "+name+" se ha creado con "+QString::number(cuantos)+" bloques\n");

        lista->create(cuantos);

        free(mapabits);

        disco.close();
}

void avl::add(char *protocolo, long id)
{
    disco.open((char *)name.toUtf8().constData(),ios::binary | ios::in | ios::out);

    header h;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&h,sizeof(header));

    if(h.cuantos==h.total)
    {
        log->append("- avl.h: No hay espacio disponible\n");
        disco.close();
        return;
    }

    mapabits map;

    unsigned char *mapabits;
    mapabits = (unsigned char *)malloc(h.total/8);
    memset(mapabits,0,h.total/8);

    disco.seekg(bs,ios_base::beg);
    disco.read((char *)mapabits,h.total/8);

    int pos;

    if(h.raiz==-1)
    {
        pos=map.freeblock(mapabits,h.total);
        map.killblock(mapabits,pos,h.total);

        nodo_avl temp;

        h.raiz=pos;
        strcpy(temp.protocolo,protocolo);
        temp.FE=0;//Preguntar
        temp.altura=0;//Preguntar
        temp.pos=pos;

        lista->add(id,temp.cuantos,temp.inicio,temp.final);

        disco.seekp(pos*bs,ios_base::beg);
        disco.write((const char *)&temp,sizeof(nodo_avl));

        h.cuantos++;
    }
    else
    {
        nodo_avl temp=searchRecursiva(h.raiz,protocolo);

        if(temp.pos==-1)
        {            
            pos=map.freeblock(mapabits,h.total);
            map.killblock(mapabits,pos,h.total);

            addRecursiva(&disco,pos,h.raiz,protocolo,id);

            h.cuantos++;
        }
        else
        {
            lista->add(id,temp.cuantos,temp.inicio,temp.final);

            disco.seekp(temp.pos*bs,ios_base::beg);
            disco.write((const char *)&temp,sizeof(nodo_avl));
        }
    }

    disco.seekp(0,ios_base::beg);
    disco.write((const char *)&h,sizeof(header));

    disco.seekp(bs,ios_base::beg);
    disco.write((const char *)mapabits,h.total/8);

    free(mapabits);

    postOrdenAltura(h.raiz);
    preOrdenFE(h.raiz);
    balancear(h.raiz);

    log->append("- avl.h: "+QString(protocolo)+" ha sido agregado\n");

    disco.close();
}

void avl::addRecursiva(fstream *disco, int i, int pos, char *protocolo, long id)
{
    nodo_avl temp;

    disco->seekg(pos*bs,ios_base::beg);
    disco->read((char *)&temp,sizeof(nodo_avl));

    if(strcmp(protocolo,temp.protocolo)<0)
    {
        if(temp.hijoIzq==-1)
        {
            nodo_avl temp2;

            temp.hijoIzq=i;
            strcpy(temp2.protocolo,protocolo);
            temp2.padre=pos;
            temp2.pos=i;

            lista->add(id,temp2.cuantos,temp2.inicio,temp2.final);

            disco->seekp(i*bs,ios_base::beg);
            disco->write((const char *)&temp2,sizeof(nodo_avl));

            disco->seekp(pos*bs,ios_base::beg);
            disco->write((const char *)&temp,sizeof(nodo_avl));
        }
        else
        {
            addRecursiva(disco,i,temp.hijoIzq,protocolo,id);
        }
    }
    else if(strcmp(protocolo,temp.protocolo)>0)
    {
        if(temp.hijoDer==-1)
        {            
            nodo_avl temp2;

            temp.hijoDer=i;
            strcpy(temp2.protocolo,protocolo);
            temp2.padre=pos;
            temp2.pos=i;

            lista->add(id,temp2.cuantos,temp2.inicio,temp2.final);

            disco->seekp(i*bs,ios_base::beg);
            disco->write((const char *)&temp2,sizeof(nodo_avl));

            disco->seekp(pos*bs,ios_base::beg);
            disco->write((const char *)&temp,sizeof(nodo_avl));
        }
        else
        {
            addRecursiva(disco,i,temp.hijoDer,protocolo,id);
        }
    }
    else
    {
        return;
    }
}

bool avl::esBalanceado(int pos)
{
    if(pos!=-1)
    {
        nodo_avl temp;

        disco.seekg(pos*bs,ios_base::beg);
        disco.read((char *)&temp,sizeof(nodo_avl));

        if(temp.FE < -1 || temp.FE > 1)
        {
            return false;
        }
        else
        {
            bool i = esBalanceado(temp.hijoIzq);
            bool d = esBalanceado(temp.hijoDer);

            if((i==true) && (d==true))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

void avl::RSD(int n)
{
    nodo_avl nodo;

    disco.seekg(n*bs,ios_base::beg);
    disco.read((char *)&nodo,sizeof(nodo_avl));

    if(nodo.padre!=-1)
    {
        nodo_avl pad;

        disco.seekg(nodo.padre*bs,ios_base::beg);
        disco.read((char *)&pad,sizeof(nodo_avl));

        if(pad.hijoDer==n)
        {
            pad.hijoDer=nodo.hijoIzq;
        }
        else
        {
            pad.hijoIzq=nodo.hijoIzq;
        }

        nodo_avl izq;

        disco.seekg(nodo.hijoIzq*bs,ios_base::beg);
        disco.read((char *)&izq,sizeof(nodo_avl));

        int d=nodo.hijoIzq;
        int e=nodo.padre;
        int f=izq.hijoDer;

        izq.hijoDer=n;
        izq.padre=nodo.padre;
        nodo.padre=nodo.hijoIzq;

        if(f!=-1)
        {
            nodo_avl temp;

            disco.seekg(f*bs,ios_base::beg);
            disco.read((char *)&temp,sizeof(nodo_avl));

            temp.padre=n;

            disco.seekp(f*bs,ios_base::beg);
            disco.write((const char *)&temp,sizeof(nodo_avl));
        }

        nodo.hijoIzq=f;
        /*
        else
        {
            nodo_avl a;

            disco.seekg(izq.hijoDer*bs,ios_base::beg);
            disco.read((char *)&a,sizeof(nodo_avl));

            int b=nodo.hijoIzq;
            int c=izq.hijoDer;
            nodo.hijoIzq=izq.hijoDer;
            a.padre=n;
            izq.hijoDer=n;
            izq.padre=nodo.padre;
            nodo.padre=b;

            disco.seekp(c*bs,ios_base::beg);
            disco.write((const char *)&a,sizeof(nodo_avl));
        }*/

        disco.seekp(n*bs,ios_base::beg);
        disco.write((const char *)&nodo,sizeof(nodo_avl));

        disco.seekp(e*bs,ios_base::beg);
        disco.write((const char *)&pad,sizeof(nodo_avl));

        disco.seekp(d*bs,ios_base::beg);
        disco.write((const char *)&izq,sizeof(nodo_avl));
    }
    else
    {
        header h;

        disco.seekg(0,ios_base::beg);
        disco.read((char *)&h,sizeof(header));

        h.raiz=nodo.hijoIzq;

        nodo_avl izq;

        disco.seekg(nodo.hijoIzq*bs,ios_base::beg);
        disco.read((char *)&izq,sizeof(nodo_avl));

        int d=nodo.hijoIzq;
        int e=izq.hijoDer;

        izq.hijoDer=n;
        nodo.padre=nodo.hijoIzq;

        if(e!=-1)
        {
            nodo_avl temp;

            disco.seekg(e*bs,ios_base::beg);
            disco.read((char *)&temp,sizeof(nodo_avl));

            temp.padre=n;

            disco.seekp(e*bs,ios_base::beg);
            disco.write((const char *)&temp,sizeof(nodo_avl));
        }

        nodo.hijoIzq=e;
        /*
        else
        {
            nodo_avl a;

            disco.seekg(izq.hijoDer*bs,ios_base::beg);
            disco.read((char *)&a,sizeof(nodo_avl));

            int b=nodo.hijoIzq;
            int c=izq.hijoDer;
            nodo.hijoIzq=izq.hijoDer;
            a.padre=n;
            izq.hijoDer=n;
            nodo.padre=b;

            disco.seekp(c*bs,ios_base::beg);
            disco.write((const char *)&a,sizeof(nodo_avl));
        }*/
        izq.padre=-1;        

        disco.seekp(n*bs,ios_base::beg);
        disco.write((const char *)&nodo,sizeof(nodo_avl));

        disco.seekp(0,ios_base::beg);
        disco.write((const char *)&h,sizeof(header));

        disco.seekp(d*bs,ios_base::beg);
        disco.write((const char *)&izq,sizeof(nodo_avl));
    }
}

void avl::RSI(int n)
{
    nodo_avl nodo;

    disco.seekg(n*bs,ios_base::beg);
    disco.read((char *)&nodo,sizeof(nodo_avl));

    if(nodo.padre!=-1)
    {
        nodo_avl pad;

        disco.seekg(nodo.padre*bs,ios_base::beg);
        disco.read((char *)&pad,sizeof(nodo_avl));

        if(pad.hijoIzq==n)
        {
            pad.hijoIzq=nodo.hijoDer;
        }
        else
        {
            pad.hijoDer=nodo.hijoDer;
        }

        nodo_avl der;

        disco.seekg(nodo.hijoDer*bs,ios_base::beg);
        disco.read((char *)&der,sizeof(nodo_avl));

        int d=nodo.hijoDer;
        int e=nodo.padre;
        int f=der.hijoIzq;

        der.hijoIzq=n;
        der.padre=nodo.padre;
        nodo.padre=nodo.hijoDer;

        if(f!=-1)
        {
            nodo_avl temp;

            disco.seekg(f*bs,ios_base::beg);
            disco.read((char *)&temp, sizeof(nodo_avl));

            temp.padre=n;

            disco.seekp(f*bs,ios_base::beg);
            disco.write((const char *)&temp,sizeof(nodo_avl));
        }

        nodo.hijoDer=f;
        /*
        else
        {
            nodo_avl a;

            disco.seekg(der.hijoIzq*bs,ios_base::beg);
            disco.read((char *)&a,sizeof(nodo_avl));

            int b=nodo.hijoDer;
            int c=der.hijoIzq;
            nodo.hijoDer=der.hijoIzq;
            a.padre=n;
            der.hijoIzq=n;
            der.padre=nodo.padre;
            nodo.padre=b;

            disco.seekp(c*bs,ios_base::beg);
            disco.write((const char *)&a,sizeof(nodo_avl));
        }*/

        disco.seekp(n*bs,ios_base::beg);
        disco.write((const char *)&nodo,sizeof(nodo_avl));

        disco.seekp(e*bs,ios_base::beg);
        disco.write((const char *)&pad,sizeof(nodo_avl));

        disco.seekp(d*bs,ios_base::beg);
        disco.write((const char *)&der,sizeof(nodo_avl));
    }
    else
    {
        header h;

        disco.seekg(0,ios_base::beg);
        disco.read((char *)&h,sizeof(header));

        h.raiz=nodo.hijoDer;

        nodo_avl der;

        disco.seekg(nodo.hijoDer*bs,ios_base::beg);
        disco.read((char *)&der,sizeof(nodo_avl));

        int d=nodo.hijoDer;
        int e=der.hijoIzq;

        der.hijoIzq=n;
        nodo.padre=nodo.hijoDer;

        if(e!=-1)
        {
            nodo_avl temp;

            disco.seekg(e*bs,ios_base::beg);
            disco.read((char *)&temp,sizeof(nodo_avl));

            temp.padre=n;

            disco.seekp(e*bs,ios_base::beg);
            disco.write((const char *)&temp,sizeof(nodo_avl));
        }

        nodo.hijoDer=e;
        /*
        else
        {
            nodo_avl a;

            disco.seekg(der.hijoIzq*bs,ios_base::beg);
            disco.read((char *)&a,sizeof(nodo_avl));

            int b=nodo.hijoDer;
            int c=der.hijoIzq;
            nodo.hijoDer=der.hijoIzq;
            a.padre=n;
            der.hijoIzq=n;
            nodo.padre=b;

            disco.seekp(c*bs,ios_base::beg);
            disco.write((const char *)&a,sizeof(nodo_avl));
        }*/
        der.padre=-1;

        disco.seekp(n*bs,ios_base::beg);
        disco.write((const char *)&nodo,sizeof(nodo_avl));

        disco.seekp(0,ios_base::beg);
        disco.write((const char *)&h,sizeof(header));

        disco.seekp(d*bs,ios_base::beg);
        disco.write((const char *)&der,sizeof(nodo_avl));
    }
}

void avl::RDI(int n)
{
    nodo_avl nodo;

    disco.seekg(n*bs,ios_base::beg);
    disco.read((char *)&nodo,sizeof(nodo_avl));

    RSD(nodo.hijoDer);
    RSI(n);
}

void avl::RDD(int n)
{
    nodo_avl nodo;

    disco.seekg(n*bs,ios_base::beg);
    disco.read((char *)&nodo,sizeof(nodo_avl));

    RSI(nodo.hijoIzq);
    RSD(n);
}

void avl::balancear(int pos)
{
    if(pos!=-1)
    {
        nodo_avl temp;

        disco.seekg(pos*bs,ios_base::beg);
        disco.read((char *)&temp,sizeof(nodo_avl));

        short fe = temp.FE;

        switch(fe)
        {
            case -2:
                if(esBalanceado(temp.hijoDer))
                {
                    nodo_avl der;

                    disco.seekg(temp.hijoDer*bs,ios_base::beg);
                    disco.read((char *)&der,sizeof(nodo_avl));

                    if(der.FE==-1)
                    {
                        RSI(pos);
                    }
                    else
                    {
                        if(der.FE==1)
                        {
                            RDI(pos);
                        }
                        else
                        {
                            balancear(temp.hijoDer);
                        }
                    }
                }
                else
                {
                    balancear(temp.hijoDer);
                }
                break;

            case 2:
                if(esBalanceado(temp.hijoIzq))
                {
                    nodo_avl izq;

                    disco.seekg(temp.hijoIzq*bs,ios_base::beg);
                    disco.read((char *)&izq,sizeof(nodo_avl));

                    if(izq.FE==1)
                    {
                        RSD(pos);
                    }
                    else
                    {
                        if(izq.FE==-1)
                        {
                            RDD(pos);
                        }
                        else
                        {
                            balancear(temp.hijoIzq);
                        }
                    }
                }
                else
                {
                    balancear(temp.hijoIzq);
                }
                break;

            default:
                balancear(temp.hijoIzq);
                balancear(temp.hijoDer);
                break;
        }
    }
}

void avl::preOrdenFE(int n)
{
    if(n==-1)
    {
        return;
    }
    else
    {
        nodo_avl temp;

        disco.seekg(n*bs,ios_base::beg);
        disco.read((char *)&temp,sizeof(nodo_avl));

        updateFE(&temp,temp.hijoIzq,temp.hijoDer);

        disco.seekp(n*bs,ios_base::beg);
        disco.write((const char *)&temp,sizeof(nodo_avl));

        preOrdenFE(temp.hijoIzq);
        preOrdenFE(temp.hijoDer);
    }
}

void avl::updateFE(nodo_avl *temp, int hi, int hd)
{
    nodo_avl hizq, hder;
    int a=-1, b=-1;
    if(hi!=-1)
    {
       disco.seekg(hi*bs,ios_base::beg);
       disco.read((char *)&hizq,sizeof(nodo_avl));

       a=hizq.altura;
    }
    if(hd!=-1)
    {
        disco.seekg(hd*bs,ios_base::beg);
        disco.read((char *)&hder,sizeof(nodo_avl));

        b=hder.altura;
    }
    temp->FE=a-b;
}

void avl::postOrdenAltura(int n)
{    
    nodo_avl temp;

    if(n==-1)
    {
        return;
    }
    else
    {
        disco.seekg(n*bs,ios_base::beg);
        disco.read((char *)&temp,sizeof(nodo_avl));

        postOrdenAltura(temp.hijoIzq);
        postOrdenAltura(temp.hijoDer);

        temp.altura=max(altura(temp.hijoIzq),altura(temp.hijoDer))+1;

        disco.seekp(n*bs,ios_base::beg);
        disco.write((const char *)&temp,sizeof(nodo_avl));
    }
}

bool avl::es_vacio(nodo_avl *n)
{
    if(n->hijoDer==-1 && n->hijoIzq==-1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int avl::altura(int pos)
{
    if(pos==-1)
    {
        return -1;
    }
    else
    {
        nodo_avl temp;

        disco.seekg(pos*bs,ios_base::beg);
        disco.read((char *)&temp,sizeof(nodo_avl));

        return temp.altura;
    }
}

//int avl::updateAltura(fstream *disco,int pos)
//{
//    if(pos==-1)
//    {
//        return 0;
//    }
//    else
//    {
//        nodo_avl temp;

//        disco->seekg(pos*bs,ios_base::beg);
//        disco->read((char *)&temp,sizeof(nodo_avl));

//        int i=updateAltura(disco,temp.hijoIzq);
//        int j=updateAltura(disco,temp.hijoDer);

//        temp.altura=max(i,j) + 1;

//        disco->seekp(pos*bs,ios_base::beg);
//        disco->write((const char *)&temp,sizeof(nodo_avl));

//        return temp.altura;
//    }
//}

int avl::max(int i, int j)
{
    if(i>=j)
    {
        return i;
    }
    else
    {
        return j;
    }
}

QList<long>* avl::mostrarLista(char *protocolo)
{
    nodo_avl temp=search(protocolo);

    if(temp.pos==-1)
    {
        return 0;
    }

    QList<long> *list=new QList<long>();

    for(int i=0; i<temp.cuantos; i++)
    {
        nodo n=lista->at(i,temp.cuantos,temp.inicio);

        list->append(n.id);
    }

    return list;
}

//void avl::deleteId(char *protocolo, long id)
//{
//    disco.open((char *)name.toUtf8().constData(),ios::binary | ios::in | ios::out);

//    header h;

//    disco.seekg(0,ios_base::beg);
//    disco.read((char *)&h,sizeof(header));

//    nodo_avl temp=searchRecursiva(h.raiz,protocolo);

//    if(temp.pos==-1)
//    {
//        disco.close();

//        return;
//    }

//    for(int i=0; i<temp.cuantos; i++)
//    {
//        nodo n=lista->at(i,temp.cuantos,temp.inicio);

//        if(n.id==id)
//        {
//            lista->deleteRrn(i,temp.cuantos,temp.inicio,temp.final);

//            break;
//        }
//    }

//    disco.seekp(temp.pos*bs,ios_base::beg);
//    disco.write((const char *)&temp,sizeof(nodo_avl));

//    disco.close();
//}

nodo_avl avl::search(char *protocolo)
{
    disco.open((char *)name.toUtf8().constData(),ios::binary | ios::in | ios::out);

    header h;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&h,sizeof(header));

    nodo_avl temp=searchRecursiva(h.raiz,protocolo);

    log->append("- avl.h: Se ha buscado a "+QString(protocolo)+"\n");

    disco.close();

    return temp;
}

nodo_avl avl::searchRecursiva(int n, char *protocolo)
{
    nodo_avl nodo;

    if(n==-1)
    {
        return nodo;
    }

    disco.seekg(n*bs,ios_base::beg);
    disco.read((char *)&nodo,sizeof(nodo_avl));

    if(strcmp(nodo.protocolo,protocolo)!=0)
    {
        if(strcmp(protocolo,nodo.protocolo)<0)
        {
           return searchRecursiva(nodo.hijoIzq,protocolo);
        }
        else if(strcmp(protocolo,nodo.protocolo)>0)
        {
           return searchRecursiva(nodo.hijoDer,protocolo);
        }
    }

    return nodo;
}

//void avl::deleteKey(int cuenta)
//{
//    disco.open(name,ios::binary | ios::in | ios::out);

//    nodo_avl n=search(cuenta);

//    header h;

//    disco.seekg(0,ios_base::beg);
//    disco.read((char *)&h,sizeof(header));

//    if(n.pos==h.raiz && h.cuantos==1)//Poner pos en ves de nextfre-1
//    {
//        h.raiz=-1;
//    }
//    else
//    {
//        if(n.pos!=-1)
//        {
//            deleteKeyRecursiva(n);
//        }
//    }

//    h.cuantos--;

//    disco.seekp(0,ios_base::beg);
//    disco.write((const char *)&h,sizeof(header));

//    postOrdenAltura(h.raiz);
//    preOrdenFE(h.raiz);
//    balancear(h.raiz);

//    disco.close();
//}

//void avl::deleteKeyRecursiva(nodo_avl n)
//{
//    if(n.hijoDer==-1 && n.hijoIzq==-1)
//    {
//        nodo_avl pad;

//        disco.seekg(n.padre*bs,ios_base::beg);
//        disco.read((char *)&pad,sizeof(nodo_avl));

//        if(pad.hijoDer==n.pos)//Poner pos en ves de nextfree-1
//        {
//            pad.hijoDer=-1;
//        }
//        else
//        {
//            pad.hijoIzq=-1;
//        }

//        disco.seekp(n.padre*bs,ios_base::beg);
//        disco.write((const char *)&pad,sizeof(nodo_avl));
//    }
//    else if(n.hijoIzq!=-1)
//    {
//        nodo_avl otro=getMayor(n.hijoIzq);
//        n.keyCuenta=otro.keyCuenta;

//        disco.seekp(n.pos*bs,ios_base::beg);//Poner pos en ves de nextfree-1
//        disco.write((const char *)&n,sizeof(nodo_avl));

//        deleteKeyRecursiva(otro);
//    }
//    else
//    {
//        nodo_avl otro=getMenor(n.hijoDer);
//        n.keyCuenta=otro.keyCuenta;

//        disco.seekp(n.pos*bs,ios_base::beg);//Poner pos en ves de nextfree-1
//        disco.write((const char *)&n,sizeof(nodo_avl));

//        deleteKeyRecursiva(otro);
//    }
//}

//nodo_avl avl::getMayor(int n)
//{
//    nodo_avl temp;

//    disco.seekg(n*bs,ios_base::beg);
//    disco.read((char *)&temp,sizeof(nodo_avl));

//    if(temp.hijoDer==-1)
//    {
//        return temp;
//    }
//    else
//    {
//        return getMayor(temp.hijoDer);
//    }
//}

//nodo_avl avl::getMenor(int n)
//{
//    nodo_avl temp;

//    disco.seekg(n*bs,ios_base::beg);
//    disco.read((char *)&temp,sizeof(nodo_avl));

//    if(temp.hijoIzq==-1)
//    {
//        return temp;
//    }
//    else
//    {
//        return getMenor(temp.hijoIzq);
//    }
//}
