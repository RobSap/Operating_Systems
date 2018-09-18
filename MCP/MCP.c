#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <proc/readproc.h>

int flag;

void sighandler(int sig)
{
flag =0;
//	printf("\nHandler read");
}


int main (int argc, char **argv)
{
	if(argc !=2)
	{
	perror("Need 2 argc");
	exit(EXIT_FAILURE);
	}

	//Default array sizes
	int bufferdefault;
	bufferdefault = 1024;

	//Array of PID ID's
	pid_t pid[bufferdefault];

	//read program[i] and its args[i] from file;
	FILE *file = fopen(argv[1], "r");

	//File error checking
	if (file==NULL)
	{
		perror("File error");
		fclose(file);
		exit(EXIT_FAILURE);
		
	}
	//read info into buffer
	char*buffer[bufferdefault];

	//Keep track of count = #of programs and arg = # of arguments
	int counter, arguments;
	counter =0;

//	printf("\nAbout to enter while loop");
	
	char blah[bufferdefault];
	
	while(fgets(blah,  bufferdefault-1, file) != NULL)
	{
//		printf("\nI printed program %d: %s",counter, blah);
		arguments = 0;
	
		buffer[arguments] =strtok(blah, "\n ");

		while(buffer[arguments]!=NULL)
		{
			arguments++;
			buffer[arguments] = strtok(NULL, "\n ");
		}

		//Print loop
/*
		int z;
		printf("\nPrint Buffer with args \n");
		for (z=0; z<arguments; z++)
		{
			printf("%s ", buffer[z]);
		}
		
		printf("\nDone with this line an its arguments");
*/
//		printf("\nAbout do fork");
		pid[counter] = fork();
//		printf("\nI did a fork");	

//		If Fork Failed
		if (pid[counter] < 0)
		{
			//Handle the error appropriatley
			//Need some type of wait here
			perror("\n fork error");
			exit(0);
		}

		else if (pid[counter] == 0)
		 {

			int sig;
		//Set the signal handler for sigusrq
		//signal(SIGUSR1, sighandler);
	

		//Set up SIGUSR1
		sigset_t sigset;
		sigemptyset(&sigset);
		sigaddset(&sigset,SIGUSR1);
		sigprocmask(SIG_BLOCK, &sigset, NULL);

		//Some type of wait
	//	int test =sigwait(&sigset, &sig);

		sigwait(&sigset, &sig);
	
	/*	if(test ==0)
		{
		printf("It is waiting");
		}
*/
		//Kill the process

		execvp(buffer[0], buffer);
		//If it wasn't in own function use below
		perror("\nError loading program, will skip");
		exit(0);
		 }

		else // (pid[counter] >0)
		{
			//This is the main process
		//	printf("\nIn MCP - Parent Process");
			
		}

		sleep(1);	
		
//		printf("Stop running process");
		//Stop sigusr1 process
		kill (pid[counter], SIGUSR1);
		kill (pid[counter], SIGSTOP);
			
		//Start it
//		kill(pid[counter],SIGCONT);
			
		counter++;
//		printf("\nwhile iteration %d done", counter);
//		Counts number of programs read

	} // end while loop //Done with part 1 and 2 of read in

//Part 3 - the pause section
int n;
int status;
int loop;
loop =1;
//signal(SIGALRM, sighandler);
	while(loop)
	{

//	printf("Made it here");
		for(n = 0; n < counter; n++)
		{	
//			printf("\nMake it here");
			//WANT TO START PROCESS 
			//Returns a 0 when process is still running....
			// waitpid(pid[n],&status,WNOHANG);
		
			
//			printf("result is %d", result);

			if( waitpid(pid[n],&status,WNOHANG) ==0)
			{
			PROCTAB* proc = openproc(PROC_FILLMEM | PROC_FILLSTAT|PROC_FILLSTATUS  | PROC_PID,pid);
				
			//	printf("\nMAde it here");
			//	printf("\nSig alarm ");



				//Start the process
				kill(pid[n],SIGCONT);
	
				//Run for 1 second
			
				signal(SIGALRM,sighandler);
				alarm(2);	
				flag=1;
				while(flag)
				;
				

			//	pause();

//				printf("Ran through %d times",n);

//				sleep(1);
//				//time is up - stop process
				kill (pid[n], SIGUSR1);
				kill (pid[n], SIGSTOP);
			
				proc_t proc_info;
				memset(&proc_info, 0, sizeof(proc_info));
				printf("\n\tProcess:\tpages\t utime\tstime\ttid \tppid \n");
				while (readproc(proc, &proc_info) != NULL)	
				 {
				if(proc_info.tid==pid[n]) 
				{ printf("%20s:\t%5ld\t%5lld\t%5lld\t%d\t%d\n",
			         proc_info.cmd, proc_info.resident,
     				    proc_info.utime, proc_info.stime,
					proc_info.tid, proc_info.ppid);
				}

				}
				closeproc(proc);
			}//End if
			else
			{ int p;
				loop =0;
				for(p=0; p<counter; p++)
				{
					if(waitpid(pid[p],&status,WNOHANG)==0)
					{	loop=1;
					//	printf("Reset loop");
					} 
				}
			}
			//	sleep(1);
		}//End for loop
	}
int j;
//Wait for all pid id's to finish before main MPC terminates
//This should not be needed, but just in case.
	for(j=0; j < counter; j++)
	{
		int wait;
	wait=waitpid(pid[j],0,0);
		if(wait==0)
			sleep(1);
	}
	
//Close file
	fclose(file);
	
//closeproc(proc);
	printf("\n");
	return 0;
}



/*Notes
//Returns a 0 when process is still running.....
int status;
result = waitpid(pid[n],&status,WNOHANG);




*/
