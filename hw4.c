#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "queue.h"

int main(int argc, char *argv[])
{
	int P = atoi(argv[1]); //maximum no. of processes

	typedef struct strc{         
		int id;
		char command[100];
		double starttime;
		double stoptime;
		char status[10];
	}STRC;
	STRC strcarray[20]; //array of structures to store all the commands,status and time
	
	
	int id=0;

	while(1)
	{
		char cmd[100]={};
		char command[100]={};
		char *token;
		char *tokens[100]={};
	
		printf("\nEnter command>");
		scanf(" %[^\n]",command);
		//printf("\np1:%s",command);
		cmd[0]='.';
		cmd[1]='/';

		token = strtok(command," ");//parse command with space as delimiter
		tokens[0]=token;
		int i =1;
		//printf("\nargs1:%s",tokens[0]);
		while(token!=NULL)
		{
			token=strtok(NULL, " ");
			tokens[i]=token;
			//printf("\nargs2:%s",tokens[i]);
			i++;
		}
		int pid=0,state=0,jobid=0,jobsn=0;
		time_t t1,t2;
		if(strcmp((tokens[0]),"submit")==0)
		{
			//printf("\nsubmitkk");
			strcat(cmd,(tokens[1]));
			
			strcarray[id].id = id;
			strcpy(strcarray[id].command,cmd);
			
			id++;
			if(jobsn<P) //if jobs running are less than P (execute the jobs if less than P)
			{
				pid=fork();
				jobid=getpid();

				printf("\njob %d added to the queue",id);
				if(pid==0) //child process
				{	  
	
					char * outfile = malloc(6);   //declaring file names
					char * errfile = malloc(6);   
					sprintf(outfile, "%d", getpid()); //getting the pid and converting to string
					sprintf(errfile, "%d", getpid());
					char outextn[] = ".out"; //assigning the corresponding extension
					char errextn[] = ".err";
					strcat(outfile,outextn);  //generating the filename with pid and extension (concatenated)
					strcat(errfile,errextn);
    					int child_out,child_err;
    					if ((child_out = open(outfile, O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1) //opening the output file with filename for writing data
    					{
		 				printf("Error opening file <pid>.out for output\n");
	  					exit(-1);
    					}
    					if ((child_err = open(errfile, O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1) //opening the error file with filename for writing error
    					{
		 			printf("Error opening file <pid>.err for output\n");
	  				exit(-1);
    					}
					dup2(child_out,1); //redirecting the output prints to .out file
					dup2(child_err,2); //redirecting the error messages to .err file
       					
					jobsn++;

					execvp(cmd,tokens+1);
					close(child_out);
					close(child_err);
					exit(0);
				}
				else //parent process
				{
				    
					if(waitpid(pid,&state,0)>0)
					{
						if(WIFEXITED(state)&&!WEXITSTATUS(state))
							printf("\nchild end successfully");
						else
							printf("\nchild problem");
					}
					else
						printf("\nwait pid failed");
				}
			}
			else 
			{

			}
		}
		else if(strcmp((tokens[0]),"showjobs")==0)
		{
			printf("\njobid\tjob\t\tstatus");
			int i;
			for(i=0;i<jobsn;i++)
			{
				printf("\n%d\t%s",strcarray[i].id,strcarray[i].command);
			}
		}
		
		else if(strcmp((tokens[0]),"submithistory")==0)
		{
			printf("\njobid\tjob\t\tstatus");
			int i;
			for(i=0;i<jobsn;i++)
			{
				printf("\n%d\t%s",strcarray[i].id,strcarray[i].command); 
			}
		}
		
	}//end of while
}


