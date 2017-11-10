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

//Je veux que mes prog s'arrêtent quand je ferme le parking
void my_err(char *message) {
    perror(message);
    exit(1);
};
struct Parking{
  int nmbrDePlace;
};

int main(){
  struct sembuf op;
  srand (time (NULL));
  struct Parking * adresse;
  int identifiant=0, semaphore_id=0;
  key_t maCle=ftok("./Fichier",10);//Creation de la clef

  identifiant= shmget(maCle,0,IPC_CREAT|0666);//Rejoindre un segment
  adresse=(struct Parking *)shmat(identifiant,NULL,0);

  if((void *)-1==(void *)adresse){
    my_err("Erreur pour rejoindre la zone mémoire\n");
  }
  printf("Je suis dans la zone mémoire\n");
  //Creation et initialisation du sémaphore

  semaphore_id = semget(maCle, 1, IPC_CREAT | 0666); //Obtention d'un identifiant de sémaphore
  if(semaphore_id==-1){
    my_err("Erreur à la création du sémaphore\n");
  }
  printf("Création du sémaphore réussie\n");
  op.sem_num = 0; //Numéro de notre sémaphore: le premier et le seul
  op.sem_flg = 0; //On ne s'en occupe pas
  while(1){
    //Entrée en section critique
    op.sem_op = -1; //Pour un P() on décrémente
    sleep(1);
   if(-1==semop(semaphore_id, &op, 1)){ //Entrée dans la section critique (P() ou down())
     my_err("Impossible de récuperer le semaphore\n");
   }
   printf("J'ai obtenu le sémaphore ");
    if(adresse->nmbrDePlace>0){
      printf("Vous pouvez rentrez dans le parking: ");
      printf("vous avez le ticket numero %i\n",rand()%100);
      adresse->nmbrDePlace--;
      printf("Il reste %i places disponibles\n",adresse->nmbrDePlace);
    }
    else{
      printf("Il n y a pas de places disponibles demande rejetée\n");
    }
    //Sortie de section critique
    op.sem_op = 1; //Pour un V() on incrémente
    if(-1==semop(semaphore_id, &op, 1)){ //Sortie de la section critique (V() ou up())
      my_err("Impossible de récuperer le semaphore\n");
    }
    printf("Je libère le sémaphore\n");
  }
  printf("%i\n",adresse->nmbrDePlace);

  if(-1==shmdt((void *)adresse)){
    my_err("Impossible de quitter la zone mémoire\n");
  }
  printf("J'ai quitté la zone mémoire\n");
return 0;
}
