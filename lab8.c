#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include<signal.h>
#include<string.h>

char c[10]="???";

void HandleUSR1(int sig)
{

	strcpy(c,"!!!");
	printf("\n!!!\n");

}

void TrimiteSemnal(int sig)
{	

	kill(getppid(),SIGUSR1);

}

void HandleWait(int sig)
{
	int status; 
	wait(&status);
    if(WIFEXITED(status))
    {
       printf("Exit status: %d\n", WEXITSTATUS(status));
       exit(6);
    }

}


int main(int argc, char *argv[])
{
	struct sigaction Semnal;
	pid_t pid;

	if(argc!=4)
	{
		printf("problem at the argc");
	}

	int a0,r,N;

	a0=atoi(argv[1]);
	r=atoi(argv[2]);
	N=atoi(argv[3]);

	double a1;
	double a2;
    a2=(double)a0;
  //  a=(float*)malloc((N+1)*sizeof(float));


	if( (pid = fork()) < 0)
	{
        printf("No bueno\n");
        exit(1);
    }

    if(pid==0)
    {

    	Semnal.sa_handler=TrimiteSemnal;
    	// how mapping works..
    	Semnal.sa_flags=0;
    	sigaction(SIGALRM,&Semnal,NULL);


        alarm(1);

        int i;
        for(i = 0; i <= N; ++i)
        {
            a1=a2 + 1/(double)r;
            a2=a1;
        }

        printf("%.2f \n",a1);

        exit(5);
    }

    struct sigaction Sig1;
    struct sigaction Sig2;

    Sig1.sa_handler=HandleUSR1;

    Sig1.sa_flags=0;
    sigaction(SIGUSR1,&Sig1,NULL);

    Sig2.sa_flags=0;
    Sig2.sa_handler=HandleWait;
    sigaction(SIGCHLD,&Sig2,NULL);



    while(1)
    {
    	printf("%s",c);
    	//sleep(1);
    }

 	
	return 0;
}