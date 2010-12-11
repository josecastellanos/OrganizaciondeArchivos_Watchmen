#include "btree.h"

btree::btree(char *_name, QTextEdit *log)
{
    this->log=log;

    disco.open(_name,ios_base::binary | ios_base::in);
    if(!disco)
    {
        disco.open(_name,ios_base::binary | ios_base::out);
    }
    else
    {
        disco.open(_name, ios_base::binary | ios_base::in | ios_base::out);
    }
    disco.close();

    name=QString(_name);

    log->append("- btree.h: "+name+" ha sido abierto\n");
}

void btree::create(int cuantos)
{
    disco.open(name.toStdString().c_str(),ios_base::binary | ios_base::in | ios_base::out);

    header h;

    int val = blocksize*8;    

    cuantos=cuantos/maxLlaves+cuantos%maxLlaves;

    if(cuantos%val!=0)
    {
        cuantos+=val-cuantos%val;
    }

    h.total = cuantos;

    unsigned char bloque[blocksize];
    memset(bloque,0,blocksize);

    disco.seekg(0,ios_base::beg);//para los bloques
    disco.write((const char*)&bloque,blocksize);

    disco.seekp(0,ios_base::beg);//del header
    disco.write((const char*)&h,sizeof(header));

    unsigned char *mapabits;
    mapabits = (unsigned char *)malloc(h.total/8);
    memset(mapabits,0,h.total/8);

    int final=1+(h.total/8)/blocksize;

    for(int i=0; i<final; i++)
    {
        int c = i / 8 ;
        mapabits[c] = mapabits[c] | (1 << (7-i%8));
    }

    disco.seekp(blocksize,ios_base::beg);//set bits
    disco.write((const char*)mapabits,h.total/8);

    nodoB temp;

    for(int i=0; i<h.total; i++)
    {
        disco.write((const char*)&temp,sizeof(nodoB));
        disco.flush();
    }

    log->append("- btree.h: "+QString(name)+" se ha creado con "+QString::number(cuantos)+" bloques\n");

    free(mapabits);

    disco.close();
}

void btree::add(long id, int rrn)
{
    disco.open(name.toStdString().c_str(),ios_base::binary | ios_base::in | ios_base::out);

    header h;

    disco.seekg(0,ios_base::beg);
    disco.read((char*)&h,sizeof(header));

    if(h.cuantos==(h.total*256))
    {
        log->append("- btree.h: No hay espacio para "+QString::number(id)+"\n");
        disco.close();
        return;
    }

    mapabits map;

    unsigned char *mapabits;
    mapabits = (unsigned char*)malloc(h.total/8);
    memset(mapabits,0,h.total/8);

    disco.seekg(blocksize,ios_base::beg);
    disco.read((char*)mapabits,h.total/8);

    if(h.raiz==-1)//Primera vez no hay Raiz
    {
        int pos = map.freeblock(mapabits,h.total);
        map.killblock(mapabits,pos,h.total);

        nodoB temp;

        h.raiz = pos;
        temp.llavesPrimarias[0].id = id;
        temp.llavesPrimarias[0].rrn = rrn;
        temp.cuantos++;

        disco.seekp(pos*blocksize,ios_base::beg);
        disco.write((const char*)&temp,sizeof(nodoB));

        disco.seekp(0,ios_base::beg);
        disco.write((const char*)&h,sizeof(header));
    }
    else
    {
        entry promo;
        int newp;

        if(searchRecursivo(h.raiz,id).id==-1)
        {
            if(addRecursiva(mapabits,h.total,h.raiz,id,rrn,promo,newp)==1)
            {
                int c=map.freeblock(mapabits,h.total);
                map.killblock(mapabits,c,h.total);

                nodoB tempp;

                tempp.llavesPrimarias[0].id=promo.id;
                tempp.llavesPrimarias[0].rrn=promo.rrn;

                tempp.apuntadores[0]=h.raiz;
                tempp.apuntadores[1]=newp;

                tempp.cuantos=1;

                h.raiz=c;

                disco.seekp(c*blocksize, ios_base::beg);
                disco.write((const char *)&tempp, sizeof(nodoB));
            }
        }
        else
        {
            log->append("- btree.h: Ya existe "+QString::number(id)+" en el arbol B\n");
            disco.close();
            return;
        }
        //si existe agregarlo(Falta Funca Agregar), si no Return;
        // el Search Me gusto este mira http://cs-netlab-01.lynchburg.edu/courses/DataStII/BTreeOps.htm
    }

    // si llego aqui se agrego
    h.cuantos++;

    // termino Escribamos header y mB
    disco.seekp(0,ios_base::beg);
    disco.write((const char*)&h,sizeof(header));

    disco.seekp(blocksize,ios_base::beg);
    disco.write((const char*)mapabits,h.total/8);

    free(mapabits);

    log->append("- btree.h: "+QString::number(id)+" ha sido agregado\n");

    disco.close();
}

