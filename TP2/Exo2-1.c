//Comment faire cet exercice sans les verrou

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <pthread.h>
struct data {

    int *Tab1;
    int *Tab2;
    int i;


};

pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;

//Creation de ma fonction utilisé par le thread
void * Mul(void * par){
  pthread_t moi =pthread_self();
  int *res=(int *)malloc(sizeof(int));// On créer une variable dynamiquement Sinon on peut pas la renvoyer en dehors du thread
   struct data *mon_D1 = (struct data*)par; //Initialisation de ma variable par la structure passé en parametre via son adresse
   *res=(*mon_D1).Tab1[(*mon_D1).i] * (*mon_D1).Tab2[(*mon_D1).i] ;//Le calcul
   pthread_mutex_lock(&verrou);
   (*mon_D1).i++;
   pthread_mutex_unlock(&verrou);

   printf("Je suis le thread %lu, je fais la multiplication de la %i-eme ligne, mon resultat est : %i\n",moi, (*mon_D1).i,*res);
  pthread_exit(res);//On retourne l'adresse de res
}


int main(){
  //Déclaration des vecteurs
  int taille=0;
  printf("Entrez la taille de vos vecteurs\n");
  scanf("%i",&taille);
  int *T1=malloc(sizeof(int)*taille), *T2=malloc(sizeof(int)*taille);

  printf("Entrez les coordonnées du premier vecteur\n");
  for(int i=0;i<taille;i++){
    printf("La valeur du %i-eme parametre: ",i);
    scanf("%i",&T1[i]);
  }

  printf("Entrez les coordonnées du deuxieme vecteur\n");
  for(int i=0;i<taille;i++){
    printf("La valeur du %i-eme parametre: ",i);
    scanf("%i",&T2[i]);
  }
  struct data D1; D1.Tab1=T1; D1.Tab2=T2; D1.i=0; // On initialise notre struct avec les valeurs des vecteurs
  //Initialisation d'un tableau de thread

  pthread_t *TabDeThread = malloc(taille*sizeof(pthread_t));
  int somme=0,resultat=0;
  void *vptr_return; // ?? Obligé de fair ça sinon ça ne marche pas

  //Creation d'autant de thread que de variable dans un vecteur
  for(int i=0;i<taille;i++){
    pthread_create(&(TabDeThread[i]),NULL,Mul,&D1);// Le thread se lance avec les bonnes variables
    //sleep(1);
  }
  for(int i=0;i<taille;i++){
    pthread_join(TabDeThread[i],&vptr_return);// on retourne la valeur
    somme= *((int *)vptr_return); // on cast en int et on récupère la valeur
    resultat=resultat+somme;
  }
  free(vptr_return);
  printf("La somme des vecteurs : V1(");
  for(int i=0;i<taille-1;i++){
    printf("%i,",T1[i]);
  }
  printf("%i) et V2(",T1[taille-1]);

  for(int i=0;i<taille-1;i++){
    printf("%i,",T2[i]);
  }
  printf("%i) a pour resultat : %i\n",T2[taille-1], resultat);

  return 0;
}
