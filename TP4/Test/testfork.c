#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(){
  int nmbrFork=0;
  int pid=0;
  int identifiant=0;
  printf("Bonjour, entrez le nombre de fork a créer \n");
  scanf("%d",&nmbrFork);
  for(int i=0;i<nmbrFork;i++){
    pid=fork();
    if(pid==0){
      identifiant=i;
      break;
    }
  }
  if(pid==0){
    printf("Je suis le fils %d on m'a donné l'identifiant %d\n",getpid(),identifiant);
    exit(0);
  }
  else{
    while(wait(NULL)>0){
    }
    printf("Je suis le père %d\n",getpid());
  }
}
