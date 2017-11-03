<<<<<<< HEAD
=======
//presque la bonne solution mais y'a un soucis si y'a plusieurs rendez vous
>>>>>>> 3f0eab65a7cc140da69442e19705cadc5f53331b

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <time.h>

#include <pthread.h>

int variable_global=0;
int variable_global2=0;
int dernierThread=0;

pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

void * travail(void* par){
  int moi= pthread_self();
  printf("Je suis le thread %i avant le rendez vous\n",moi);// Ajouter de l'affichage
  sleep(rand()%5);
  pthread_mutex_lock(&verrou);
  variable_global++;// Ajouter de l'affichage
  if(variable_global==1){printf("je suis le thread %i et je suis arriver au rendez-vous en %i-er\n",moi,variable_global);} else{ printf("je suis le thread %i et je suis arriver au rendez-vous en %i-eme\n",moi,variable_global);}
  if(variable_global==variable_global2){dernierThread=moi;}
  while(variable_global!=variable_global2){ // Il n'est plus respecter
<<<<<<< HEAD
=======
  //  printf("\nLa variable global vaut %i, variable_global2 : %i\n",variable_global, variable_global2);
>>>>>>> 3f0eab65a7cc140da69442e19705cadc5f53331b
    pthread_cond_wait(&condition, &verrou);
  } //Tant que c'est pas fini j'attend
  pthread_mutex_unlock(&verrou);//Pourquoi on déverouille le verrou alors que le pthread_cond_wait est censé l'avoir déjà fait ? Car On rperend le veerrou juste après le reveil

  if (moi==dernierThread){
    printf("Je suis le thread %i et je vous dis de vous arrêtez !\n",moi);
    pthread_cond_broadcast(&condition);
    sleep(rand()%5);
  }
   printf("Je m'arrête \n");
   pthread_exit(NULL);
}

int main() {
  srand(time(NULL));
  int taille;
  int nombre;
  printf("Entrez le nombre de thread a créer");
  scanf("%i",&taille);
  variable_global2=taille;
  printf("Entre le nombre de rendez-vous que vous voulez");
  scanf("%i",&nombre);
  pthread_t *T=malloc(sizeof(pthread_t)*taille);
  for(int k=0; k<nombre; k++){
    sleep(3);
    if(k+1==1){printf("\n\n\n\n %i-er rendez-vous \n",k+1);}else{printf("\n\n\n\n %i-eme rendez-vous \n",k+1);}
   for(int i=0;i<taille;i++){
      pthread_create(&T[i],NULL,travail,NULL);
    }

    for(int i=0;i<taille;i++){
      pthread_join(T[i],NULL);
    }
    printf("Toutes les tâches se sont terminé je peux fini le main \n");

      pthread_mutex_lock(&verrou);
      variable_global=0;// Ajouter de l'affichage
      pthread_mutex_unlock(&verrou);
    }
return 0;
}
