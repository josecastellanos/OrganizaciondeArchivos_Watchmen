#include "btree.h"

btree::btree(char *_name)
{
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
    int len=strlen(_name);
    name=new char[len+1];
    memcpy(name,_name,len);
    name[len+1]='\0';
}

void btree::create(int cuantos)
{
    disco.open(name,ios_base::binary | ios_base::in | ios_base::out);

    header h;

    int val = blocksize*8;

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

    disco.close();
}

void btree::add(int id, int rrn)
{
    disco.open(name,ios_base::binary | ios_base::in | ios_base::out);

    header h;

    disco.seekg(0,ios_base::beg);
    disco.read((char*)&h,sizeof(header));

    if(h.cuantos==h.total)
    {
        cout<<"Full";
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
        bool ver=false;
        entry promo;
        int newp;

        if(searchRecursivo(h.raiz,id).id==-1)
        {
            addRecursiva(mapabits, h.total, h.raiz, id, rrn, ver, promo, newp);
        }
        //si existe agregarlo(Falta Funca Agregar), si no Return;
        // el Search Me gusto este mira http://cs-netlab-01.lynchburg.edu/courses/DataStII/BTreeOps.htm
    }

    // si llego aqui se agrego
    disco.seekg(0,ios_base::beg);
    disco.read((char*)&h,sizeof(header));

    h.cuantos++;

    // termino Escribamos header y mB
    disco.seekp(0,ios_base::beg);
    disco.write((const char*)&h,sizeof(header));

    disco.seekp(blocksize,ios_base::beg);
    disco.write((const char*)mapabits,h.total/8);

    disco.close();
}

