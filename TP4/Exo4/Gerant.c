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

void my_err(char *message) {
    perror(message);
    exit(1);
};
  struct monSceau{
    int sceau;
    int capaciter;
  }MonSceau;


  union semun { //Pour initialiser le semaphore
      int              val;    /* Valeur pour SETVAL */
      struct semid_ds *buf;    /* Tampon pour IPC_STAT, IPC_SET */
      unsigned short  *array;  /* Tableau pour GETALL, SETALL */
      struct seminfo  *__buf;  /* Tampon pour IPC_INFO*/
  };

int main(){
  struct monSceau * adresse;
  int semaphore_id, identifiant=0;
  union semun egCtrl;
  struct sembuf op;
  egCtrl.val=2;
  key_t maCle=ftok("./Fichier",10);//Creation de la clef
  //Creation du semaphore qui sera utilisé par les autres programmes
  identifiant= shmget(maCle,sizeof(MonSceau),IPC_CREAT|0666);//Créer un segment mémoire
  semaphore_id = semget(maCle,3,IPC_CREAT | IPC_EXCL | 0666); //Obtention d'un identifiant de sémaphore
  if(identifiant==-1){my_err("Impossible de creer la zone mémoire\n");}
  printf("J'ai créer une zone mémoire\n");
  adresse=shmat(identifiant,NULL,0); //Initialise le segment mémoire
  printf("Entrez la capaciter en litres du sceau à remplir\n");
  scanf("%d",&(adresse->capaciter));
  printf("Les équipes doivent remplir un sceau de 10 litres\n");
  adresse->sceau=0;

  if(semaphore_id==-1){my_err("Erreur à la création du sémaphore\n");}
  printf("J'ai créer un sémaphore\n");
  if(semctl(semaphore_id, 0, SETVAL, egCtrl) == -1){//Premier semaphore pour commencer
    my_err("Impossible d'initialiser le sémaphore 0\n");
  }
  printf("J'ai initialiser le semaphore 0\n");
  if(semctl(semaphore_id, 1, SETVAL, egCtrl) == -1){//Deuxieme semaphore 2eme RDV
    my_err("Impossible d'initialiser le sémaphore 1\n");
  }
  printf("J'ai initialiser le semaphore 1\n");
  egCtrl.val=1;
  if(semctl(semaphore_id, 2, SETVAL, egCtrl) == -1){//Exclusion mutuelle
    my_err("Impossible d'initialiser le sémaphore 2\n");
  }
  printf("J'ai initialiser le semaphore 2\n");
  printf("J'ai initialiser mes sémaphore\n");

  while(1){
    op.sem_op=0;
    op.sem_num=1;
    op.sem_flg=0;
    if(-1==semop(semaphore_id, &op, 1)){ //On décrémente le second sémaphore
      my_err("Impossible de sortir du sémaphore\n");
    }
    op.sem_op=2;
    if(-1==semop(semaphore_id, &op, 1)){ //On décrémente le second sémaphore
      my_err("Impossible de sortir du sémaphore\n");
    }
    op.sem_num=0;
    if(-1==semop(semaphore_id, &op, 1)){ //On décrémente le second sémaphore
      my_err("Impossible de sortir du sémaphore\n");
    }
    printf("Vous avez rendu les bambous\n");
  }
  return 0;
}
