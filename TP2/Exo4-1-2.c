#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <time.h>

#include <pthread.h>

#define TAILLE 10
//Cette fois l'image est stocké dans la structure et l'index du thread est attribué dans la fonction donc on a une fct par thread.

pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER;


//J'écris dans QuelThreadTravail[i]= moi et je check if(QuelThreadTravail[i]==moi alors je travail sinon j'attend)

struct data {
	int IMAGE[TAILLE];
	int QuelThreadTravail[TAILLE];
}data;

void * Fonction1(void *par){
	int ma_fonction_numero = 1;
	// // sleep(rand()%5);
	struct data *mon_D1 = (struct data*)par;
	  pthread_t moi=pthread_self();
		printf("Je suis %i le premier thread\n",(int)moi);
		 // sleep(rand()%4);
		 for(int i=0; i<TAILLE;i++){
			 pthread_mutex_lock(&verrou);
			 mon_D1->IMAGE[i]=5;
			 mon_D1->QuelThreadTravail[i]=ma_fonction_numero;
			 printf("Le thread %i travail sur la zone Z%i de l'image\n",mon_D1->QuelThreadTravail[i],i);
			 pthread_mutex_unlock(&verrou);
			 // sleep(4);//Juste pour voir que a chaque fois c'est dabord le thread 1 qui travail puis le thread 2
		 }
	 printf("Je suis %i le premier thread j'ai fini mon travail.\n",(int)moi);
	 	pthread_exit(NULL);
 }

 void * Fonction2( void * par){
	 int ma_fonction_numero = 2;
	 pthread_t moi=pthread_self();
	 struct data *mon_D1 = (struct data*)par;
	printf("Je suis %i le second thread\n",(int)moi);
	 	for(int i=0;i<TAILLE;i++){
		 	pthread_mutex_lock(&verrou);
		 	while(mon_D1->QuelThreadTravail[i]!=1){//Tant que la fonction n'a pas fait son travail j'attend
				pthread_mutex_unlock(&verrou);
			 	printf("Jattend que le 1-er thread ait finit de travail sur la case %i\n",i);
			 	pthread_mutex_lock(&verrou);
			}
			//printf("Je suis le 2eme thread, Le 1-er thread a finit de travail sur la case %i\n\nJe commence donc a travailler\n",i);
		 	mon_D1->IMAGE[i]=mon_D1->IMAGE[i]+3;
		 	mon_D1->QuelThreadTravail[i]++;

			printf("Le thread %i travail sur la zone Z%i de l'image\n",mon_D1->QuelThreadTravail[i],i);
		 	pthread_mutex_unlock(&verrou);
		}
		printf("Je suis %i le second thread j'ai fini mon travail.\n",(int)moi);
	  pthread_exit(NULL);
 }

	int main(){
	srand(time(NULL));
	pthread_t T1;
	pthread_t T2;

	struct data D1;
	for(int i=0;i<TAILLE;i++){
		D1.IMAGE[i]=0;
		D1.QuelThreadTravail[i]=0;
	}


		pthread_create(&T2,NULL,Fonction2,&D1);//Second thread
		pthread_create(&T1,NULL,Fonction1,&D1);//Premier Thread

		pthread_join(T1,NULL);
		pthread_join(T2,NULL);



return 0;
	}
