#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>

int counter=0;
int ok=0;

void HandleUSR1(int sig)
{
	char buffer[4096];

	struct stat buf;

	/*
	// checking if the filename is regular.
	if(lstat(file,&buf)<0)
	{
		printf("Error stat \n");
		exit(4);
	}

	if(!S_ISREG(buf.st_mode))
	{
		printf("%s is not reg file \n",file);
		exit(5);
	}*/



//creating the symlink
// number the symlinks and print them. 
// ok=1;
}

void HandleWait(int sig)
{
	int status;
	wait(&status);
	if(WIFEXITED(status))
	{
		printf("Exit status: %d \n", WEXITSTATUS(status));
		exit(4);
	}
}
int main(int argc, char *argv[])
{
	
	if(argc!=3)
	{
		printf("Usage ./exec word file n");
		exit(-1);
	}

	char string[100];

	strcpy(string,argv[1]);
	//printf("%s \n",string);

	for(int i=0;i<strlen(string);i++)
	{
		if(!isalnum(string[i]))
		{
			printf("The word must contain only chars and numbers");
			exit(-1);
		}
	}

	int number;

	number=atoi(argv[2]);

	if(!(number<=200 && number>=100))
	{
		printf("The number must be betweeen 100 and 200");
		exit(-1);
	}
	//printf("%d \n", number);

	pid_t pid;
	//int status;
	struct sigaction Signal;

	char *constructed;
	FILE* file;

	file=fopen("file.txt","w");

	constructed=(char*)malloc(number*sizeof(char));
	int n=number;
	if((pid=fork())<0)
	{
		printf("The program didn;t create successfully the child");
		exit(-1);
	}
	if(pid==0)
	{
		//Signal.sa_handler=SendSignalToParent;
		
		//printf("%d", getpid());

		for(int i=0; i<strlen(string);i++)
		{
			if(isdigit(string[i]))
			{
				counter++;
				kill(getppid(),SIGUSR1);
			}
		}

		// should have printed with a write with a 4096 buffer size
		for(int i=0;i<n;i++)
		{
			//write(file,buff,nbytes);
			// buffer should be the block composed of all the strings
			// with strcpy could have been composed.
			// nbytes 4096,
			// file should be a char and we could strcpy the file into the buffer.
			// each time we put the string into the buffer we give him memory with realloc
			// and then we put the string into the buffer. The buffer will be printed.
			fprintf(file, "%s",string );
		}

		exit(5);
	}

	struct sigaction Signal1;
	struct sigaction Signal2;

	Signal1.sa_handler=HandleUSR1;
	Signal1.sa_flags=0;
	sigaction(SIGUSR1,&Signal1,NULL);

	Signal2.sa_handler=HandleWait;
	Signal2.sa_flags=0;
	sigaction(SIGCHLD,&Signal2,NULL);

	while(ok)
	{
		continue;
	}

	return 0;
}
