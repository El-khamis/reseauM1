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
# include <time.h>

void my_err(char *message) {
    perror(message);
    exit(1);
};


struct Parking{
  int nmbrDePlace;
};

int main(){

  struct Parking * adresse;
  struct sembuf op;
  int identifiant=0,semaphore_id=0;
  key_t maCle=ftok("./Fichier",10);//Creation de la clef

  identifiant= shmget(maCle,0,IPC_CREAT|0666);//Rejoindre un segment
  adresse=(struct Parking *)shmat(identifiant,NULL,0);

  if((void *)-1==(void *)adresse){
    my_err("Erreur pour rejoindre la zone mémoire\n");
  }
  printf("Je suis dans la zone mémoire\n");

  semaphore_id = semget(maCle, 1, IPC_CREAT | 0666); //Obtention d'un identifiant de sémaphore
  if(semaphore_id==-1){
    my_err("Erreur à la création du sémaphore\n");
  }
  printf("On a obtenu le sémaphore\n");
  op.sem_num = 0; //Numéro de notre sémaphore: le premier et le seul
  op.sem_flg = 0; //On ne s'en occupe pas
  while(1){
    sleep(2);
    //Entree section critique
    op.sem_op = -1; //Pour un P() on décrémente
   semop(semaphore_id, &op, 1); //Entrée dans la section critique (P() ou down())
    if(adresse->nmbrDePlace>49){
      printf("Il n'y a personne dans le parking vous ne pouvez pas sortir \n");
    }
    else{
      printf("Aurevoir merci d'avoir utiliser notre parking ");
      adresse->nmbrDePlace++;
      printf("Il y a %i places disponibles\n", adresse->nmbrDePlace);
    }
    //sortie section critique
    op.sem_op = 1; //Pour un V() on incrémente
    semop(semaphore_id, &op, 1); //Sortie de la section critique (V() ou up())
  }
  if(-1==shmdt((void *)adresse)){
    my_err("Impossible de quitter la zone mémoire\n");
  }
  printf("J'ai quitté la zone mémoire\n");
return 0;
}
