#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>



int ok = 1;
int cont = 0;

void countC(char *file, char c)
{

    int contor=0;
    char buffer[4096];

    struct stat buf;

    if(lstat(file, &buf) < 0)
    {
        printf("Error stat\n");
        exit(4);
    }
    if(!S_ISREG(buf.st_mode))
    {
        printf("%s is not a regular file\n", file);
        exit(5);
    }


    int fd = open(file, O_RDONLY);
    int nr_bytes;

    while( (nr_bytes = read(fd, &buffer, 4096)) != 0 )
    {
        for(int i = 0; i < nr_bytes; ++i){
            if(buffer[i] == c)
                contor++;
        }
    }

    printf("%s : %d\n", file, contor);

    close(fd);
}


void setOK(int x)
{
    cont++;

    int w, status;
    w = wait(&status);
    if(WIFEXITED(status))
        printf("Child process %d exits with status: %d\n", w, WEXITSTATUS(status));
}

int main(int argc, char *argv[]){

    if(argc < 2)
    {
        printf("Usage ./executable c file1 file2 … \n");
        exit(1);
    }

    if(strlen(argv[1]) != 1 && !isalnum(argv[1][0]))
    {
        printf("c must be an alpha numeric character \n");
        exit(1);
    }

    pid_t pid;

    char c = argv[1][0];

    ///create children
    for(int i = 2; i < argc; ++i)
    {
        if( (pid = fork()) < 0)
        {
            printf("Unable to create child!");
            exit(1);
        }
        if(pid == 0)
        {
            countC(argv[i], c);
            exit(getppid());
        }
    }

    struct sigaction Sig;
    Sig.sa_handler = setOK;
    Sig.sa_flags = 0;
    sigaction(SIGCHLD, &Sig, NULL);

    while(cont != argc-2)
    {
        printf(".");
    }

    return 0;
}
