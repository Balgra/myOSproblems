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
        printf("Wrong number of arguments!\nUsage: ./exe file\n");
        exit(1);
    }

    int fd1[2];
    int fd2[2];

    if(pipe(fd1) == -1){
        printf("Error pipe\n");
        exit(1);
    }
    if(pipe(fd2) == -1){
        printf("Error pipe\n");
        exit(1);
    }

    int pid;
    if((pid = fork()) < 0){
        printf("Error fork\n");
        exit(1);
    }
    ///cod child
    if(pid == 0){

        close(fd1[1]);
        close(fd2[0]);

        //char buf[2];
        //int new_line=1, ok = 1;

        dup2(fd1[0], 0);
        dup2(fd2[1], 1);

        execlp("grep", "grep", "-E", "^[A-Z].*", NULL);

        /**
        while(read(fd1[0], &buf, 1)){

            if(new_line == 1){
                if(isupper(buf[0])){
                    ok = 1;
                }
                else
                    ok = 0;
                new_line = 0;
            }
            if(buf[0] == '\n')
                new_line = 1;

            if(ok)
                write(fd2[1], &buf, 1);
        }**/

        close(fd1[0]);
        close(fd2[1]);

        exit(1);
    }
    ///cod parent
    close(fd1[0]);
    close(fd2[1]);
    int file = open(argv[1], O_RDONLY);
    	struct stat buf;
	// first we test if the stat is ok then we test if the buffer is reg.
	if(stat(argv[1],&buf) == -1)
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	} 

	if (!S_ISREG (buf.st_mode))
	{
		fprintf(stderr,"The file is not regular");
		exit(EXIT_FAILURE);
	}
    char buf2[2];
    int contor = 0;
    int ok1=1, ok2= 1;

    while(ok1){
        //execlp("echo","echo", "sex",NULL);



        if(read(file, &buf2, 1)){
            write(fd1[1], &buf2, 1);
            ok1 = 1;
        }
        else
            ok1 = 0;
    }

    close(fd1[1]);
    //printf(":D");
    while(ok2){

        if(read(fd2[0], &buf2, 1)){
            if(isupper(buf2[0]))
                contor++;
            ok2 = 1;
        }
        else
            ok2 = 0;
    }


    close(fd2[0]);
    close(file);

    printf("%d\n", contor);

    int status;
    pid = wait(&status);
    if(WIFEXITED(status))
        //fprintf(stderr, "Exit status: %d\n", WEXITSTATUS(status));
    exit(EXIT_SUCCESS);
    return 0;
}
