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
  int continuer;
  key_t maCle=ftok("./FiFiFichier",10);//Creation de la clef
  int file_id = msgget(maCle, IPC_CREAT|0666);//Creation de la file
  struct MonCalcul operation;
  struct MonCalcul poubelle;
  struct resultat monResultat;
  while(1){
    printf("Voulez-vous continuez ? \n0 pour oui\n1 pour non\n");
    scanf("%i",&continuer);
    if(continuer){
      printf("Fin du programme, suppression de la file\n");
      if(msgctl(file_id,IPC_RMID,NULL)<0){
        my_err("Impossible de supprimer la file\n");
      }
      exit(0);
    }
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
    printf("\n\nLe message a bien été envoyé, j'attend la réponse de mon calcul\n");
    if(msgrcv(file_id,&monResultat,sizeof(monResultat),(long)8,IPC_NOWAIT)<0){//si je met ipc_nowait je reçois jamais le premier message,
                                                                    // mais si je met pas IPC_NOWAIT je peux pas détecter dans mes calculatrice ne sont pas alluméesd
      perror("Problem de reception\n");
      printf("\n\n\nLe programme en charge de ce calcul n'est pas en cours d'execution, je retire ce message de la fille pour ne pas créer de doublon\n");
      msgrcv(file_id,&poubelle,sizeof(poubelle),0,0);
    }
    else{
      printf("Le resultat est : %i\n",monResultat.res);
    }
  }
  return 0;
}
