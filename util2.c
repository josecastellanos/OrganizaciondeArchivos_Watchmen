#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
	int cuenta;
	char nombre[20];
	char carrera[4];
}alumno;

void main(int argc, char *argv[])
{
	FILE *archivo;
	if(strcmp(argv[1],"-c")==0 && argc==4)
	{
		if((archivo=fopen(argv[3],"wb"))==NULL)
		{
			printf("No se puede abrir el archivo \n");
			exit(1);
		}
		
		int header=0;
		
		fwrite(&header, sizeof(int), 1, archivo);

		alumno temp;
		
		temp.cuenta=0;
		strcpy(temp.nombre,"");
		strcpy(temp.carrera,"");
		
		int fin=atoi(argv[2]);
		int i;
		
		for(i=0; i<fin; i++)
		{
			fwrite(&temp, sizeof(alumno), 1, archivo);
		}

		fclose(archivo);
	}
	if(strcmp(argv[1],"-a")==0 && argc==6)
	{
		if((archivo=fopen(argv[5],"rb+"))==NULL)
		{
			printf("No se puede abrir el archivo \n");
			exit(1);
		}
		
		int valueheader;
		
		fread(&valueheader, sizeof(int), 1, archivo);

		alumno temp;
		
		temp.cuenta=atoi(argv[2]);
		strcpy(temp.nombre,argv[3]);
		strcpy(temp.carrera,argv[4]);
		
		int pos=sizeof(int)+valueheader*sizeof(alumno);
		
		fseek(archivo, pos, 0);
		
		fwrite(&temp, sizeof(alumno), 1, archivo);

		valueheader++;
		
		fseek(archivo,0,0);
		
		fwrite(&valueheader, sizeof(int), 1, archivo);

		fclose(archivo);	
	}
	if(strcmp(argv[1],"-l")==0 && argc==3)
	{
		if((archivo=fopen(argv[2],"rb"))==NULL)
		{
			printf("No se puede abrir el archivo \n");
			exit(1);
		}
		
		int valueheader;
		
		fread(&valueheader, sizeof(int), 1, archivo);

		int i;

		for(i=0; i<valueheader; i++)
		{			
			alumno temp;
		
			fread(&temp, sizeof(alumno), 1, archivo);

			printf("Cuenta: %d \n", temp.cuenta);
			printf("Nombre: %s \n", temp.nombre);
			printf("Carrera: %s \n", temp.carrera);
			printf("--------------------------\n");			
		}
		
		fclose(archivo);	
	}
	if(strcmp(argv[1],"-q")==0 && argc==4)
	{
		if((archivo=fopen(argv[3],"rb"))==NULL)
		{
			printf("No se puede abrir el archivo \n");
			exit(1);
		}

		int pos=sizeof(int)+atoi(argv[2])*sizeof(alumno);		

		fseek(archivo, pos, 0);

		alumno temp;

		fread(&temp, sizeof(alumno), 1, archivo);

		printf("Cuenta: %d \n", temp.cuenta);
		printf("Nombre: %s \n", temp.nombre);
		printf("Carrera: %s \n", temp.carrera);
		
		fclose(archivo);	
	}
	if(strcmp(argv[1],"-d")==0 && argc==4)
	{
		if((archivo=fopen(argv[3],"rb+"))==NULL)
		{
			printf("No se puede abrir el archivo \n");
			exit(1);
		}

		int valueheader;
		
		fread(&valueheader, sizeof(int), 1, archivo);

		valueheader--;

		int arg=atoi(argv[2]);
		int pos=sizeof(int)+arg*sizeof(alumno);

		fseek(archivo, pos, 0);		

		int i;
	
		for(i=arg; i<valueheader; i++)
		{
			pos=sizeof(int)+(i+1)*sizeof(alumno);

			fseek(archivo, pos, 0);

			alumno temp;

			fread(&temp, sizeof(alumno), 1, archivo);

			pos-=sizeof(alumno);

			fseek(archivo, pos, 0);

			fwrite(&temp, sizeof(alumno), 1, archivo);	
		}		

		fseek(archivo, 0, 0);
		
		fwrite(&valueheader, sizeof(int), 1, archivo);		
		
		fclose(archivo);	
	}
}
