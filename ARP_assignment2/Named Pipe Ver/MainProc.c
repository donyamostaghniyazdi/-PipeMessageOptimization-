#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/select.h>
#include <time.h>
#include <signal.h>
#define MAX_SIZE 1000000

typedef struct
{
    time_t timestamp;
    char g;
    int x;
} message;

typedef struct
{
    time_t OvOffsG1, OvOffsG2, TotalLatency;
    int G1Overall, G2Overall;
} mydata;

int main(int argc, char *argv[])
{
    int s = 0;
    int f[2];
	char f1[2];
    if ((pipe(f)) == -1)
        perror("Error: unnamed pipe");
    sprintf(f1, "%d", f[1]);
    pid_t rcivepid = fork();
    if (rcivepid == -1)
        perror("Error in receiver Fork");
    }    
    else if (rcivepid == 0)
    {
        if ((execlp("./Receiver", "./Receiver", f1, (char *)NULL)) == -1)      
            perror("Error in Executing Receiver");        
        exit(0);
    }
    else    
    {        
        kill(getpid(), SIGSTOP);
        close(f[1]);
        
        FILE *logfile;
        logfile = fopen("MyLog.txt", "w");
        int sz = MAX_SIZE;
        mydata d;
        if ((read(f[0], &d, sizeof(mydata))) == -1)        
            perror("Error in reading pipe");        
        printf("\n\n______________\n\n");
		fprintf("\n\n______________\n\n");
        
		printf("G1 Cycle Number Is: %d\n", d.G1Overall);
		fprintf(logfile, "G1 Cycle Number Is: %d\n", d.G1Overall);
		
        printf("G2 Cycle Number Is: %d\n", d.G2Overall);
		fprintf(logfile, "G2 Cycle Number Is: %d\n", d.G2Overall);
		
        printf("Messages Received Number: %d\n", sz);
		fprintf(logfile, "Messages Received Number: %d\n", sz);
		
        printf("delay in G1 Cycle: %ld\n", d.OvOffsG1);
		fprintf(logfile, "delay in G1 Cycle: %ld\n", d.OvOffsG1);
		
        printf("delay in G2 Cycle: %ld\n", d.OvOffsG2);
		fprintf(logfile, "delay in G2 Cycle: %ld\n", d.OvOffsG2);
		
        printf("Average delay: %d\n", (int)(d.TotalLatency / sz));
		fprintf(logfile, "Average delay: %d\n", (int)(d.TotalLatency / sz));
        
		printf("bandwidth G and R: %ld \n\n", (sz * sizeof(message) / ((d.TotalLatency) / MAX_SIZE)));
        fprintf(logfile, "bandwidth G and R: %d \n\n", (int)(sz * sizeof(message) / ((d.TotalLatency) / MAX_SIZE)));
		
		printf("\n\n______________\n\n");    
        fprintf("\n\n______________\n\n");
        close(f[0]);
        if (wait(&s) == -1)        
            printf("error in the receiver(%d)\n", s);        
    }
    exit(EXIT_SUCCESS);
}
