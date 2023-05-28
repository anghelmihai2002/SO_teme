#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define SCRIERE "RESP_PIPE_98075"
#define REQUEST "REQ_PIPE_98075"
#define MEM_NAME "/x7iwgU6D"

int main(){

    if(mkfifo(SCRIERE, 0600) != 0){
        perror("Nu merge");
        exit(1);
    }

    int fd_s = -1, fd_r = -1, i = 0, var = 98075;
    char s[] = "";
    unsigned int mem_lenght;
    volatile char *sharedChar = NULL;
        


    fd_r = open(REQUEST, O_RDONLY);
    fd_s = open(SCRIERE, O_WRONLY);

    write(fd_s, "START$", 6);
    while(true){
        while(true){
            read(fd_r, &s[i], sizeof(char));
            if(s[i] == '$') break;
            i++;
        }
        s[i] = '\0';
        if(strcmp(s, "PING") == 0){
            write(fd_s, "PING$", 5);
            write(fd_s, &var, sizeof(var));
            write(fd_s, "PONG$", 5);
            continue;
        }
        else if(strcmp(s, "CREATE_SHM") == 0){
            int shmFd;
            read(fd_r, &mem_lenght, sizeof(unsigned int));
            shmFd = shm_open(MEM_NAME, O_CREAT | O_RDWR, 664);
            ftruncate(shmFd, mem_lenght);
            sharedChar = (volatile char*)mmap(0, sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
            if(sharedChar == (void*)-1){
                write(fd_s, "CREATE_SHM$", 11);
                write(fd_s, "ERROR$", 6);
            }
            else {
                write(fd_s, "CREATE_SHM$", 11);
                write(fd_s, "SUCCESS$", 8);    
            }
            continue;
        }
        else if(strcmp(s, "WRITE_TO_SHM") == 0){
            unsigned int offs, val;
            read(fd_r, &offs, sizeof(unsigned int));
            read(fd_r, &val, sizeof(unsigned int));
            if(0 > (offs + sizeof(val)) || (offs + sizeof(val)) > mem_lenght){
                write(fd_s, "WRITE_TO_SHM$", 13);
                write(fd_s, "ERROR$", 6);
            }
            
            continue;
        }
        else if(strcmp(s, "EXIT") == 0){
            break;
        }
        else break;
    }
}