#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <time.h>

#include <pthread.h>

#define TAILLE 10

//L'image est une varibale global et la structure contient uniquement un i pour différencier les threads
pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;
int IMAGE[TAILLE];
int QuelThreadTravail[TAILLE];

struct data {
int i;
}data;

void * Travail(void *par){
	sleep(rand()%5);
	struct data *mon_D1 = (struct data*)par;
	  pthread_t moi=pthread_self();
	 if(mon_D1->i==1){
		 printf("Je suis %i le premier thread %i\n",mon_D1->i,(int)moi);
		 sleep(rand()%4);
		 for(int i=0; i<TAILLE;i++){
			 pthread_mutex_lock(&verrou);
			 IMAGE[i]=5;
			 QuelThreadTravail[i]=1;
			 printf("Le thread %i travail sur la zone Z%i de l'image\n",QuelThreadTravail[i],i);
			 pthread_mutex_unlock(&verrou);
			 sleep(1);//Juste pour voir que a chaque fois c'est dabord le thread 1 qui travail puis le thread 2
		 }
	 }
	 else{
		 printf("Je suis %i le deuxième thread %i\n", mon_D1->i,(int)moi);
		 for(int i=0;i<TAILLE;i++){
			 pthread_mutex_lock(&verrou);
			 while(QuelThreadTravail[i]!=1){
				 pthread_mutex_unlock(&verrou);
				// printf("Jattend que le 1-er thread ait finit de travail sur la case %i\n",i);
				 pthread_mutex_lock(&verrou);
			 }
			 IMAGE[i]=IMAGE[i]+3;
			 QuelThreadTravail[i]++;
			 printf("Le thread %i travail sur la zone Z%i de l'image\n",QuelThreadTravail[i],i);
			 pthread_mutex_unlock(&verrou);
		 }
		 printf("Je suis le thread %i j'ai fini en %i-er\n",(int)moi,mon_D1->i);
	 }
	  pthread_exit(NULL);
	}

	int main(){
		srand(time(NULL));
		pthread_t T1;
		pthread_t T2;

		struct data *D1;
		for(int i=0;i<TAILLE;i++){
			IMAGE[i]=0;
			QuelThreadTravail[i]=0;
		}
		D1 = malloc(sizeof(data));
		(*D1).i=1;
		pthread_create(&T1,NULL,Travail,D1);//Premier Thread
		D1 = malloc(sizeof(data));// Allocation dynamique pour leur donné une struct différente à chaque fois
		(*D1).i=2;
		pthread_create(&T2,NULL,Travail,D1);//Second thread
		pthread_join(T1,NULL);
		pthread_join(T2,NULL);
		return 0;
	}
