#include "avl.h"

avl::avl(char *_name)
{
    cout<<sizeof(nodo_avl);

    disco.open(_name,ios::binary | ios::in);
    if(!disco)
    {        /*
        validoCrear=true;
        validoLeer=false;*/
        disco.open(_name,ios::binary | ios::out);
    }
    else
    {/*
        validoCrear=false;
        validoLeer=true;*/
        disco.open(_name,ios::binary | ios::in | ios::out);
    }/*
    validoUpdate=false;
    validoEscribir=false;*/
    disco.close();
    int len=strlen(_name);
    name=new char[len+1];
    memcpy(name,_name,len);
    name[len+1]='\0';
}

void avl::mostrar()
{
    header h;

    disco.open(name,ios::binary | ios::in | ios::out);

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&h,sizeof(header));

    int ini=1+(h.total/8)/bs;
    int fin=ini+h.cuantos;

    for(int i=ini; i<fin; i++)
    {
        nodo_avl n;

        disco.seekg(i*bs,ios_base::beg);
        disco.read((char *)&n,sizeof(nodo_avl));

        cout<<"Source: "<<n.source<<endl;
        cout<<"HijoIzq: "<<n.hijoIzq<<endl;
        cout<<"HijoDer: "<<n.hijoDer<<endl;
        cout<<"Altura: "<<n.altura<<endl;
        cout<<"Padre: "<<n.padre<<endl;
        cout<<"FE: "<<n.FE<<endl;
        cout<<"Pos: "<<n.pos<<endl;
        cout<<"-----------------------------"<<endl;
    }

    disco.close();
}

void avl::create(int cuantos)
{/*
    if(validoCrear)
    {*/
    disco.open(name,ios::binary | ios::in | ios::out);

        header h;

        if(cuantos%416!=0)
        {
            cuantos+=416-cuantos%416;
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

        lista = new avlList("avlList.dat");
        lista->create(cuantos*cuantos);

        disco.close();
        //disco.open(name,ios::binary | ios::in | ios::out);

//        validoCrear=false;
//        validoEscribir=true;
//        validoLeer=true;
    //}
}

void avl::add(char *source, long id, int rrn)
{/*
    if(!validoEscribir)
    {
        return;
    }*/
    disco.open(name,ios::binary | ios::in | ios::out);

    header h;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&h,sizeof(header));

    if(h.cuantos==h.total)
    {
        cout<<"No hay espacio"<<endl;
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
        strcpy(temp.source,source);
        temp.FE=0;//Preguntar
        temp.altura=0;//Preguntar
        temp.pos=pos;

        lista->add(id,rrn,temp.cuantos,temp.inicio,temp.final);

        disco.seekp(pos*bs,ios_base::beg);
        disco.write((const char *)&temp,sizeof(nodo_avl));

        h.cuantos++;
    }
    else
    {
        nodo_avl temp=search(source);

        if(temp.pos==-1)
        {            
            pos=map.freeblock(mapabits,h.total);
            map.killblock(mapabits,pos,h.total);

            addRecursiva(&disco,pos,h.raiz,source,id,rrn);

            h.cuantos++;
        }
        else
        {
            lista->add(id,rrn,temp.cuantos,temp.inicio,temp.final);

            disco.seekp(temp.pos*bs,ios_base::beg);
            disco.write((const char *)&temp,sizeof(nodo_avl));
        }
    }

    disco.seekp(0,ios_base::beg);
    disco.write((const char *)&h,sizeof(header));

    disco.seekp(bs,ios_base::beg);
    disco.write((const char *)mapabits,h.total/8);

    postOrdenAltura(h.raiz);
    preOrdenFE(h.raiz);
    balancear(h.raiz);

    disco.close();
}

