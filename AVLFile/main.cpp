#include "avllist.h"
#include <qalgorithms.h>
#include "avl.h"
#include "btree.h"

class entry_A
{
    public:
        entry_A()
        {
            id=-1;
            rrn=-1;
        }
        long id;
        int rrn;// protocolo
};

int main(int argc, char *argv[])
{
    cout<<sizeof(entry_A);

//    cout<<sizeof(nodo_data)<<endl;
//    int a[3];
//    a[0]=5;
//    a[1]=2;
//    a[2]=4;
//    qSort(&a[0],&a[3]);
//    cout<<a[0]<<endl;
//    cout<<a[1]<<endl;
//    cout<<a[2]<<endl;
//    int b=3;
//    if((b+1)%2!=0)
//    {
//        cout<<(b+1)/2;
//    }
//    else
//    {
//        cout<<(b+1)/2;
//    }
//    int *c=a;
//    for(int i=0; i<3; i++)
//    {
//        cout<<c[i];
//    }
//    if(strcmp(argv[1],"-c")==0 && argc==4)
//    {
//        data_alumno a(argv[3]);
//        a.create(atoi(argv[2]));
//        a.add(80,"Jose","I-1");

//        avl a("avl.dat");
//        a.create(10);
//        a.add("a",5);/*
//        a.add("a",6);
//        a.add("a",9);*/
//        a.add("b",4);
//        a.add("c",3);/*
//        a.add("c",2);*/
//        a.add("d",7);
//        a.add("e",8);
//        /*a.add("f",1);
//        a.add("f",2);*/
//        a.add("f",1);
//        a.add("g",5);
//        a.mostrar();

//        btree a("btree.dat");
//        a.create(25);
//        a.add(3,0);
//        a.add(7,0);
//        a.add(9,0);
//        a.add(23,0);
//        a.add(45,0);
//        a.add(1,0);
//        a.add(5,0);
//        a.add(14,0);
//        a.add(25,0);
//        a.add(24,0);
//        a.add(13,0);
//        a.add(11,0);
//        a.add(8,0);
//        a.add(19,0);
//        a.add(4,0);
//        a.add(31,0);
//        a.add(35,0);
//        a.add(56,0);
//        a.add(2,0);
//        a.add(6,0);
//        a.add(12,0);
//        a.mostrar();
    //}
}
