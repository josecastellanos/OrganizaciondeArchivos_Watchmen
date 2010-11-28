#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{
	char *a;
	a=(char *)malloc(sizeof(char)*10);

	strcpy(a,"dagoberto");
	
	char *b;
	b=(char *)malloc(sizeof(char)*(10+9));
	
	strcpy(b,a);

	strncat(b,"btree.dat",9);

	printf("%s \n",b);
}
