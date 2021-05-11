#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

int ok = 0;

void HandleWait(int sig){
    ok++;
    int status, w;
    w = wait(&status);
    if(WIFEXITED(status))
        printf("The child process with the PID %d ended with the code %d\n", w, WEXITSTATUS(status));
}

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Usage <exec> file\n");
        exit(1);
    }

    struct stat buf;
    if (lstat(argv[1], &buf) < 0){
    	printf("Error stat\n");
    	exit(1);
    }
	if( !S_ISREG(buf.st_mode) ){
		printf("Usage <exec> file\n");
		return -2;
	}

    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1)
    {
        printf("Scandal pipe1\n");
        exit(1);
    }
    if (pipe(pipe2) == -1)
    {
        printf("Scandal pipe2\n");
        exit(1);
    }

    int pid1,pid2,pid3;

    struct sigaction Sig;
    Sig.sa_handler = HandleWait;
    Sig.sa_flags = 0;
    sigaction(SIGCHLD, &Sig, NULL);


    ///Child 1
    if((pid1 = fork()) < 0){
        printf("Error at creating child 1\n");
        exit(1);
    }
    if(pid1 == 0){
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);
        dup2(pipe1[1], 1);
        execlp("cat", "cat", argv[1], NULL);
        exit(2);
    }

    ///Child 2
    if((pid2 = fork()) < 0){
        printf("Error at creating child 2\n");
        exit(1);
    }
    if(pid2 == 0){
        close(pipe1[1]);
        close(pipe2[0]);
        dup2(pipe2[1], 1);
        dup2(pipe1[0], 0);
        execlp("grep", "grep", "-E", "[0-9+-=]", NULL);
        exit(1);
    }

    ///Child 3
    if((pid3 = fork()) < 0){
        printf("Error at creating child 2\n");
        exit(2);
    }
    if(pid3 == 0){
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);
        dup2(pipe2[0], 0);
        execlp("wc", "wc", "-l", NULL);
        exit(4);
    }

    close(pipe1[1]);
    close(pipe1[0]);
    close(pipe2[1]);
    close(pipe2[0]);

    while(ok!=3){
        printf("a\b");
    }


    return 0;
}
