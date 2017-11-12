//Comment faire cet exercice sans les verrou ?

#include <time.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <pthread.h>


pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;
struct data {
    int *Tab1;
    int *Tab2;
    int *Resultat;//Jfface surement ce tableau a chaque fois
    int taille;
    int i;//Je change le i à chaque fois
};

void * Som(void * par){
  int *somme=(int *)malloc(sizeof(int));
  pthread_t moi = pthread_self();
  struct data *mon_D1 = (struct data*)par;
  printf("Je suis le thread %lu, je fais la somme\n",moi);
 pthread_mutex_lock(&verrou);
  printf("J'ai verrouillé 1er fois \n");

  while( ((*mon_D1).i)!=(*mon_D1).taille  ){
    pthread_mutex_unlock(&verrou);
  //  printf("J'ai déverrouillé \n");

    pthread_mutex_lock(&verrou);
  //  printf("J'ai verrouillé \n");
  }
    //printf("Je suis le thread %lu , ")
    printf("Tous les calculs ont été fait je peux commencer la somme\n");
    for(int z=0; z<(*mon_D1).taille;z++){
    //  printf("%i\n",(*mon_D1).Resultat[z]);
        *somme=*somme+(*mon_D1).Resultat[z];
      }
      pthread_mutex_unlock(&verrou);
      pthread_exit(somme);
    }

//Creation de ma fonction utilisé par le thread
void * Mul(void * par){
  pthread_t moi = pthread_self();
  int resultat;
   struct data *mon_D1 = (struct data*)par; //Initialisation de ma struct par la structure passé en parametre via son adresse
   pthread_mutex_lock(&verrou); //On verrouille notre variable global
   resultat=(*mon_D1).Tab1[(*mon_D1).i] * (*mon_D1).Tab2[(*mon_D1).i] ;//Le calcul
   (*mon_D1).Resultat[(*mon_D1).i]=resultat;
   (*mon_D1).i++;
  // printf("Je suis le thread %lu, je fais la multiplication de la %i-eme ligne, mon resultat est : %i\n",moi, (*mon_D1).i,resultat);
   pthread_mutex_unlock(&verrou);

  pthread_exit(NULL);
}


int main(){
  //CALCUL  DU TEMPS DEXECUTION
  float temps;
  clock_t t1, t2;
  t1 = clock();

  FILE * fp;

   fp = fopen ("file.txt", "r");

  //Déclaration des vecteurs
  int taille=0;
  int somme=5;

//RECUPERATION DES VECTEURS


  printf("Entrez la taille de vos vecteurs\n");
  //scanf("%i",&taille);
  fscanf(fp, "%d",&taille);
  int *T1=malloc(sizeof(int)*taille), *T2=malloc(sizeof(int)*taille), *T3=malloc(sizeof(int)*taille);

  printf("Entrez la valeur du premier vecteur\n");
  for(int i=0;i<taille;i++){
  //  printf("La valeur du %i-eme parametre: ",i+1);
    //scanf("%i",&T1[i]);
    fscanf(fp, "%d",&T1[i]);

  }

  printf("Entrez la valeur du deuxieme vecteur\n");
  for(int i=0;i<taille;i++){
//    printf("La valeur du %i-eme parametre: ",i+1);
    //scanf("%i",&T2[i]);
    fscanf(fp, "%d",&T2[i]);

  }


//CALCULS



  struct data D1; D1.Tab1=T1; D1.Tab2=T2; D1.Resultat=T3; D1.taille=taille; D1.i=0; // On initialise notre struct
  //Initialisation d'un tableau de thread
  pthread_t ThreadDeSomme; pthread_create(&ThreadDeSomme,NULL,Som,&D1);

  pthread_t *TabDeThread = malloc(taille*sizeof(pthread_t));
  //Creation d'autant de thread que de variable dans un vecteur
  for(int i=0;i<taille;i++){
    pthread_create(&(TabDeThread[i]),NULL,Mul,&D1);// Le thread se lance avec les bonnes variables
  }
  for(int i=0;i<taille;i++){
    pthread_join(TabDeThread[i],NULL);// on retourne la valeur
  }


//RECUPERATION DU RESULTAT

  void *vptr_return;
  pthread_join(ThreadDeSomme,&vptr_return);
  somme= *((int *)vptr_return);
  printf("La somme des vecteurs : V1(");
  for(int i=0;i<taille-1;i++){
    printf("%i,",T1[i]);
  }
  printf("%i) et V2(",T1[taille-1]);

  for(int i=0;i<taille-1;i++){
    printf("%i,",T2[i]);
  }
  printf("%i) a pour resultat : %i\n",T2[taille-1], somme);

  t2 = clock();
    temps = (float)(t2-t1)/CLOCKS_PER_SEC;
    printf("Calul fait en temps = %f\n", temps);

  return 0;
}
