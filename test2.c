#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
int ok=1;
void HandleWait(int sig)
{
	int status,w;
	w=wait(&status);
	if(WIFEXITED(status))
	{
		printf("The child process with the pid %d ended witht the code %d \n",w,WEXITSTATUS(status));
	}
	ok=0;
}

int main(int argc, char *argv[])
{
	if(argc!=3)
	{
		fprintf(stderr,"usage /execfile file instruciton");
		exit(EXIT_FAILURE);
	}

	int fd1[2];
	int fd2[2];

	if(pipe(fd1)==-1)
	{
		perror("problem at pipe 1 \n");
		exit(EXIT_FAILURE);
	}

	if(pipe(fd2)==-1)
	{
		perror("problem at pipe 2 \n");
		exit(EXIT_FAILURE);
	}

	int counterFinal=0;
	
	int pid1;
	if((pid1=fork())<0)/*first child*/
	{
		perror("problem at pid");
		exit(EXIT_FAILURE);
	}	
	else
	{

		if(pid1==0)
		{
			close(fd2[0]);
			close(fd1[0]);
			close(fd2[1]);
			dup2(fd1[1],1);

			execlp("cat","cat",argv[1],(char*)NULL);

			close(fd1[1]);
			exit(1);

		}
	}

	int pid2;
	if((pid2=fork())<0)/*second child*/
	{
		perror("problem at pid");
		exit(EXIT_FAILURE);
	}	
	else
	{

		if(pid2==0)
		{		
			close(fd1[1]);
			close(fd2[0]);
			dup2(fd1[0],0);
			dup2(fd2[1],1);
			char *Sndarg=strcat("^",argv[2]);// not sure if it actually works
			// trebuia poate sa fii pus un char steluta[10]="^" si la ceasta sa fac cat cu argv[2];

			execlp("grep","grep","-E",Sndarg,(char*)NULL);

			close(fd1[0]);
			close(fd2[1]);
			exit(2);
		}
	}

	int pid3;
	if((pid3=fork())<0)
	{
		perror("problem at pid");
		exit(EXIT_FAILURE);
	}	
	else
	{
		if(pid3==0)
		{
			close(fd1[0]);
			close(fd1[1]);
			close(fd2[1]);
			dup2(fd2[0],0);
			char buffer[2];
			int counterLeft=0,counterRight=0;
			while(read(fd2[0],&buffer,1))
			{
				if(*buffer=='(')
				{
					counterLeft++;
				}
				if(*buffer==')')
				{
					counterRight++;
				}
				if(*buffer=='\n')
				{
					counterFinal++;
					if(counterRight==counterLeft)
					{
						kill(getppid(),SIGUSR1);
					}
					counterRight=0;
					counterLeft=0;
				}
			}

			close(fd2[0]);

			exit(counterFinal);
		}
	}

	/*cod parinte*/

	struct sigaction Sig;
	Sig.sa_handler=HandleWait;
	Sig.sa_flags=0;
	sigaction(SIGCHLD,&Sig,NULL);


	int file=open(argv[1], O_RDONLY);

	struct stat buf;
	if(stat(argv[1],&buf)==-1)
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	if(!S_ISREG(buf.st_mode))
	{
		fprintf(stderr, "problem at regular file");
		exit(EXIT_FAILURE);
	}


	while(ok)
	{
		continue;
	}



	close(file);

	return 0;
}
