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

void my_err(char * message){
  perror(message);
  exit(1);
}

struct monImage{
  long etiquette;
  int image[20];
}monimage;

int main(){
  struct sembuf op;
  struct monImage *Image;
  int identifiant=0,semaphore_id=0;
  key_t maCle=ftok("./Fichier",10);//Creation de la clef
  identifiant= shmget(maCle,0,IPC_CREAT|0666);//Rejoindre un segment mémoire
  Image=(struct monImage *)shmat(identifiant,NULL,0);
  if((void *)-1==(void *)Image){
    my_err("Erreur pour rejoindre la zone mémoire\n");
  }
  printf("Je suis dans la zone mémoire\n");
  for(int i=0;i<20;i++){
    printf("Image[%d]=%d \n",i,Image->image[i]);
  }
  //Mtn on arrive dans la partie concurentielle
  semaphore_id = semget(maCle, 1, IPC_CREAT | 0666); //Obtention d'un identifiant de sémaphore
  if(semaphore_id==-1){
    my_err("Erreur à la création du sémaphore\n");
  }
  printf("Création du sémaphore réussie\n");
  op.sem_num = 0; //Numéro de notre sémaphore: le premier et le seul
  op.sem_flg = 0; //On ne s'en occupe pas
  for(int i=0;i<20;i++){
    op.sem_op = -1; //Pour un P() on décrémente
   if(-1==semop(semaphore_id, &op, 1)){ //Entrée dans la section critique (P() ou down())
     my_err("Impossible d'entrer dans la semaphore\n");
   }
   if(Image->image[i]==2){
     Image->image[i]=3;
     printf("image[%i]=%i \n",i,Image->image[i]);
     sleep(0.4);
     op.sem_op = 1; //Pour un P() on décrémente
     if(-1==semop(semaphore_id, &op, 1)){ //Sortie de la section critique
       my_err("Impossible de sortie de la section critique\n");
     }
   }
   else{
     op.sem_op = 1; //Pour un P() on décrémente
       i--;
     if(-1==semop(semaphore_id, &op, 1)){ //Sortie de la section critique
       my_err("Impossible de sortie de la section critique\n");
     }
     sleep(1);
   }

  }
return 0;
}
