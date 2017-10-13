#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <pthread.h>
int variable_global=0;
pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;
struct data {

    int *Tab1;
    int *Tab2;
    int i;


};
//Creation de ma fonction utilisé par le thread
void * Mul(void * par){
  pthread_t moi = pthread_self();
  int resultat;
   struct data *mon_D1 = (struct data*)par; //Initialisation de ma struct par la structure passé en parametre via son adresse
   pthread_mutex_lock(&verrou); //On verrouille notre variable global
   resultat=(*mon_D1).Tab1[(*mon_D1).i] * (*mon_D1).Tab2[(*mon_D1).i] ;//Le calcul
   printf("Je suis le thread %lu\n",moi);
   (*mon_D1).i++;
   variable_global+=resultat;
   pthread_mutex_unlock(&verrou);

  pthread_exit(NULL);
}


int main(){
  //Déclaration des vecteurs
  int taille=0;
  printf("Entrez la taille de vos vecteurs\n");
  scanf("%i",&taille);
  int *T1=malloc(sizeof(int)*taille), *T2=malloc(sizeof(int)*taille);

  printf("Entrez la valeur du premier vecteur\n");
  for(int i=0;i<taille;i++){
    printf("La valeur du %i-eme parametre: ",i);
    scanf("%i",&T1[i]);
  }

  printf("Entrez la valeur du deuxieme vecteur\n");
  for(int i=0;i<taille;i++){
    printf("La valeur du %i-eme parametre: ",i);
    scanf("%i",&T2[i]);
  }
  struct data D1; D1.Tab1=T1; D1.Tab2=T2; D1.i=0;// On initialise notre struct
  //Initialisation d'un tableau de thread

  pthread_t *TabDeThread = malloc(taille*sizeof(pthread_t));
  //Creation d'autant de thread que de variable dans un vecteur
  for(int i=0;i<taille;i++){
    pthread_create(&(TabDeThread[i]),NULL,Mul,&D1);// Le thread se lance avec les bonnes variables
  }
  for(int i=0;i<taille;i++){
    pthread_join(TabDeThread[i],NULL);// on retourne la valeur
  }
  printf("La somme des vecteurs : V1(");
  for(int i=0;i<taille-1;i++){
    printf("%i,",T1[i]);
  }
  printf("%i) et V2(",T1[taille-1]);

  for(int i=0;i<taille-1;i++){
    printf("%i,",T2[i]);
  }
  printf("%i) a pour resultat : %i\n",T2[taille-1], variable_global);

  return 0;
}
