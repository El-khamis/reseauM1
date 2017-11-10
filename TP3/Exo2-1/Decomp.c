# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>


void my_err(char * message){
  perror(message);
  exit(1);
}

struct MonNombre{//PAs besoin d'un opérateur on envoie à des prog differents
  long etiquette;
  int x;
};

int main(){
  float temps;
  clock_t t1, t2;
  t1 = clock();
  printf("ékkkk\n");
  struct MonNombre nmbr;
  int pid2=0;
  int nmbrDeFork=0;
  key_t maCle=ftok("./FiFiFichier",10);//Creation de la clef
  int file_id = msgget(maCle, IPC_CREAT|0666);//Creation de la file
  if(file_id==-1){
    my_err("Problem de creation de la file\n");
  }
  printf("Entrez le nombre de fils à créer\n");
  scanf("%d",&nmbrDeFork);

  for(int i=0;i<nmbrDeFork;i++){
    pid2=fork();
    if(pid2==0){
      break;
    }
  }

  if(pid2==0){
    while(1){
      int i=0;
      struct MonNombre nmbrFils;
      printf("Je suis le fils %d\n",getpid()); //Travail du fils
      if(msgrcv(file_id,&nmbrFils,(size_t)sizeof(nmbrFils),(long)1,0)<0){
        my_err("Problem de reception\n");
      }
      else{
        printf("J'ai reçu le nombre %d\n",nmbrFils.x);
      }


      for(i=2;i<=nmbrFils.x/2;i++){
        if(nmbrFils.x%i==0){
          break;
        }
      }
      if((nmbrFils.x/2)+1==i){
        nmbrFils.etiquette=2;
        if(msgsnd(file_id,&nmbrFils,sizeof(nmbrFils),0)<0){my_err("Problem d'envoie\n");}
      }
      else{
        nmbrFils.etiquette=1;
        nmbrFils.x = nmbrFils.x/i;
        if(msgsnd(file_id,&nmbrFils,sizeof(nmbrFils),0)<0){my_err("Problem d'envoie\n");}
        nmbrFils.x=i;
        if(msgsnd(file_id,&nmbrFils,sizeof(nmbrFils),0)<0){my_err("Problem d'envoie\n");}
      }


    }
  }
  else{
    int resultat=1;
    int nombre=0;
      printf("Je suis le père \n"); //Travail du père
      printf("Quels nombre voulez-vous décomposer\n");
      scanf("%d",&(nmbr.x));
      nombre=nmbr.x;
      nmbr.etiquette=1;
      if(msgsnd(file_id,&nmbr,sizeof(nmbr),0)<0){
        my_err("Problem d'envoie\n");
      }
      printf("Message envoyé\n");
      while(resultat!=nombre){
        if(msgrcv(file_id,&nmbr,sizeof(nmbr),2,0)<0) {my_err("Problem de reception\n");}
        resultat=resultat*nmbr.x;
        printf("%d",nmbr.x);
        if (resultat!=nombre) printf("*");
      }
      printf("=%d\n",resultat);

        t2 = clock();
        temps = (float)(t2-t1)/CLOCKS_PER_SEC;
        printf("Calul fait en temps = %f\n", temps);
        kill(0, SIGTERM);
  }

}
