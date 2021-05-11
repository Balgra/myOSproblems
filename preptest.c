#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

// showing everything in a dir
/*
int main(int argc, char *argv[])
{
	DIR *dp;
	struct dirent *dirp;

	if(argc!=2)
	{
		fprintf(stderr,"gib directory");
		exit(EXIT_FAILURE);
	}

	if((dp=opendir(argv[1]))==NULL)
	{
		perror("can't open dir");
		exit(EXIT_FAILURE);
	}


	while((dirp=readdir(dp))!=NULL)
	{
		printf("%s \n",dirp->d_name);
	}

	closedir(dp);

	return 0;
}*/


/*
int main()
{
	char buf[4096];
	pid_t pid;
	int status;

	printf("%% ");

	while(fgets(buf,4096,stdin)!= NULL)
	{
		if(buf[strlen(buf)-1]=='\n')
		{
			buf[strlen(buf) - 1] = 0;
		}

		if((pid=fork()) < 0)
		{
			perror("error at pid");
		}
		else
		{
			if(pid==0)
			{
				execlp(buf,buf,(char *) 0);
				fprintf(stderr,"couldtn't exec %s: \n",buf);
				exit(127);
			}
		}

		if((pid=waitpid(pid,&status,0))<0)
		{
			perror("error at waitpid");
		}
		printf("%% ");
	}
	exit(0);
}*/

/*
int globvar = 6;
char buf[] = "a write to stdout\n";
int ok=1;
int main()
{
		int var; 
		pid_t pid;
		var = 88;
		if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
			perror("write error");

		printf("before fork\n"); // we don’t flush stdout 

		if ((pid = fork()) < 0) 
		{
			perror("fork error");
		} 		
		else 
		{
			if (pid == 0)  //child
			{ //child 
				globvar++; // modify variables
				var++;
			} 
			else 
			{
				printf("mumu\n");
				//sleep(2); // parent 
			}
		}
		printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,var);
		exit(0);
}*/

int main()
{

	int n;
	int fd[2];
	pid_t pid;
	char line[100];
	if (pipe(fd) < 0)
	{
	perror("pipe error");
	}
	if ((pid = fork()) < 0)
	{
	perror("fork error");
	} 
	else 
	{
		if (pid > 0) 
		{ /* parent */
			close(fd[0]);
			write(fd[1], "hello world\n", 12);
		} 
		else 
		{ /* child */
			close(fd[1]);
			n = read(fd[0], line, 100);
			write(STDOUT_FILENO, line, n);
			execlp("bash","bash","run.sh",NULL);
			exit(0);
		}
	}

	int status;
    pid_t result = wait(&status);
 
    printf("\nPID: %d, code; %d \n", result, WEXITSTATUS(status));

	exit(0);
}