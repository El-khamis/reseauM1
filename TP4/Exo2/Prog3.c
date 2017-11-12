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
#include <semaphore.h>


void my_err(char *message) {
    perror(message);
    exit(1);
};

int main(){
  struct sembuf op;
  int Resultat[1];
  int semaphore_id=0;
  key_t maCle=ftok("./Fichier",10);//Creation de la clef

  semaphore_id = semget(maCle, 1, IPC_CREAT | 0666); //Obtention d'un identifiant de sémaphore
  if(semaphore_id==-1){
    my_err("Erreur à la création du sémaphore\n");
  }
  printf("Je travail \n");
  sleep(3);
  //On décremente

  op.sem_num = 0; //Numéro de notre sémaphore: le premier et le seul
  op.sem_flg = 0; //On ne s'en occupe pas
  op.sem_op = -1; //Pour un P() on décrémente
  semop(semaphore_id, &op, 1); //Entrée dans la section critique (P() ou down())
  printf("Je suis dans la section critique\n");
  //On attend
  if(semctl(semaphore_id, 0, GETVAL, Resultat) == -1){
    my_err("Impossible d'initialiser le sémaphore 0\n");
  }
    printf("Res[%i]=%i\n",1,Resultat[1]);

  op.sem_op = 0; //Pour un P() on décrémente
  printf("Je suis au rendez-vous\n");

  semop(semaphore_id, &op, 1); //Entrée dans la section critique (P() ou down())


   printf("Je quitte le rdv\n");
   return 0;
}
