#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
// parametrii: lab.c si laboratory.
#define BUF_SIZE 4096

int count_digits(char buffer[], int nr_bytes){
	int contor = 0;
	for(int i=0; i <  nr_bytes; i++){
		if(isdigit(buffer[i]))
			contor++;
	}
	return contor;
}


int main(int argc, char *argv[]){

	if(argc != 3){
		printf("Usage <exec> file dir\n");
		return -1;
	}

	struct stat buf;
	
	if (lstat(argv[1], &buf) < 0){
    	printf("Error stat\n");
    	exit(4);
    }

	if( !S_ISREG(buf.st_mode) ){
		printf("Usage <exec> file dir*\n");
		return -2;
	}

	int fd = open(argv[1], O_RDONLY);

	char buffer[BUF_SIZE];
	int nr_bytes, contor = 0;

	while( (nr_bytes = read(fd, &buffer, 4096)) != 0)
	{
		contor += count_digits(buffer, nr_bytes);
	}
	
	printf("nr de cifre: %d\n", contor);
	char path[200];
	strcpy(path, argv[2]);
	strcat(path, "/");
	strcat(path, argv[1]);

	int fd2 = creat(path, S_IRUSR | S_IWUSR);
	printf("%s\n", path);
	char str[100];
	sprintf(str, "%d %d", contor, buf.st_gid);

	write(fd2, str, strlen(str));
	close(fd);
	close(fd2);
	return 0;
}