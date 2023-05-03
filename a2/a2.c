#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "a2_helper.h"

void *thread_function_9(void *unused)
{
    int *j = (int*) unused;
    info(BEGIN, 9, *j);
    if(*j != 3 && *j != 5){
        info(END, 9, *j);
    }
    else if(*j == 5){
        info(END, 9, *j);
        info(END, 9, *j - 2);
    }
    return NULL;
}

void *thread_function_6(void *unused)
{
    int *j = (int*) unused;
    info(BEGIN, 6, *j);
    info(END, 6, *j);
    return NULL;
}

void *thread_function_8(void *unused)
{
    int *j = (int*) unused;
    info(BEGIN, 8, *j);
    if(*j < 10 || *j > 14){
        info(END, 8, *j);
    }
    else if(*j == 14){
        for(int i = 14; i > 9; i--)
            info(END, 8, i);
    }
    return NULL;
}


int main(){
    init();
    pid_t pid2, pid3, pid4, pid5, pid6, pid7, pid8, pid9;
    info(BEGIN, 1, 0);
    pid2 = fork();
    if(pid2 == 0){
        info(BEGIN, 2, 0);
        info(END, 2, 0);
        return 0;
    }
    waitpid(pid2, NULL, 0);
    pid3 = fork();
    if(pid3 == 0){
        info(BEGIN, 3, 0);
        pid5 = fork();
        if(pid5 == 0){
            info(BEGIN, 5, 0);
            pid7 = fork();
            if(pid7 == 0){
                info(BEGIN, 7, 0);
                info(END, 7, 0);
                return 0;
            }
            waitpid(pid7, NULL, 0);
            pid8 = fork();
            if(pid8 == 0){
                info(BEGIN, 8, 0);
                pthread_t tids[37];
                for(int i = 1; i <= 36; i++){
                    pthread_create(&tids[i], NULL, thread_function_8, &i);
                    pthread_join(tids[i], NULL);
                }
                info(END, 8, 0);
                return 0;
            }
            waitpid(pid8, NULL, 0);
            pid9 = fork();
            if(pid9 == 0){
                info(BEGIN, 9, 0);
                pthread_t tids[6];
                for(int i = 1; i < 6; i++){
                    pthread_create(&tids[i], NULL, thread_function_9, &i);
                    pthread_join(tids[i], NULL);
                }
                info(END, 9, 0);
                return 0;
            }
            waitpid(pid9, NULL, 0);
            info(END, 5, 0);
            return 0;
        }
        waitpid(pid5, NULL, 0);
        pid6 = fork();
        if(pid6 == 0){
            info(BEGIN, 6, 0);
            pthread_t tids[7];
            for(int i = 1; i < 7; i++){
                pthread_create(&tids[i], NULL, thread_function_6, &i);
                pthread_join(tids[i], NULL);
            }
            info(END, 6, 0);
            return 0;
        }
        waitpid(pid6, NULL, 0);
        info(END, 3, 0);
        return 0;
    }
    waitpid(pid3, NULL, 0);
    pid4 = fork();
    if(pid4 == 0){
        info(BEGIN, 4, 0);
        info(END, 4, 0);
        return 0;
    }
    waitpid(pid4, NULL, 0);
    info(END, 1, 0);
    return 0;
}