#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main ()
{
int i;

	for(i =0; i<10; i++)
	{
	printf("No, I am Batman \n");
	sleep(1);
	}


return 0;
}

