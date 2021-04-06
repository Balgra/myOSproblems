/*#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int N;
int hardLinkContor;
int simLinkContor;

void openDirectories(char *directorPath)
{
    DIR *director=opendir(directorPath);

    if(director==NULL)
    {
        printf("Acesta este un mesaj de eroaore");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(director)) != NULL)
    {
        printf("%s \n",entry->d_name);

        if(entry->d_type == DT_DIR)
        {
            char path[1024];

            if (!(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
            {
                 snprintf(path, sizeof(path), "%s/%s", directorPath, entry->d_name);
                 openDirectories(path);
            }
        }

        if(entry->d_type==DT_LNK)
        {
            struct stat buf;
            char filename[1024]="";
            strcpy(filename,directorPath);
            strcat(filename,"/");
            strcat(filename,entry->d_name);

            if(lstat(filename, &buf) < 0)
            {
                printf("Error stat1 \n");
                exit(-1);
            }

            if(buf.st_size>N)
            {
                simLinkContor++;
            }

        }

        if(entry->d_type==DT_REG)
        {
            struct stat buf;
            char filename[1024]="";
            strcpy(filename,directorPath);
            strcat(filename,"/");
            strcat(filename,entry->d_name);

            if(lstat(filename, &buf) < 0)
            {
                printf("Error stat2 \n");
                exit(-1);
            }

            if(buf.st_nlink>N)
            {
                hardLinkContor++;
            }

        }
    }



    closedir(director);
}

int main(int argc, char *argv[])
{
    if(argc !=3)
    {
        printf("Usage <exec> dir N\n");
        exit(-1);
    }


    N=atoi(argv[2]);

    openDirectories(argv[1]);

    printf("Hard link counter is %d \nSoft Lnk counter is %d \n",hardLinkContor, simLinkContor);

    return 0;
}*/


#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int N;
int hardLinkContor=0;
int simLinkContor=0;

void openDirectories(char *directorPath)
{
    DIR *director=opendir(directorPath);

    if(director==NULL)
    {
        printf("Acesta este un mesaj de eroaore");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(director)) != NULL)
    {
        struct stat buf;
        char path[1024];
        char filename[1024]="";
        //printf("%s \n",entry->d_name);
        switch (entry->d_type)
        {

            case DT_DIR:
                if (!(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0))
                {
                     snprintf(path, sizeof(path), "%s/%s", directorPath, entry->d_name);
                     openDirectories(path);
                }
                break;

            case DT_LNK:
                sprintf(filename,"%s/%s",directorPath,entry->d_name);

                if(lstat(filename, &buf) < 0)
                {
                    printf("Error stat1 \n");
                    exit(-1);
                }

                if(buf.st_size>N)
                {
                    simLinkContor++;
                }
                break;

            case DT_REG:   
                sprintf(filename,"%s/%s",directorPath,entry->d_name);

                if(lstat(filename, &buf) < 0)
                {
                    printf("Error stat2 \n");
                    exit(-1);
                }

                if(buf.st_nlink>N)
                {
                    hardLinkContor++;
                }
                break;

        }
    }



    closedir(director);
}

int main(int argc, char *argv[])
{
    if(argc !=3)
    {
        printf("Usage <exec> dir N\n");
        exit(-1);
    }


    N=atoi(argv[2]);

    openDirectories(argv[1]);

    printf("Hard link counter is %d \nSoft Lnk counter is %d \n",hardLinkContor, simLinkContor);

    return 0;
}
