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
	int fd1 = atoi(argv[1]);
    if (mkfifo("G2Rfifo", 0666) == -1)
    {
        if (errno != EEXIST)        
            perror("Error In mkfifo");        
    }
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
        time_t rectime;                         
        int fd = open("G2R_fifo", O_RDONLY);
        if (fd == -1)        
            perror("Error In Open Named Pipe to read");
        while (1)
        {            
            fq(y, G1_PID); 
            y++;
            struct timeval tva1;
            if ((read(fd, &msgs, sizeof(message))) == -1)
                perror("Error In Reads G1 or G2");
            gettimeofday(&tva1, NULL);
            rectime = tva1.tv_usec;
            if (msgs.timestamp > rectime)            
                dfr = MAX_SIZE - msgs.timestamp + rectime;            
            else            
                dfr = tva1.tv_usec - msgs.timestamp;            
            latency += dfr;
            if (i == 0)            
                SGenTime = msgs.timestamp;            
            else if (i == MAX_SIZE - 1)
            {
                EGenTime = msgs.timestamp;
                ERecTime = rectime;
            }
            if (msgs.g == 'A')
            {
                G1EndTime = msgs.timestamp;
				G1O++;
                G1InitTime = G1EndTime;                
                if (G1InitTime > G1EndTime && msgs.x != 1)                
                    offs = MAX_SIZE - G1InitTime + G1EndTime;                
                else if (G1InitTime < G1EndTime && msgs.x != 1)                
                    offs = G1EndTime - G1InitTime;
                OOffG1 += offs;
            }
            else if (msgs.g == 'Z')
            {
				G2InitTime = G2EndTime;
				G2O++;				   
                G2EndTime = msgs.timestamp;
                if (G2InitTime > G2EndTime)                
                    offs = MAX_SIZE - G2InitTime + G2EndTime;                
                else                
                    offs = G2EndTime - G2InitTime;                
                OOffG2 += offs;
            }
            i++;
            printf("message %d from %c: at %ld, receive at %ld\n", msgs.x, msgs.g, msgs.timestamp, rectime);            
            if ((fq(y, G1_PID)))             
                break;            
        }
        MyData d;
        d.OOffG1 = OOffG1;
        d.OOffG2 = OOffG2;
        d._latency = latency;
        d.G1O = G1O;
        d.G2O = G2O;
        if ((write(fd1, &d, sizeof(MyData))) == -1)        
            perror("Error in Write Fifo");        
        kill(getppid(), SIGCONT);
        close(fd1);
        if (wait(&st) == -1)        
            printf("error in G1(%d)\n", st);        
    }
	return 0;
}


