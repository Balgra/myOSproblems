#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<ctype.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>

int nr;
char *file;
size_t bufsize;
size_t n=0;
char *filenew=NULL;
char c;

void HandleUSR1(int sig)
{

	// tema cu while in loc de 100. cati caracteri decimali pot fi reprezentati pe un int.
	snprintf(filenew,bufsize+1+100,"%s_%ld",file,n);

//	filenew=NULL;

	n++;

	if(symlink(file, filenew) == -1)
	{
		perror("symlink fail");
	}


}

void HandlePID(int sig)
{

	int status;
	pid_t w;
	w = wait(&status);
	if(w == -1)
	{
		perror("SDF");
		exit(EXIT_FAILURE);
	}

	if(WIFEXITED(status))
	{
		printf("child existed with the status %d ",WEXITSTATUS(status));
	}

	printf("status: %d nroflinks %ld \n", WEXITSTATUS(status), n+1);
	//free(filenew);
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		fprintf(stderr,"usage /exefile word file n \n");
		exit(EXIT_FAILURE);
	}

	nr= strtol(argv[3],NULL,10);

	size_t sizeString=strlen(argv[1]);
	printf("sizeString %ld \n",sizeString);
	bufsize=strlen(argv[1]);

	filenew=(char*) malloc((bufsize + 1 + 100) * sizeof(char));

	if(filenew == NULL)
	{
		perror("incorect allocaiton");
		exit(EXIT_FAILURE);
	}

	file=argv[1];
	

	for(size_t i=0; i < strlen(argv[1]); i++)
	{
		if(!(isalnum(argv[1][i])))
		{
			fprintf(stderr, "The word containst charcter not good \n");
			exit(EXIT_FAILURE);
		}
	}

	struct stat buf;
	// first we test if the stat is ok then we test if the buffer is reg.
	if(stat(argv[2],&buf) == -1)
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	} 

	if (!S_ISREG (buf.st_mode))
	{
		fprintf(stderr,"The file is not regular");
		exit(EXIT_FAILURE);
	}

	pid_t pid;

	struct sigaction Signal;
	Signal.sa_handler=HandleUSR1;
	Signal.sa_flags=0;
	if(sigaction(SIGUSR1,&Signal,NULL) == -1)
	{
		// nu pui /n la perror.
		perror("sigaction broke");
		exit(EXIT_FAILURE);
	}

	struct sigaction Signal2;
	Signal2.sa_handler=HandlePID;
	Signal2.sa_flags=0;
	// daca moare copilu trimite un sigCHLD.
	if(sigaction(SIGCHLD,&Signal2,NULL) == -1)
	{
		// nu pui /n la perror.
		perror("sigaction2 broke");
		exit(EXIT_FAILURE);
	}




	if((pid = fork()) < 0)
	{
		perror("fork not ok\n ");
		exit(EXIT_FAILURE);
	}

	// tema de casa, sa il pun intr-un vector de aparitii, si dau doar la pozitiile alea.
	if(pid==0)
	{
		int fd= open(argv[2], O_WRONLY);
		if(fd==-1)
		{
			perror("file not open");
			exit(EXIT_FAILURE);
		}
		for(size_t i=0; i < sizeString; ++i)
		{
			if(isdigit(argv[1][i]))
			{	
				c=argv[1][i];
				if(kill(getppid(),SIGUSR1)==-1)
				{
					perror("kill broke \n");
					exit(EXIT_FAILURE);
				}
			}

		}

		size_t written=0;
//if ( (i = 1) != 2) exemplu.
		size_t ret=0;
		int counter=0;
		while((ret = write(fd,&argv[1][written],sizeString) ) != -1)
		{
			written += ret;

			if(written == sizeString)
			{
				break;
			}

			sizeString -= written;
			counter++;
		}


		if(ret == -1)
		{
			perror("error writing file");
			exit(EXIT_FAILURE);
		}

		exit(EXIT_SUCCESS);
	}

	
	while(1)
	{
		//printf(".");
	}

	return 0;
}