int btree::addRecursiva(unsigned char *m, int cuantos, int pos, long id, int rrn, entry &promo, int &newp)
{
    int i=0;
    nodoB temp;

    if(pos==-1)
    {
        promo.id=id;
        promo.rrn=rrn;
        newp=-1;
        return 1;
    }
    else
    {
        disco.seekg(pos*blocksize, ios_base::beg);
        disco.read((char *)&temp, sizeof(nodoB));

        while(i < temp.cuantos && id > temp.llavesPrimarias[i].id)
        {
            i++;
        }

//        if(i!=0)
//        {
//            if(temp.llavesPrimarias[i-1].id < id)
//            {
//                i++;
//            }
//        }
    }

    entry e;
    int pright;

    int r=addRecursiva(m,cuantos,temp.apuntadores[i],id,rrn,e,pright);

    if(r==0)
    {
        return r;
    }
    else if(temp.cuantos < maxLlaves)
    {
        int n;

        for(n=0; n<temp.cuantos; n++)
        {
            if(e.id < temp.llavesPrimarias[n].id)
            {
                break;
            }
        }

        for(int j=(temp.cuantos-1); j>=n; j--)
        {
            temp.llavesPrimarias[j+1].id=temp.llavesPrimarias[j].id;
            temp.llavesPrimarias[j+1].rrn=temp.llavesPrimarias[j].rrn;
            temp.apuntadores[j+2]=temp.apuntadores[j+1];
        }

        temp.llavesPrimarias[n].id=e.id;
        temp.llavesPrimarias[n].rrn=e.rrn;
        temp.apuntadores[i+1]=pright;

        temp.cuantos++;

        disco.seekp(pos*blocksize, ios_base::beg);
        disco.write((const char *)&temp, sizeof(nodoB));

        return 0;
    }
    else
    {
        nodoB newtemp;

        split(m,cuantos,e,pright,temp,promo,newp,newtemp);

        disco.seekp(pos*blocksize, ios_base::beg);
        disco.write((const char *)&temp, sizeof(nodoB));

        disco.seekp(newp*blocksize, ios_base::beg);
        disco.write((const char *)&newtemp, sizeof(nodoB));

        return 1;
    }
}

void btree::split(unsigned char *m, int cuantos, entry key, int rrn, nodoB &temp, entry &promo, int &newp, nodoB &newtemp)
{
    int i;

    for(i=0; i<temp.cuantos; i++)
    {
        if(key.id < temp.llavesPrimarias[i].id)
        {
            break;
        }
    }

    int j=(temp.cuantos+1)/2;

    int x;

    entry arr[maxLlaves+1];
    int ap[maxApuntadores+1];

    for(int y=0; y<i; y++)
    {
        arr[y].id = temp.llavesPrimarias[y].id;
        arr[y].rrn = temp.llavesPrimarias[y].rrn;

        ap[y]=temp.apuntadores[y];
    }

    ap[i]=temp.apuntadores[i];

    for(int y=i; y<maxLlaves; y++)
    {
        arr[y+1].id = temp.llavesPrimarias[y].id;
        arr[y+1].rrn = temp.llavesPrimarias[y].rrn;

        ap[y+2]=temp.apuntadores[y+1];
    }

    arr[i].id=key.id;
    arr[i].rrn=key.rrn;
    ap[i+1]=rrn;

    promo.id=arr[j].id;
    promo.rrn=arr[j].rrn;

    mapabits map;

    int d=map.freeblock(m,cuantos);
    map.killblock(m,d,cuantos);

    newp=d;

    clear(temp.llavesPrimarias, temp.apuntadores);

    for(x=0; x<j; x++)
    {
        temp.llavesPrimarias[x].id=arr[x].id;
        temp.llavesPrimarias[x].rrn=arr[x].rrn;

        temp.apuntadores[x]=ap[x];
    }

    temp.apuntadores[j]=ap[j];

    temp.cuantos=j;

    int k=0;

    for(x=j+1; x<=maxLlaves; x++)
    {
        newtemp.llavesPrimarias[k].id=arr[x].id;
        newtemp.llavesPrimarias[k].rrn=arr[x].rrn;

        newtemp.apuntadores[k]=ap[x];

        k++;
    }

    newtemp.apuntadores[k]=ap[maxApuntadores];

    newtemp.cuantos=k;
}

//void btree::split2(nodoB &temp, unsigned char *m, int cuantos, entry &e, int &ap)
//{
//    int i;

//    for(i=0; i<temp.cuantos; i++)
//    {
//        if(id < temp.llavesPrimarias[i].id)
//        {
//            break;
//        }
//    }

//    int j=(temp.cuantos+1)/2;

//    int x;

//    entry arr[maxLlaves+1];

//    for(int y=0; y<maxLlaves+1 ; y++)
//    {
//        if(y==i)
//            continue;

//        arr[y].id = temp.llavesPrimarias[y].id;
//        arr[y].rrn = temp.llavesPrimarias[y].rrn;
//    }

//    arr[i].id=id;
//    arr[i].rrn=rrn;

//    entry *a=temp.llavesPrimarias;
//    int *b=temp.apuntadores;
//    temp.cuantos=1;

//    clear(temp.llavesPrimarias, temp.apuntadores);

//    temp.llavesPrimarias[0].id=arr[j].id;
//    temp.llavesPrimarias[0].rrn=arr[j].rrn;

