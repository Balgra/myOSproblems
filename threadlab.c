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
#include<pthread.h>

int mat[1000][1000];
int n;
int v[20];


void * thr_fn(void *arg)
{
	int *aux;
	aux=(int *)arg;
	int sum=0;

	for(int i=0; i< n;i++)
	{
		sum=sum+mat[*aux][i];
		
	}

	v[*aux]=sum;
	printf("The sum of lines %d is %d \n",*aux,sum);

	pthread_exit((void *) 0);
}


int main(int argc, char *argv[])
{

	FILE *file;
  	file=fopen(argv[1], "r");
    if(file==NULL)
    {
      fprintf(stderr,"problem at file %s \n", argv[1]);
      exit(EXIT_FAILURE);
    }
  	fscanf(file,"%d",&n);
    if(n==0)
    {
      fprintf(stderr,"Bad input \n");
      exit(EXIT_FAILURE);
    }

 	for(int i = 0; i < n; i++)
  	{
      for(int j = 0; j < n; j++) 
      {
       		if (!fscanf(file, "%d", &mat[i][j])) 
       		{
       	   	 	break;
       		}
      }
  }

  printf("The matix looks like this: \n");
  for(int i=0;i<n;i++)
  {
    for(int j=0;j<n;j++)
    {
      printf("%d ",mat[i][j]);
    }
    printf("\n");
  }

  	int err;
  int vector[20];

  for(int i=0;i<n;i++)
  {
    vector[i]=i;
  }

	pthread_t tid[20];
	for(int i=0; i< n; i++)
	{
		err=pthread_create(&tid[i],NULL,thr_fn,(void *) &vector[i]);
		if(err!=0)
		{
			perror("can't create thread");
				exit(err);
		}
   // usleep(1);
		/*err=pthread_join(tid[i],NULL);
		if(err!=0)
		{
			perror("problem at join");
			exit(err);
		}*/
	}

//to be debugged.
	for(int i=0;i<n ;i++)
	{
		err=pthread_join(tid[i],NULL);
		if(err!=0)
		{
			perror("problem at join");
			exit(err);
		}
	}

	int finalSum=0;
	for(int i=0;i<n ;i++)
	{
		finalSum=finalSum+v[i];
	}

	printf("The Finals sum is : %d \n", finalSum);

  return finalSum;

}
















/*
#include<stdio.h>
#include <sys/types.h> 
#include <signal.h>
#include <unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int m[50][50];
int suma[50];

FILE *f;
pthread_t thread[20];
int n,total;

void *suma_pe_linie(void *i){
    
    int j;
    int *i_intreg = (int *)i;
    printf("%d \n",*i_intreg);
   
     
    for(j=0; j<n; j++){
        printf("suma[%d] %d \n",*i_intreg,m[*i_intreg][j]);
            suma[*i_intreg]+=m[*i_intreg][j];
    }
    
    pthread_exit((void *) 0);
}
   



int main(int argc, char **argv) {
    int i,j,create,join;
    f=fopen(argv[2],"r");
    
    if(argc !=3) {
        printf("Usage %s n file",argv[0]);
        exit(0);
    }
    
    n=atoi(argv[1]);
    
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            fscanf(f,"%d",&m[i][j]);
    printf("Afisare matrice: \n");
    for(i=0;i<n;i++) {
        for(j=0;j<n;j++)
            printf("%d ",m[i][j]);
        printf("\n");
    }
    
    for(i=0 ; i<n; i++)
    {
        create = pthread_create(&thread[i],NULL,suma_pe_linie,(void*)&i); //&i-ce parametru primeste functia mea, trebuie sa fie pointer,o adresa
        //join = pthread_join(thread[i],NULL);
        //total+=suma[i];

    }
    
        
    for(i=0;i<n;i++){
    	join = pthread_join(thread[i],NULL);
        total+=suma[i];
    }
    
        
    printf("Suma totala : %d \n",total);
    
    
return 0;
}*/