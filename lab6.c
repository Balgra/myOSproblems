#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int n;

int main(int argc, char *argv[]){
    pid_t pid;
    int status;

    if(argc >= 2){
        n = atoi(argv[1]);
    }
    else{
        printf("Usage prog n file1 file2 file3 ...\n");
        exit(1);
    }

    ///First child
    if( (pid = fork()) < 0){
        printf("No bueno\n");
        exit(1);
    }
    if(pid == 0){
        printf("%d: ", pid);
        for(int i = 1; i <= n; ++i){
            printf("%d ", i);
        }
        printf("\n");
        exit(10);
    }

    ///The other children
    for(int i = 2; i < argc; ++i)
    {
        if( (pid = fork()) < 0)
        {
            printf("No bueno\n");
            exit(1);
        }
        
        if(pid == 0)
        {
            execlp("wc", "wc", "-l", argv[i], NULL);
            exit(i);
        }
        //wait(&status);
    }

    for(int i = 1; i < argc; ++i){
        wait(&status);
        if(WIFEXITED(status))
            printf("Exit status: %d\n", WEXITSTATUS(status));
    }

    //wait(&status);




    return 0;
}
