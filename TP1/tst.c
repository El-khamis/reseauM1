#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(){
  printf("getpid = %d getppid = %d\n\n",getpid(),getppid());
  pid_t pid= fork();
  if(pid!=0){
    fork();
  }
    printf("getpid = %d getppid = %d After the fork \n \n",getpid(),getppid());

  
 

  return 0;
}
