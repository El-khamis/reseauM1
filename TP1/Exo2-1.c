#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int fct(int i){
  if( i==0){
    return 0;
  }
  else{
    pid_t pid;
    pid=fork();
    if(pid==0){
       printf("Je suis %d mon pere est %d\n",getpid(),getppid());
       sleep(10);
      return 0;
    }
    else{

      fct(i-1);
    }
  }
  return 0;
}
int main(){
  int largeur;
  //Le faire avc un une fonction recursive
  //If pid!=0 alors on est dans le pere
  // Le fils a un pid == 0
  printf("Entrez la largeur de l'arbre\n");
  scanf("%d",&largeur);
  fct(largeur);
  return 0;
}
