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
  //On choppe le sémaphore
  identifiant= shmget(maCle,0,IPC_CREAT|0666);//Demande d'attache à un segment mémoire
  Sceau=shmat(identifiant,NULL,0); //Initialise la structure



  printf("Bonjour l'équipe bleu vous devez remplir le sceau de %d litres et il est pour l'instant vide \n",Sceau->capaciter);
  semaphore_id = semget(maCle, 1, IPC_CREAT | 0666); //Obtention d'un identifiant de sémaphore
  if(semaphore_id==-1){
    my_err("Erreur à la création du sémaphore\n");
  }
  printf("Création du sémaphore réussie\n");
  op.sem_num = 0; //Numéro de notre sémaphore: le premier et le seul
  op.sem_flg = 0; //On ne s'en occupe pas

  // Afficher le nombre de ressource pour chaque sémaphore


  for(int i=0;i<4;i++){
    pid2=fork();
    if(pid2==0){
      break;
    }
  }
//Fin des forks on sépare le code pour les fils et le père


  if(pid2==0){//Fils (mais on sait pas qui et on s'enfou y'en a deux qu attende sion les autres taff )
    op.sem_num = 0;//Je  décremente la valeur
    op.sem_op = -1;
    op.sem_flg = IPC_NOWAIT;
    srand(time(NULL));
    while(Sceau->sceau < Sceau->capaciter){
      if(-1==semop(semaphore_id, &op,1)){ //Entrée dans la section critique (P() ou down())
         //my_err("Impossible d'entrer dans la semaphore\n");
         op.sem_flg = 0;
         printf("%d, j'attend de prendre un bambou \n",getpid()%100);
         if(-1==semop(semaphore_id, &op,1)){ //Entrée dans la section critique (P() ou down())
            //my_err("Impossible d'entrer dans la semaphore\n");
          }
          sleep(0.3);
       }
       if(Sceau->sceau >= Sceau->capaciter){
         exit(1);
       }
       printf("%d et j'ai pris un bambou ",getpid()%100);
      printf("je cours à la bassine !\n");
      int val=rand()%5;
      int val2=rand()%10;
      sleep(val);
      printf("%d j'ai fini de chercher l'eau en %d secondes et j'ai ramener %d litres \n",getpid()%100,val,val2);
      //DEBUT DE SECTION CRITIQUE on rempli pas en même temps;
      op.sem_op=-1;
      op.sem_num=2;
      op.sem_flg=0;
      if(-1==semop(semaphore_id, &op, 1)){ //On décrémente le second sémaphore
        my_err("Impossible d'entrer dans la section critique\n");
      }
      printf("\n\n\nJ ENTRE EN SECTION CRITIQUE \n\n\n");
      printf("%d,et j'ai rempli le sceau de %d litres\n",getpid()%100,val2);
      Sceau->sceau+=val2;
      op.sem_op=1;
      op.sem_num=2;
      printf("\n\n\nFIN EN SECTION CRITIQUE \n\n\n");
      if(-1==semop(semaphore_id, &op, 1)){ //On décrémente le second sémaphore
        my_err("Impossible de sortir de la section critique\n");
      }
      //FIN DE SECTION CRITIQUE
      op.sem_op=-1;
      op.sem_num=1;
      op.sem_flg=0;


      printf("%d et je suis au premier rendez-vous\n",getpid()%100);
      if(-1==semop(semaphore_id, &op, 1)){ //On décrémente le second sémaphore
        my_err("Impossible de sortir du sémaphore\n");
      }
      printf("J'ai décrementer le semaphore mtn j'attend\n");
      op.sem_op=0;
      if(-1==semop(semaphore_id, &op, 1)){ //On décrémente le second sémaphore
        my_err("Impossible de sortir du sémaphore\n");
      }
      printf("%d mon voisin est la ",getpid()%100);
      //Mtn on remet tous les semaphore à leur valeur initiale
      printf("Nous avons rempli le sceau de %d litres chacun il y a maintenant %d : ",val2,Sceau->sceau);
      printf("On a rendu les bambous\n");
      sleep(0.2);
      if(Sceau->sceau >= Sceau->capaciter){
        exit(1);
      }
    }
    exit(1);
  }
  else{ //Pere
    wait(NULL);
    printf("Bravo le sceau a été rempli, il contient %d litres \n",Sceau->sceau);
    kill(0, SIGTERM);
  }
}
