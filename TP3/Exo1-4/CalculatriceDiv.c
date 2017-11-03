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
struct MonCalcul{
  long etiquette;
  int x;
  int y;
};

struct resultat{
  long etiquette;
  int res;
};
int main(){
   key_t maCle=ftok("./FiFiFichier",10);
   int file_id = msgget(maCle, IPC_CREAT|0666);
   struct MonCalcul operation;
   while(1){
     printf("J'attend la reception d'un message\n");
     if(msgrcv(file_id,&operation,sizeof(operation),(long)4,0)<0){//Je ne recois que sur le canal 1
       my_err("Problem de reception\n");
     }
     printf("%i",operation.x);
     printf("/");
     printf("%i\n",operation.y);
     printf("Message reçu\n");
     struct resultat monResultat;
     monResultat.etiquette=1;
     monResultat.res=operation.x/operation.y;
     if(msgsnd(file_id,&monResultat,sizeof(monResultat),0)<0){
       my_err("Problem d'envoie");
     }
     printf("Le resultat a été envoyé\n");
     msgctl(file_id,1,NULL);
   }
   return 0;
 }
