#include <stdio.h>
#include <string.h>

char look;

void readch()
{
	look = getchar();
}

void err(char* string)
{
	printf(strcat('Error: ', string));
	printf("\n");
}

void expected(char* string)
{
	err(strcat(string, ' Expected'));
}

void match(char x)
{
	if(strcmp(look, x))
	{
		readch();
	}
	else
	{
		err("Expected different character");
	}
	
}