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

  union semun {
      int              val;    /* Valeur pour SETVAL */
      struct semid_ds *buf;    /* Tampon pour IPC_STAT, IPC_SET */
      unsigned short  *array;  /* Tableau pour GETALL, SETALL */
      struct seminfo  *__buf;  /* Tampon pour IPC_INFO*/
  };
int main(){
  int semaphore_id;
  union semun egCtrl;
  struct sembuf op;
  op.sem_num = 0; //Numéro de notre sémaphore: le premier et le seul
  op.sem_flg = 0; //On ne s'en occupe pas
  op.sem_op=0;//Ici 0 car on va juste attendre
  egCtrl.val=3;
  key_t maCle=ftok("./Fichier",10);//Creation de la clef
  //Creation du semaphore qui sera utilisé par les autres programmes

  semaphore_id = semget(maCle,1, IPC_CREAT | IPC_EXCL | 0666); //Obtention d'un identifiant de sémaphore, echoue si le sémaphore existe déjà
  if(semaphore_id==-1){
    my_err("Erreur à la création du sémaphore\n");
  }
  printf("J'ai créer un sémaphore\n");

  if(semctl(semaphore_id, 0, SETVAL, egCtrl) == -1){
    my_err("Impossible d'initialiser le sémaphore 0\n");
  }
  printf("J'ai initialiser mon sémaphore\n");

  return 0;
}
