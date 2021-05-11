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


int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Usage ...\n");
        exit(1);
    }

    int fd[2];
    if(pipe(fd) == -1){
        printf("Error pipe\n");
        exit(1);
    }

    int pid;
    if((pid = fork()) < 0){
        printf("Error fork\n");
        exit(1);
    }
    ///cod child
    if(pid == 0)
    {
        close(fd[1]);
        char buf[2];
        char args[300] = "";
        while(read(fd[0], buf, 1))
        {
            //printf("%c", buf[0]);
            if(isdigit(buf[0]))
            {

                strcat(args, buf);
                strcat(args, " ");
            }
        }
        
        close(fd[0]);

        execlp("bash", "bash","PracticeBash2.sh", args, NULL);

        exit(1);
    }

    close(fd[0]);

    int file = open(argv[1], O_RDONLY);
    char buf2;
    while(read(file, &buf2, 1))
    {
        //printf("%c", buf2);
        write(fd[1], &buf2, 1);
    }

    close(fd[1]);
    close(file);


    int status;
    pid = wait(&status);
    if(WIFEXITED(status))
        printf("Exit status: %d\n", WEXITSTATUS(status));
    exit(EXIT_SUCCESS);

    return 0;
}
