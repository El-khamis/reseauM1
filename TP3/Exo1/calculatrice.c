# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>

# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

void my_err(char *message) {
    perror(message);
    exit(1);
}

struct calcul{
    int x;
    int y;
    char operateur;
};
 int main(){
   int resultat;
   key_t ma_cle;
   int IdDuFichier=0,write=0;
   ma_cle=ftok("./MonFichier",1);
   IdDuFichier= msgget(ma_cle, 0666|IPC_CREAT);
  struct calcul message;
  while(1){
    if(msgrcv(IdDuFichier, &message, sizeof(message),0,0)==-1){my_err("Problem de reception \n");}
    printf("%i",message.x);
    printf("%c",message.operateur);
    printf("%i",message.y);
    switch (message.operateur){
    case '-':
    resultat=message.x-message.y;
    break;
    case '/':
    resultat=message.x/message.y;
    break;
    case '*':
    resultat=message.x*message.y;
    break;
    case '+':
    resultat=message.x+message.y;
    break;
    default :
    printf("Aucun operateur reconnu\n");
    break;
  }
  printf("\n%i\n",resultat);
  msgsnd(IdDuFichier, &resultat, sizeof(resultat), 0);
  msgctl(IdDuFichier,1,NULL);
}
   return 0;
 }
