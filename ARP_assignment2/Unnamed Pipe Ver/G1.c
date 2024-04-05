#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

pid_t G2PID;

typedef struct
{
    time_t timestamp;
    char g;
    int x;
} message;

void HSKill(int s){
    kill(G2PID, SIGKILL);
    kill((getpid()), SIGKILL);
}

int main(int argc, char *argv[])
{    
	signal(SIGKILL, &HSKill);
    int b = 0, s = 0;          
    char g = 'A';
	G2PID = fork();
	message MSG1;    
    int rp1 = atoi(argv[1]);
	int wp1 = atoi(argv[2]);
    int rp2 = atoi(argv[3]);
	int wp2 = atoi(argv[4]);
    int rp3 = atoi(argv[5]);    
    int wp3 = atoi(argv[6]);
    if (G2PID == -1)
    {
        perror("Error In G2");
        return 1;
    }
    else if (G2PID == 0)
    {
        close(rp1);
        close(wp1);
        close(rp2);
        close(rp3);
        close(wp3);
        if ((execlp("./G2", "./G2", argv[4], (char *)NULL)) == -1)        
            perror("Error in executing G2");
        exit(-1);
    }
    else    
    {     
        close(rp1);
        close(rp2);
        close(wp2);
        close(rp3);
        close(wp3);
        while (1)
        {
            b++;
            struct timeval tv;
            MSG1.g = g;
            MSG1.x = b;
            gettimeofday(&tv, NULL); 
            time_t myTime = tv.tv_usec;
            MSG1.timestamp = myTime; 
            if ((write(wp1, &MSG1, sizeof(message))) == -1)
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
