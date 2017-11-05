# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/msg.h>
# include <unistd.h>
# include <sys/ipc.h>
# include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

void my_err(char *message) {
    perror(message);
    exit(1);
};

struct Parking{
  int nmbrDePlace;
}parking;

int main(){
  struct Parking *adresse;
  char lettre;
  int identifiant;
  int semaphore_id;
  key_t maCle=ftok("./Fichier",10);//Creation de la clef
  identifiant= shmget(maCle,sizeof(parking),IPC_CREAT|0666);//Créer un segment
  if(identifiant==-1){
    my_err("Impossible de creer la zone mémoire\n");
  }
  printf("J'ai créer une zone mémoire\n");
  //Mtn je m'attache et jecris
  adresse=(struct Parking *)shmat(identifiant,NULL,0);
  adresse->nmbrDePlace=50;
  //Creation du semaphore qui sera utilisé par les autres programmes
  semaphore_id = semget(maCle, 1, IPC_CREAT | IPC_EXCL | 0666); //Obtention d'un identifiant de sémaphore, echoue si le sémaphore existe déjà
  if(semaphore_id==-1){
    my_err("Erreur à la création du sémaphore\n");
  }
  semctl(semaphore_id, 0, SETVAL, 1);
  printf("Bonjour  et bienvenue dans notre parking de %i places\n",adresse->nmbrDePlace);
  printf("Appuyez sur q pour quitter\n");
  scanf(" %c",&lettre);
  printf("Suppression du segment mémoire\n");
  if(0>shmctl(identifiant,IPC_RMID,NULL)){
    my_err("Impossible de supprimer le segment memoire\n");
  }
  if(0>semctl(semaphore_id, 0, IPC_RMID)){
    my_err("Impossible de supprimer le semaphore\n");
  }
  printf("Suppression du sémaphore\n");
  if(adresse->nmbrDePlace==50){
    printf("Notre parking est vide\n");
  }
  else{
    printf("Il reste dans notre parking %i places, aurevoir \n",adresse->nmbrDePlace);
  }
  return 0;
}
