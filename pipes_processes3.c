#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>



int main(int argc, char **argv){ 
  
  
  int pipefd1[2];//create a pipe 
  int pipefd2[2];//create a pipe
  int pid; //parent 
  
  

  char *cat_args[] = {"cat", "scores", NULL}; 
  char *grep_args[] = {"grep",argv[1], NULL}; 
  char *sort_args[]={"sort",NULL};
  

  pipe(pipefd1); 
  pipe(pipefd2); 
  pid = fork();  

  if (pid == 0) //child executes cat, I want the output of cat 
    {
      // child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe

      dup2(pipefd1[1], 1); 
      

      // close unused hald of pipe

      close(pipefd1[0]); 
      close(pipefd2[1]); 
      close(pipefd2[0]); 
      close(pipefd1[1]);

      execvp(*cat_args, cat_args); //execvp=replaces addr space , 2nd arg= all params 
    }
  else 
    {
      if(fork()==0){ 
        dup2(pipefd1[0],0);  
        
        
        dup2(pipefd2[1],1);  
        
        close (pipefd2[0]);
        close (pipefd1[1]);
        close(pipefd2[1]);
        close(pipefd1[0]);
        
        
        execvp(*grep_args,grep_args);
        
      } else{ //is parent 
        
        if(fork() ==0){ 
          
          // parent gets here and handles "cat scores"

          // replace standard output with output part of pipe
          
          dup2(pipefd2[0],0);
          
          // close unused unput half of pipe
          close (pipefd2[1]); 
          close (pipefd1[1]); 
          close (pipefd1[0]); 
          close(pipefd2[0]);
          
          execvp(*sort_args,sort_args);
          
        }
        
      }
  }
  
  close (pipefd2[1]); 
  close (pipefd1[1]); 
  close (pipefd1[0]); 
  close(pipefd2[0]);
  
  wait(NULL);
  wait(NULL);
  wait(NULL); 
}