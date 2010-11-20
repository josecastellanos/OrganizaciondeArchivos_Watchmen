#include "data_alumno.h"
#include "avllist.h"

int main(int argc, char *argv[])
{
//    if(strcmp(argv[1],"-c")==0 && argc==4)
//    {
//        data_alumno a(argv[3]);
//        a.create(atoi(argv[2]));
//        a.add(80,"Jose","I-1");
        avl a("avl.dat");
        a.create(10);
        a.add(30,5);
        a.add(30,6);
        a.add(30,9);
        a.add(25,4);
        a.add(50,3);
        a.add(50,2);
        a.add(40,7);
        a.add(45,8);
        a.add(28,1);
        a.add(28,2);
        a.add(28,1);
        a.mostrar();
        a.lista->mostrar();
    //}
}
