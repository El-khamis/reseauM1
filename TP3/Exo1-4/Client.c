# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#include <unistd.h>


void my_err(char *message) {
    perror(message);
    exit(1);
};

struct MonCalcul{//PAs besoin d'un opérateur on envoie à des prog differents
  long etiquette;
  int x;
  int y;
};

struct resultat{
  long etiquette;
  int res;
};
int main(){
  char operateur;
  key_t maCle=ftok("./FiFiFichier",10);//Creation de la clef
  int file_id = msgget(maCle, IPC_CREAT|0666);//Creation de la file 
  struct MonCalcul operation;
  struct resultat monResultat;
  while(1){
    printf("Entrez une operation (x operateur y)\n");
    scanf("%i",&((operation.x)));
    scanf(" %c",&(operateur));
    scanf("%i",&(operation.y));

    switch (operateur){
      case '-':
      operation.etiquette=2;
      break;
      case '/':
      operation.etiquette=4;
      break;
      case '*':
      operation.etiquette=3;
      break;
      case '+':
      operation.etiquette=1;
      break;
      default :
      printf("Aucun operateur reconnu\n");
      exit(1);
      break;
    }
    if(msgsnd(file_id,&operation,sizeof(operation),0)<0){
      my_err("Problem d'envoie\n");
    }
    printf("Le message a bien été envoyé, j'attend la réponse de mon calcul\n");
    if(msgrcv(file_id,&monResultat,sizeof(monResultat),(long)0,0)<0){//Je reçois de tous les cannaux
      my_err("Problem de reception\n");
    }
    printf("Le resultat est : %i\n",monResultat.res);
  }
  return 0;
}