//    mapabits map;

//    int c=map.freeblock(m,cuantos);
//    map.killblock(m,c,cuantos);
//    int d=map.freeblock(m,cuantos);
//    map.killblock(m,d,cuantos);

//    temp.apuntadores[0]=c;
//    temp.apuntadores[1]=d;

//    for(x=0; x<j ; x++)
//    {
//        addRecursiva(m,cuantos,c,arr[x].id,arr[x].rrn);
//    }

//    for(x=j+1; x<temp.cuantos; x++)
//    {
//        addRecursiva(m,cuantos,d,arr[x].id,arr[x].rrn);
//    }
//}

//void btree::split(nodoB &temp, unsigned char *m, int cuantos, int id, int rrn)
//{
//    int i;

//    for(i=0; i<temp.cuantos; i++)
//    {
//        if(id < temp.llavesPrimarias[i].id)
//        {
//            break;
//        }
//    }

//    int j=(temp.cuantos+1)/2;

//    int x;

//    entry arr[maxLlaves+1];

//    for(int y=0; y<maxLlaves+1 ; y++)
//    {
//        if(y==i)
//            continue;

//        arr[y].id = temp.llavesPrimarias[y].id;
//        arr[y].rrn = temp.llavesPrimarias[y].rrn;
//    }

//    arr[i].id=id;
//    arr[i].rrn=rrn;

//    entry *a=temp.llavesPrimarias;
//    int *b=temp.apuntadores;
//    temp.cuantos=1;

//    clear(temp.llavesPrimarias, temp.apuntadores);

//    temp.llavesPrimarias[0].id=arr[j].id;
//    temp.llavesPrimarias[0].rrn=arr[j].rrn;

//    mapabits map;

//    int c=map.freeblock(m,cuantos);
//    map.killblock(m,c,cuantos);
//    int d=map.freeblock(m,cuantos);
//    map.killblock(m,d,cuantos);

//    temp.apuntadores[0]=c;
//    temp.apuntadores[1]=d;

//    for(x=0; x<j ; x++)
//    {
//        addRecursiva(m,cuantos,c,arr[x].id,arr[x].rrn);
//    }

//    for(x=j+1; x<temp.cuantos; x++)
//    {
//        addRecursiva(m,cuantos,d,arr[x].id,arr[x].rrn);
//    }
//}

//void btree::mostrar()
//{
//    disco.open(name,ios::binary | ios::in | ios::out);

//    cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;

//    header h;

//    disco.seekg(0,ios_base::beg);
//    disco.read((char *)&h,sizeof(header));

//    int base=1+(h.total/8)/blocksize;

//    nodoB temp;

//    for(int i=0; i<25; i++)
//    {
//        cout<<"Pos: "<<base+i<<endl;

//        disco.seekg((base+i)*blocksize, ios_base::beg);
//        disco.read((char *)&temp, sizeof(nodoB));

//        int j;

//        for(j=0; j<maxLlaves; j++)
//        {
//            cout<<"LlavesPrimarias["<<j<<"].id: "<<temp.llavesPrimarias[j].id<<endl;
//            cout<<"LlavesPrimarias["<<j<<"].rrn: "<<temp.llavesPrimarias[j].rrn<<endl;
//        }


//        for(j=0; j<maxApuntadores; j++)
//        {
//            cout<<"Apuntadores["<<j<<"]: "<<temp.apuntadores[j]<<endl;
//        }

//        cout<<"Cuantos: "<<temp.cuantos<<endl;
//        cout<<"----------------------------------"<<endl;
//    }

//    disco.close();
//}

void btree::clear(entry *a, int *b)
{
    for(int i=0; i<maxApuntadores; i++)
    {
        if(i<maxLlaves)
        {
            a[i].id=-1;
            a[i].rrn=-1;
        }
        b[i]=-1;
    }
}

entry btree::search(long id)
{
    disco.open(name.toStdString().c_str(),ios_base::binary | ios_base::in | ios_base::out);

    header h;

    disco.seekg(0,ios_base::beg);
    disco.read((char*)&h,sizeof(header));

    entry temp=searchRecursivo(h.raiz,id);

    disco.close();

    log->append("- btree.h: Se ha buscado a "+QString::number(id)+"\n");

    return temp;
}

entry btree::searchRecursivo(int pos, long id)
{
    nodoB temp;

    disco.seekg(pos*blocksize,ios_base::beg);
    disco.read((char*)&temp,sizeof(nodoB));

    for(int i=0; i<temp.cuantos; i++)
    {
        if(temp.llavesPrimarias[i].id==id)
        {
            return temp.llavesPrimarias[i];
        }
    }

    int i=0;

    while(i < temp.cuantos && id > temp.llavesPrimarias[i].id)
    {
        i++;
    }

//    if(i!=0)
//    {
//        if(temp.llavesPrimarias[i-1].id < id)
//        {
//            i++;
//        }
//    }

    if(temp.apuntadores[i]!=-1)
    {
        return searchRecursivo(temp.apuntadores[i],id);
    }

    entry a;
    return a;
}