void btree::addRecursiva(unsigned char *m, int cuantos, int pos, int id, int rrn, bool &ver, entry &promo, int &newp)
{
    nodoB temp;

    disco.seekg(pos*blocksize, ios_base::beg);
    disco.read((char *)&temp, sizeof(nodoB));

    if(temp.cuantos < maxLlaves)
    {
        if(ver==true)
        {            
            int i;

            for(i=0; i<temp.cuantos; i++)
            {
                if(id < temp.llavesPrimarias[i].id)
                {
                    break;
                }
            }

            for(int j=(temp.cuantos-1); j>=i; j--)
            {
                temp.llavesPrimarias[j+1].id=temp.llavesPrimarias[j].id;
                temp.llavesPrimarias[j+1].rrn=temp.llavesPrimarias[j+1].rrn;
                temp.apuntadores[j+2]=temp.apuntadores[j+1];
            }

            temp.llavesPrimarias[i].id=id;
            temp.llavesPrimarias[i].rrn=rrn;
            temp.apuntadores[i+1]=newp;
        }
        else
        {
            temp.llavesPrimarias[temp.cuantos].id=id;
            temp.llavesPrimarias[temp.cuantos].rrn=rrn;

            sort(temp.llavesPrimarias,temp.cuantos);
        }

        temp.cuantos++;

        ver=false;
    }
    else if(temp.cuantos==maxLlaves)
    {
        int i;

        for(i=0; i<temp.cuantos; i++)
        {
            if(id < temp.llavesPrimarias[i].id)
            {
                break;
            }
        }

        int j=(temp.cuantos+1)/2;

        int x;

        entry arr[maxLlaves+1];

        for(int y=0; y<maxApuntadores ; y++)
        {
            if(y==i)
                continue;

            arr[y].id = temp.llavesPrimarias[y].id;
            arr[y].rrn = temp.llavesPrimarias[y].rrn;
        }

        arr[i].id=id;
        arr[i].rrn=rrn;

        entry e=arr[j];

        entry *a=temp.llavesPrimarias;
        int *b=temp.apuntadores;

        clear(temp.llavesPrimarias, temp.apuntadores);

        for(x=0; x<j; x++)
        {
            temp.llavesPrimarias[x].id=arr[x].id;
            temp.llavesPrimarias[x].rrn=arr[x].rrn;

            for(int y=0; y<maxLlaves; y++)
            {
                if(a[y].id==arr[x].id)
                {
                    if(y==0 && x==0)
                    {
                        temp.apuntadores[y]=b[y];
                    }
                    temp.apuntadores[y+1]=b[y+1];

                    break;
                }
            }
        }

        temp.cuantos=j;

        mapabits map;

        int d=map.freeblock(m,cuantos);
        map.killblock(m,d,cuantos);

        header h;

        disco.seekg(0,ios_base::beg);
        disco.read((char*)&h,sizeof(header));

        if(h.raiz==pos)
        {
            int c=map.freeblock(m,cuantos);
            map.killblock(m,c,cuantos);

            h.raiz=c;

            addRecursiva(m,cuantos,c,e.id,e.rrn,ver,promo,newp);

            nodoB tempp;

            disco.seekg(c*blocksize, ios_base::beg);
            disco.read((char *)&tempp, sizeof(nodoB));

            tempp.apuntadores[0]=pos;
            tempp.apuntadores[1]=d;

            disco.seekp(0, ios_base::beg);
            disco.write((const char *)&h, sizeof(header));

            disco.seekp(c*blocksize, ios_base::beg);
            disco.write((const char *)&tempp, sizeof(nodoB));
        }

        for(x=j+1; x<=maxLlaves; x++)
        {
            addRecursiva(m,cuantos,d,arr[x].id,arr[x].rrn,ver,promo,newp);
        }

        promo=e;
        newp=d;

        ver=true;
    }
    else
    {
        int k = 0 ;

        while(k < temp.cuantos && id > temp.llavesPrimarias[k].id)
        {
            k++;
        }

        if(id > temp.llavesPrimarias[k-1].id)
        {
            k--;
        }

        if(id < temp.llavesPrimarias[k].id)
        {
            if(temp.apuntadores[k]!=-1)
            {
                addRecursiva(m,cuantos,temp.apuntadores[k],id,rrn,ver,promo,newp);
            }
            else
            {
                mapabits map;

                int i=map.freeblock(m,cuantos);
                map.killblock(m,i,cuantos);

                temp.apuntadores[k]=i;

                addRecursiva(m,cuantos,i,id,rrn,ver,promo,newp);
            }

            if(ver==true)
            {
                addRecursiva(m,cuantos,pos,promo.id,promo.rrn,ver,promo,newp);
            }
        }
        else if(id > temp.llavesPrimarias[k].id)
        {
            if(temp.apuntadores[k+1]!=-1)
            {
                addRecursiva(m,cuantos,temp.apuntadores[k+1],id,rrn,ver,promo,newp);
            }
            else
            {
                mapabits map;

                int i=map.freeblock(m,cuantos);
                map.killblock(m,i,cuantos);

                temp.apuntadores[k+1]=i;

                addRecursiva(m,cuantos,i,id,rrn,ver,promo,newp);
            }

            if(ver==true)
            {
                addRecursiva(m,cuantos,pos,promo.id,promo.rrn,ver,promo,newp);
            }
        }
    }

    disco.seekp(pos*blocksize, ios_base::beg);
    disco.write((const char *)&temp, sizeof(nodoB));
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

void btree::mostrar()
{
    disco.open(name,ios::binary | ios::in | ios::out);

    cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;

    header h;

    disco.seekg(0,ios_base::beg);
    disco.read((char *)&h,sizeof(header));

    int base=1+(h.total/8)/blocksize;

    nodoB temp;

    for(int i=0; i<25; i++)
    {
        disco.seekg((base+i)*blocksize, ios_base::beg);
        disco.read((char *)&temp, sizeof(nodoB));

        int j;

        for(j=0; j<maxLlaves; j++)
        {
            cout<<"LlavesPrimarias["<<j<<"].id: "<<temp.llavesPrimarias[j].id<<endl;
            cout<<"LlavesPrimarias["<<j<<"].rrn: "<<temp.llavesPrimarias[j].rrn<<endl;
        }


        for(j=0; j<maxLlaves; j++)
        {
            cout<<"Apuntadores["<<j<<"]: "<<temp.apuntadores[j]<<endl;
        }

        cout<<"Cuantos: "<<temp.cuantos<<endl;
        cout<<"----------------------------------"<<endl;
    }

    disco.close();
}

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

void btree::sort(entry *arr, int fin)
{
    for(int i=0; i<(fin-1); i++)
    {
        entry ii=arr[i];
        for(int j=i+1; j<fin; j++)
        {
            entry jj=arr[j];
            if(arr[i].id > arr[j].id)
            {
                arr[i]=jj;
                arr[j]=ii;
            }
        }
    }
}

entry btree::searchRecursivo(int pos, int id)
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

    if(id > temp.llavesPrimarias[i].id)
    {
        i++;
    }

    if(temp.apuntadores[i]!=-1)
    {
        return searchRecursivo(temp.apuntadores[i],id);
    }

    entry a;
    return a;
}
