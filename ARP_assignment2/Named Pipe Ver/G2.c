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

typedef struct {    
	time_t timestamp;
    char g;
    int x;    
} message;

int main(int argc, char *argv[])
{
   message MSG2;
   int b = 0;     
   char a = 'Z';   
   int f = open("G2Rfifo", O_WRONLY);
   if (f == -1){
      perror("Open Named Pipe to write");
   }
   while (1)
   {
	  int offs = 10;
      b++;
      struct timeval tv;
      MSG2.g = a;
      MSG2.x = b;
      gettimeofday(&tv, NULL);
      MSG2.timestamp = tv.tv_usec;      
      if ((write(f, &MSG2, sizeof(message))) == -1)      
         perror("Error In write on G2");      
      int RndNum = offs + (rand() % (offs + 1)); 
      usleep(RndNum);                           
   }
   close(f);
   exit(0);
}
