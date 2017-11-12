# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
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
  struct monImage{
    long etiquette;
    int image[20];
  }monimage;

  union semun {
      int              val;    /* Valeur pour SETVAL */
      struct semid_ds *buf;    /* Tampon pour IPC_STAT, IPC_SET */
      unsigned short  *array;  /* Tableau pour GETALL, SETALL */
      struct seminfo  *__buf;  /* Tampon pour IPC_INFO*/
  };

int main(){
  struct monImage * adresse;
  int semaphore_id, identifiant=0;
  union semun egCtrl;
  egCtrl.val=1;
  key_t maCle=ftok("./Fichier",10);//Creation de la clef
  //Creation du semaphore qui sera utilisé par les autres programmes
  semaphore_id = semget(maCle,1, IPC_CREAT | IPC_EXCL | 0666); //Obtention d'un identifiant de sémaphore
  identifiant= shmget(maCle,sizeof(monimage),IPC_CREAT|0666);//Créer un segment mémoire
  if(identifiant==-1){my_err("Impossible de creer la zone mémoire\n");}
  printf("J'ai créer une zone mémoire\n");
  adresse=(struct monImage *)shmat(identifiant,NULL,0); //Initialise le segment mémoire
  for(int i=0;i<20;i++){
    adresse->image[i]=0;
  }
  if(semaphore_id==-1){my_err("Erreur à la création du sémaphore\n");}
  printf("J'ai créer un sémaphore\n");
  if(semctl(semaphore_id, 0, SETVAL, egCtrl) == -1){
    my_err("Impossible d'initialiser le sémaphore 0\n");
  }
  printf("J'ai initialiser mon sémaphore\n");
  return 0;
}
