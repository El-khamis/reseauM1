# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/msg.h>
# include <unistd.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/types.h>
# include <wait.h>
#include <time.h>

void my_err(char * message){
  perror(message);
  exit(1);
}

struct MonSceau{//PAs besoin d'un opérateur on envoie à des prog differents
  int sceau;
  int capaciter;
};

int main(){
  struct MonSceau *Sceau;
  int semaphore_id, identifiant;
  struct sembuf op;
  int pid2=1;
  key_t maCle=ftok("./FiFiFichier",10);//Creation de la clef
  int file_id = msgget(maCle, IPC_CREAT|0666);//Creation de la file pour l'envoie de message
  //On choppe le sémaphore
  identifiant= shmget(maCle,0,IPC_CREAT|0666);//Demande d'attache à un segment mémoire
  Sceau=shmat(identifiant,NULL,0); //Initialise la structure



  printf("Vous devez remplir un sceau de %d litres et il contient pour l'instant %d litres \n",Sceau->capaciter, Sceau->sceau);
  semaphore_id = semget(maCle, 1, IPC_CREAT | 0666); //Obtention d'un identifiant de sémaphore
  if(semaphore_id==-1){
    my_err("Erreur à la création du sémaphore\n");
  }
  printf("Création du sémaphore réussie\n");
  op.sem_num = 0; //Numéro de notre sémaphore: le premier et le seul
  op.sem_flg = 0; //On ne s'en occupe pas
  if(file_id==-1){
    my_err("Problem de creation de la file\n");
  }
  // Afficher le nombre de ressource pour chaque sémaphore


  for(int i=0;i<4;i++){
    pid2=fork();
    if(pid2==0){
      break;
    }
  }
//Fin des forks on sépare le code pour les fils et le père


  if(pid2==0){//Fils (mais on sait pas qui et on s'enfou y'en a deux qu attende sion les autres taff )
  struct sembuf TableauOpe[2];
  struct sembuf TableauOpe2[1];
  TableauOpe2[0].sem_num = 0;//Je  décremente la valeur
  TableauOpe2[0].sem_op = -1;
  TableauOpe2[0].sem_flg = 0;
  srand(time(NULL));
      while(Sceau->sceau < Sceau->capaciter){
        printf("Je suis le fils %d, j'attend de prendre un bambou \n",getpid());
        if(-1==semop(semaphore_id, TableauOpe2,1)){ //Entrée dans la section critique (P() ou down())
           my_err("Impossible d'entrer dans la semaphore\n");
         }
    /*  op.sem_op=0;
      //Première attente
      if(-1==semop(semaphore_id, &op, 1)){ //Attente des deux premier candidats
         my_err("Impossible d'entrer dans la semaphore\n");
       }*/
      printf("Je suis le fils %d je cours à la bassine !\n",getpid());
      int val=rand()%5;
      int val2=rand()%10;
      sleep(val);
      printf("Je suis le fils %d j'ai fini de chercher l'eau en %d secondes et j'ai ramener %d litres \n",getpid(),val,val2);
      //DEBUT DE SECTION CRITIQUE on rempli pas en même temps;
      op.sem_op=-1;
      op.sem_num=2;
      if(-1==semop(semaphore_id, &op, 1)){ //On décrémente le second sémaphore
        my_err("Impossible d'entrer dans la semaphore\n");
      }
      printf("Je suis le fils %d,et j'ai rempli le sceau de %d litres\n",getpid(),val2);
      Sceau->sceau+=val2;
      op.sem_op=1;
      op.sem_num=2;
      if(-1==semop(semaphore_id, &op, 1)){ //On décrémente le second sémaphore
        my_err("Impossible d'entrer dans la semaphore\n");
      }
      //FIN DE SECTION CRITIQUE
      struct sembuf DecremAndWait[2];
      DecremAndWait[0].sem_op=-1;
      DecremAndWait[0].sem_num=1;
      DecremAndWait[0].sem_flg=0;
      DecremAndWait[1].sem_op=0;
      DecremAndWait[1].sem_num=1;
      DecremAndWait[1].sem_flg=0;

      printf("Je suis le fils %d et je suis au premier rendez-vous\n",getpid());
      if(-1==semop(semaphore_id, DecremAndWait, 1)){ //On décrémente le second sémaphore
        my_err("Impossible de sortir du sémaphore\n");
      }
      //Mtn on remet tous les semaphore à leur valeur initiale
      TableauOpe[0].sem_num = 0;//Je incremente la valeur
      TableauOpe[0].sem_op = 1;
      TableauOpe[0].sem_flg = 0;
      TableauOpe[1].sem_num = 1;//J'incremente la value
      TableauOpe[1].sem_op =  1;
      TableauOpe[1].sem_flg = 0;
      if(-1==semop(semaphore_id, TableauOpe, 2)){ //Attente des deux premier candidats
         my_err("Impossible d'entrer dans la semaphore\n");
      }
      printf("Je suis le fils %d nous avons rempli le sceau de %d litres il y a maintenant %d lites sur un total de %d ",getpid(),val2,Sceau->sceau, Sceau->capaciter);
      printf("On a rendu les bambous\n");
    }
    exit(1);
  }
  else{ //Pere
    wait(NULL);
    printf("Bravo le sceau a été rempli, il contient %d litres \n",Sceau->sceau);
    kill(0, SIGTERM);
  }
}
