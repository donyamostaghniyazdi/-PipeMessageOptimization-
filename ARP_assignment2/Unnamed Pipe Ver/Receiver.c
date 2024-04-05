#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_SIZE 1000000

typedef struct
{
    time_t timestamp;
    char g;
    int x;
} message;

typedef struct
{
    time_t OOffG1, OOffG2, _latency;
    int G1Overall, G2Overall;
} MyData;

int fq(int y, pid_t G1PID)
{
    int returnvalue;
    if (y == MAX_SIZE)
    {
        kill(G1PID, SIGKILL);
        returnvalue = 1;
    }
    else
    {
        returnvalue = 0;
    }
    return returnvalue;
}

int main(int argc, char *argv[])
{
    int rp1 = atoi(argv[1]); 
    int rp2 = atoi(argv[3]); 
    int rp3 = atoi(argv[5]); 
    int wp1 = atoi(argv[2]);
    int wp2 = atoi(argv[4]);
    int wp3 = atoi(argv[6]);

    pid_t G1PID = fork();
    if (G1PID == -1)
    {
        perror("Error in G1 Pid");
        return 2;
    }
    else if (G1PID == 0)
    {
        if ((execlp("./G1", "./G1", (char *)NULL)) == -1)        
            perror("Error in execution G1");        
        exit(0);
    }
    else
    {
        message msgs;
		int offs, i = 0, y = 0, G1O = 0, G2O = 0, st = 0;
		double latency = 0;
		unsigned long int SGenTime = 0, G2EndTime = 0, EGenTime = 0 ERecTime = 0;
        unsigned long int G1InitTime = 0, G2InitTime = 0,  G1EndTime = 0;
        unsigned long int dfr = 0, OOffG1, OOffG2;
		fd_set pf;
        struct timeval timval;
		timval.tv_sec = 0;
        timval.tv_usec = 1;
        int R_Value;
		time_t rectime;		
        close(wp1);
        close(wp2);
        close(rp3);
        int Rnd_fd[2]; 
        Rnd_fd[0] = rp1;
        Rnd_fd[1] = rp2;
        while (1)
        {
            fq(y, G1PID);
            FD_ZERO(&pf); FD_SET(rp1, &pf);  FD_SET(rp2, &pf);
            struct timeval myTimeValue;
            R_Value = select(6, &pf, NULL, NULL, &timval);
            if (R_Value == -1)
                perror("Error in select");
            else if (R_Value)
            {
                y++;
                if ((FD_ISSET(rp1, &pf)) && (FD_ISSET(rp2, &pf)))
                {
                    int RndCh = rand() % (2);
                    if ((read(Rnd_fd[RndCh], &msgs, sizeof(message))) == -1)                    
                        perror("Error in reading G1 or G2");                    
                    gettimeofday(&myTimeValue, NULL);
                    rectime = myTimeValue.tv_usec;
                    if (msgs.timestamp > rectime)                    
                        dfr = MAX_SIZE - msgs.timestamp + rectime;                    
                    else                    
                        dfr = myTimeValue.tv_usec - (msgs.timestamp);                    
                    latency += dfr;
                }
                else if (FD_ISSET(rp1, &pf))
                {                    
                    if ((read(rp1, &msgs, sizeof(message))) == -1)
                        perror("Error in reading G1");                    
                    gettimeofday(&myTimeValue, NULL);
                    rectime = myTimeValue.tv_usec;
                    if (msgs.timestamp > rectime)                    
                        dfr = MAX_SIZE - msgs.timestamp + rectime;                    
                    else                    
                        dfr = myTimeValue.tv_usec - (msgs.timestamp);                    
                    latency += dfr;
                }
                else if (FD_ISSET(rp2, &pf))
                {
                    if ((read(rp2, &msgs, sizeof(message))) == -1)                    
                        perror("Error in reading G2");                    
                    gettimeofday(&myTimeValue, NULL);
                    rectime = myTimeValue.tv_usec;
                    if (msgs.timestamp > rectime)                    
                        dfr = MAX_SIZE - msgs.timestamp + rectime;                    
                    else                    
                        dfr = myTimeValue.tv_usec - (msgs.timestamp);                    
                    latency += dfr;
                }
                if (i == 0)                
                    SGenTime = msgs.timestamp;                
                else if (i == MAX_SIZE - 1)
                {
                    EGenTime = msgs.timestamp;
                    ERecTime = rectime;
                }
                if (msgs.g == 'A')
                {
                    G1O++;
                    G1InitTime = G1EndTime;
                    G1EndTime = msgs.timestamp;
                    if (G1InitTime > G1EndTime && msgs.x != 1)                    
                        offs = MAX_SIZE - G1InitTime + G1EndTime;                    
                    else if (G1InitTime < G1EndTime && msgs.x != 1)                    
                        offs = G1EndTime - G1InitTime;                    
                    OOffG1 += offs;
                }
                else if (msgs.g == 'Z')
                {
                    G2O++;
                    G2InitTime = G2EndTime;
                    G2EndTime = msgs.timestamp;
                    if (G2InitTime > G2EndTime)                    
                        offs = MAX_SIZE - G2InitTime + G2EndTime;                    
                    else                    
                        offs = G2EndTime - G2InitTime;                    
                    OOffG2 += offs;
                }
                i++;
				printf("message %d from %c: at %ld, receive at %ld\n", msgs.x, msgs.g, msgs.timestamp, rectime);
            }
            else            
                usleep(10);            
            if ((fd(y, G1PID)))            
                break;            
        }
        MyData d;
        d.OOffG1 = OOffG1;
        d.OOffG2 = OOffG2;
        d._latency = latency;
        d.G1Overall = G1O;
        d.G2Overall = G2O;
        if ((write(wp3, &d, sizeof(MyData))) == -1)        
           perror("Error in Write Fifo 3");        
        kill(getppid(), SIGCONT);
        close(rp1);
        close(rp2);
        close(wp3);
		if (wait(&st) == -1)        
            printf("error in G1(%d)\n", st);   
    }
	return 0;
}