void avl::addRecursiva(fstream *disco, int i, int pos, char *source, long id, int rrn)
{
    nodo_avl temp;

    disco->seekg(pos*bs,ios_base::beg);
    disco->read((char *)&temp,sizeof(nodo_avl));

    if(strcmp(source,temp.source)<0)
    {
        if(temp.hijoIzq==-1)
        {
            nodo_avl temp2;

            temp.hijoIzq=i;
            strcpy(temp2.source,source);
            temp2.padre=pos;
            temp2.pos=i;

            lista->add(id,rrn,temp2.cuantos,temp2.inicio,temp2.final);

            disco->seekp(i*bs,ios_base::beg);
            disco->write((const char *)&temp2,sizeof(nodo_avl));

            disco->seekp(pos*bs,ios_base::beg);
            disco->write((const char *)&temp,sizeof(nodo_avl));
        }
        else
        {
            addRecursiva(disco,i,temp.hijoIzq,source,id,rrn);
        }
    }
    else if(strcmp(source,temp.source)>0)
    {
        if(temp.hijoDer==-1)
        {            
            nodo_avl temp2;

            temp.hijoDer=i;
            strcpy(temp2.source,source);
            temp2.padre=pos;
            temp2.pos=i;

            lista->add(id,rrn,temp2.cuantos,temp2.inicio,temp2.final);

            disco->seekp(i*bs,ios_base::beg);
            disco->write((const char *)&temp2,sizeof(nodo_avl));

            disco->seekp(pos*bs,ios_base::beg);
            disco->write((const char *)&temp,sizeof(nodo_avl));
        }
        else
        {
            addRecursiva(disco,i,temp.hijoDer,source,id,rrn);
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

        pad.hijoIzq=nodo.hijoIzq;

        nodo_avl izq;

        disco.seekg(nodo.hijoIzq*bs,ios_base::beg);
        disco.read((char *)&izq,sizeof(nodo_avl));

        int d=nodo.hijoIzq;
        int e=nodo.padre;

        if(izq.hijoDer==-1)
        {
            izq.hijoDer=n;
            izq.padre=nodo.padre;
            nodo.padre=nodo.hijoIzq;
            nodo.hijoIzq=-1;
        }
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
        }

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

        if(izq.hijoDer==-1)
        {
            izq.hijoDer=n;
            nodo.padre=nodo.hijoIzq;
            nodo.hijoIzq=-1;
        }
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
        }

        izq.padre=-1;        

        disco.seekp(n*bs,ios_base::beg);
        disco.write((const char *)&nodo,sizeof(nodo_avl));

        disco.seekp(0,ios_base::beg);
        disco.write((const char *)&h,sizeof(header));

        disco.seekp(d*bs,ios_base::beg);
        disco.write((const char *)&izq,sizeof(nodo_avl));
    }
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

        pad.hijoDer=nodo.hijoDer;

        nodo_avl der;

        disco.seekg(nodo.hijoDer*bs,ios_base::beg);
        disco.read((char *)&der,sizeof(nodo_avl));

        int d=nodo.hijoDer;
        int e=nodo.padre;

        if(der.hijoIzq==-1)
        {
            der.hijoIzq=n;
            der.padre=nodo.padre;
            nodo.padre=nodo.hijoDer;
            nodo.hijoDer=-1;
        }
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
        }

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

        if(der.hijoIzq==-1)
        {
            der.hijoIzq=n;
            nodo.padre=nodo.hijoDer;
            nodo.hijoDer=-1;
        }
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
        }

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

    RSD(nodo.hijoIzq);
    RSI(n);
}

void avl::RDD(int n)
{
    nodo_avl nodo;

    disco.seekg(n*bs,ios_base::beg);
    disco.read((char *)&nodo,sizeof(nodo_avl));

    RSI(nodo.hijoDer);
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
                        RSD(pos);
                    }
                    else
                    {
                        if(der.FE==1)
                        {
                            RDD(pos);
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
                        RSI(pos);
                    }
                    else
                    {
                        if(izq.FE==-1)
                        {
                            RDI(pos);
                        }
                    }
                }
                else
                {
                    balancear(temp.hijoIzq);
                }
                break;

            default:
                balancear(temp.hijoDer);
                balancear(temp.hijoIzq);
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
    if(n==-1)
    {
        return;
    }
    else
    {
        nodo_avl temp;

        disco.seekg(n*bs,ios_base::beg);
        disco.read((char *)&temp,sizeof(nodo_avl));

        postOrdenAltura(temp.hijoIzq);
        postOrdenAltura(temp.hijoDer);

        temp.altura=max(altura(temp.hijoIzq),altura(temp.hijoDer))+1;

        disco.seekp(n*bs,ios_base::beg);
        disco.write((const char *)&temp,sizeof(nodo_avl));
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

nodo_avl avl::search(char *source)
{
    header h;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&h,sizeof(header));

    return searchRecursiva(h.raiz,source);
}

nodo_avl avl::searchRecursiva(int n, char *source)
{
    nodo_avl nodo;

    if(n==-1)
    {
        return nodo;
    }

    disco.seekg(n*bs,ios_base::beg);
    disco.read((char *)&nodo,sizeof(nodo_avl));

    if(strcmp(nodo.source,source)!=0)
    {
        if(strcmp(source,nodo.source)<0)
        {
           return searchRecursiva(nodo.hijoIzq,source);
        }
        else if(strcmp(source,nodo.source)>0)
        {
           return searchRecursiva(nodo.hijoDer,source);
        }
    }
    else
    {
        return nodo;
    }
    //return 0;
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
