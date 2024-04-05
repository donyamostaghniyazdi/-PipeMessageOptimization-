# -PipeMessageOptimization-


PARAPHRASE THIS ENTIRE PART TILL HOW TO COMPILE. THERE ARE TOOLS AVAILABLE ONLINE FOR IT

*In the program, two generator processes G1 and G2 are supposed to generate quasi periodic messages to send to 
 process R. Process R will then computed the message and send it to process M for Outputing, making process M the 
 last process to terminal. It is for this reason that i have chosen process M to be the Master Process. The 
 process M then forks a child process which represents the process R, process R forks process G1 and process G1
 forks process G2. The process tree looks something like this (M)--->(R)--->(G1)--->(G2).
*For the pipes, in the unnamed pipe version, three unammed pipes are created in the M process and the file 
 descriptors required by each of the processes are then passed down the heirarchy during the exec process as 
 command line arquments. In the Named pipe version, the only change from the previous architecture is the 
 communication between the Gi processes and the R process. Instead of using two unnamed pipes for the 
 communication, one named pipe was created at the R process and then open for READ_ONLY, the created 
 FIFO(Named Pipe) was opened by each of the Gi processes for WRITE_ONLY.
*Another thing to note is that the process computes its time in microseconds and after 1000000 microseconds
 its 1 second then the microseconds starts from 1 again. The program is going to run for well over 1 second
 which means multiple cycle of microseconds. This has to be watched out for in the latency, offset delay and
 Bandwidth computation. My solution to this is a conditional statement that checks if creation time is higher than 
 reception time. If this is TRUE it means another cycle just started and the difference in time will be calculated 
 as difference = (1000000 - creation_time) + reception time 
*How the program will run: the program will print each message received from the R process in the format
 "The message %d from %c was created at %ld and received at %ld\n", where %d is for the sequence number of
 the message, %c is a char identifying the message, and %ld is for the timestamp of the message. 
 Initially i left the print out of the program, and put just a single print statement that prints, "please wait
 while the program computes", but the program takes a while to finish so i thought it would be a good idea to 
 have a feedback while the program is running so you know the program is working. 
 After the loop is done and all the message has been received and computed by the R process, the R process 
 then sends a kill signal to the G1 process which has a signal handler to handler the sigkill signal, in the 
 handler function of the G1, there are two statements one sends a kill signal to kill the child process which 
 is G2 and the other kills the current pid which is itself.
 After the signal has been handled the R process sends all the computed data to the M process through a pipe
 by the way of struct and the M process does some extra work on the data and Outputs it. This means when you
 run the program you'll see a lot of messages generated and after that the M_process then output the final 
 computation of all the messages. The M process also keeps a log of the computed output, the messages generated 
 is not included in the log file, only the final output is logged.

                                
HOW TO COMPILE

Run the shell script ----> run.sh

RESULTS

UNNAMED PIPE

Offset = 10
Number of G1 Cycle: 500006
Number of G2 Cycle: 499994
Number of Messages Received: 1000000
Offset delay in G1 Cycle(usec): 3573455236793
Offset delay in G2 Cycle(usec): 3573455664857
Average delay(latency) between generation and reception of messages(usec): 244
Estimated bandwidth between G and R(bit/s): 65573 

Offset = 20
Number of G1 Cycle: 500154
Number of G2 Cycle: 499846
Number of Messages Received: 1000000
Offset delay in G1 Cycle(usec): 3573462790844
Offset delay in G2 Cycle(usec): 3573463056527
Average delay(latency) between generation and reception of messages(usec): 118
Estimated bandwidth between G and R(bit/s): 135593 

Offset G1 = 10
Offset G2 = 20
Number of G1 Cycle: 540807
Number of G2 Cycle: 459193
Number of Messages Received: 1000000
Offset delay in G1 Cycle(usec): 3573458726596
Offset delay in G2 Cycle(usec): 3573459035129
Average delay(latency) between generation and reception of messages(usec): 173
Estimated bandwidth between G and R(bit/s): 92485 

Offset G1 = 20
Offset G2 = 10
Number of G1 Cycle: 459213
Number of G2 Cycle: 540787
Number of Messages Received: 1000000
Offset delay in G1 Cycle(usec): 3573458626807
Offset delay in G2 Cycle(usec): 3573458903220
Average delay(latency) between generation and reception of messages(usec): 170
Estimated bandwidth between G and R(bit/s): 94117 


NAMED PIPE

Offset = 10
Number of G1 Cycle: 499890
Number of G2 Cycle: 500110
Number of Messages Received: 1000000
Offset delay in G1 Cycle(usec): 45345081
Offset delay in G2 Cycle(usec): 45537097
Average delay(latency) between generation and reception of messages(usec): 25
Estimated bandwidth between G and R(bit/s): 640000 


Offset = 20
Number of G1 Cycle: 499919
Number of G2 Cycle: 500081
Number of Messages Received: 1000000
Offset delay in G1 Cycle(usec): 53043516
Offset delay in G2 Cycle(usec): 53754780
Average delay(latency) between generation and reception of messages(usec): 17
Estimated bandwidth between G and R(bit/s): 941176 


Offset G1 = 10
Offset G2 = 20
Number of G1 Cycle: 536007
Number of G2 Cycle: 463993
Number of Messages Received: 1000000
Offset delay in G1 Cycle(usec): 49160355
Offset delay in G2 Cycle(usec): 50098495
Average delay(latency) between generation and reception of messages(usec): 16
Estimated bandwidth between G and R(bit/s): 1000000 


Offset G1 = 20
Offset G2 = 10
Number of G1 Cycle: 463892
Number of G2 Cycle: 536108
Number of Messages Received: 1000000
Offset delay in G1 Cycle(usec): 49180385
Offset delay in G2 Cycle(usec): 49966513
Average delay(latency) between generation and reception of messages(usec): 17
Estimated bandwidth between G and R(bit/s): 941176 


DISCUSSION
MAKE A COMPARISON BASED ON THE PIPE RESULTS ON TOP
