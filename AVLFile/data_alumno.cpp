#include "data_alumno.h"

data_alumno::data_alumno(char *_name)
{
    disco.open(_name,ios::binary | ios::in);
    if(!disco)
    {
        validoCrear=true;
        validoLeer=false;
        disco.open(_name,ios::binary | ios::out);
    }
    else
    {        
        validoCrear=false;
        validoLeer=true;
        disco.open(_name,ios::binary | ios::in | ios::out);
    }
    validoUpdate=false;
    validoEscribir=false;
    char *temp=(char *)malloc(8);
    strcpy(temp,"avl.dat");
    strcpy(name,_name);
    llavesPrimarias=new avl(temp);
}

void data_alumno::create(int cuantos)
{
    if(validoCrear)
    {
        int header=1;

        unsigned char bloque[sizeof(alumno)];
        memset(bloque,0,sizeof(alumno));

        disco.seekp(0,ios_base::beg);
        disco.write((const char *)bloque,sizeof(alumno));

        disco.seekp(0,ios_base::beg);
        disco.write((const char*)&header,sizeof(int));

        disco.seekp(sizeof(alumno),ios_base::beg);

        alumno temp;

        for(int i=2; i<=(cuantos+1); i++)
        {
            if(i>cuantos)
            {
                temp.nextfree=-1;
            }
            else
            {
                temp.nextfree=i;
            }
            disco.write((const char*)&temp,sizeof(alumno));
            disco.flush();
        }

        disco.close();
        disco.open(name,ios::binary | ios::in | ios::out);

        llavesPrimarias->create(cuantos);

        validoCrear=false;
        validoEscribir=true;
        validoLeer=true;
    }
}

void data_alumno::close()
{
    disco.close();
}

int data_alumno::freepos()
{
    if(validoLeer)
    {
        disco.seekg(0,ios_base::beg);

        int header;

        disco.read((char *)&header,sizeof(int));

        validoUpdate=true;
        validoLeer=false;

        return header;
    }
    return -1;
}

void data_alumno::updateHeader(int value)
{
    if(value!=-1 && validoUpdate)
    {
        alumno temp;

        disco.seekg(value*sizeof(alumno),ios_base::beg);
        disco.read((char *)&temp,sizeof(alumno));

        disco.seekp(0,ios_base::beg);
        disco.write((const char *)&temp.nextfree,sizeof(int));

        validoUpdate=false;
        validoLeer=true;

        disco.flush();
    }
}

void data_alumno::add(int cuenta, char *nombre, char *carrera)
{
    if(!validoEscribir)
    {
        return;
    }

    int pos=freepos();
    updateHeader(pos);

    alumno temp;

    disco.seekg(pos*sizeof(alumno),ios_base::beg);
    disco.read((char *)&temp,sizeof(alumno));

    strcpy(temp.nombre,nombre);
    strcpy(temp.carrera,carrera);

    disco.seekp(pos*sizeof(alumno),ios_base::beg);
    disco.write((const char *)&temp,sizeof(alumno));

    llavesPrimarias->add(cuenta,0);
}
