#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

pid_t G2PID;

void HSKill(int s){
    kill(G2PID, SIGKILL);
    kill((getpid()), SIGKILL);
}

typedef struct {    
	time_t timestamp;
    char g;
    int x;    
} message;

int main(int argc, char *argv[]){
    signal(SIGKILL, &HSKill);
    int b = 0, s = 0;          
    char g = 'A';
	G2PID = fork();
    message MSG1;
    if (G2PID == -1){
        perror("Error In G2");
        return 1;
    }
	else if (G2PID == 0){
        if ((execlp("./G2", "./G2", argv[4], (char *)NULL)) == -1)
            perror("Error in Execute G2!");        
        exit(-1);
    }
	else{
        int f = open("G2Rfifo", O_WRONLY);
        if (f == -1)        
            perror("Error in Open Named Pipe to write");        
        while (1)
        {
            b++;
            struct timeval tv;
            MSG1.g = g;
            MSG1.x = b;
            gettimeofday(&tv, NULL);
            time_t myTime = tv.tv_usec;
            MSG1.timestamp = myTime;            
            if ((write(f, &MSG1, sizeof(message))) == -1)
                perror("Error: Failed to write on G1");            
            int offs = 10;            
            int rndNum = offs + (rand() % (offs + 1));
            usleep(rndNum);
        }
        close(f);
        if (wait(&s) == -1)
            printf("Error in G2(%d)\n", s);        
    }
    return 0;
}
