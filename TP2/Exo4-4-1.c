#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <time.h>

#include <pthread.h>

#include <string.h>

//Concatener pour avoir le mot salut


#define TAILLE 1
#define NMBR_THREAD 5 // Nombre de thread.
//Cette fois l'image est stocké dans la structure et l'index du thread est attribué dans le fonction donc on a 2 fct.
//Dupliquer les struct contenant juste leur identité et mettre les tableaux en variable global.



//J'écris dans SurQuoiTravailLeThread[i]= moi et je check if(SurQuoiTravailLeThread[i]==moi alors je travail sinon j'attend)

struct data {
	int IMAGE[TAILLE];
	char mot[TAILLE*21];
	int DernierThreadSurLaZone[TAILLE];
	pthread_mutex_t *verrou;
	pthread_cond_t *condition;
}data;

void* Fonction1(void*);
void* Fonction2(void*);
void* Focntion3(void*);
void* Fonction4(void*);
void* Fonction5(void*);
//Première fonction, n'attend personne et bosse directement sur l'image
void * Fonction1(void *par){
	int ma_fonction_numero = 1;
	struct data *mon_D1 = (struct data*)par;
		 for(int i=0; i<TAILLE;i++){
			 pthread_mutex_lock(&(mon_D1->verrou[i]));
			 mon_D1->IMAGE[i]=mon_D1->IMAGE[i]+5;
			 strcat(mon_D1->mot,"Nique ");
			 mon_D1->DernierThreadSurLaZone[i]=2;
			printf("Le thread numero %i travail sur la partie %i\n",1,i);
			pthread_cond_broadcast(&(mon_D1->condition[i]));
			pthread_mutex_unlock(&(mon_D1->verrou[i]));
		 }
	 	printf("Je suis %i-er le premier thread j'ai fini mon travail.\n",ma_fonction_numero);
	 	pthread_exit(NULL);
 }

 void * Fonction2( void * par){
	 int ma_fonction_numero = 2;
	 struct data *mon_D1 = (struct data*)par;
	 	for(int i=0;i<TAILLE;i++){
			pthread_mutex_lock(&(mon_D1->verrou[i]));
		 	while(mon_D1->DernierThreadSurLaZone[i]!=ma_fonction_numero){//Tant que les fonction précédents n'ont pas fais leurs travails
				pthread_cond_wait(&(mon_D1->condition[i]),&(mon_D1->verrou[i]));
			}
		 	mon_D1->IMAGE[i]=mon_D1->IMAGE[i]+5;
			strcat(mon_D1->mot,"ta ");
			printf("Le thread numero %i travail sur la partie %i\n",2,i);
			mon_D1->DernierThreadSurLaZone[i]++;
			pthread_cond_broadcast(&(mon_D1->condition[i]));
			pthread_mutex_unlock(&(mon_D1->verrou[i]));


		}
		printf("Je suis le second thread j'ai fini mon travail.\n");
	  pthread_exit(NULL);
 }

 void * Fonction3(void*  par){
	 int ma_fonction_numero = 3;
	 struct data *mon_D1 = (struct data*)par;
	 	for(int i=0;i<TAILLE;i++){
			pthread_mutex_lock(&(mon_D1->verrou[i]));
		 	while(mon_D1->DernierThreadSurLaZone[i]!=ma_fonction_numero){//Tant que les fonction précédents n'ont pas fais leurs travails
				pthread_cond_wait(&(mon_D1->condition[i]),&(mon_D1->verrou[i]));
			}
		 	mon_D1->IMAGE[i]=mon_D1->IMAGE[i]+5;
			strcat(mon_D1->mot,"mere ");
			printf("Le thread numero %i travail sur la partie %i\n",3,i);
			mon_D1->DernierThreadSurLaZone[i]++;
			pthread_cond_broadcast(&(mon_D1->condition[i]));
			pthread_mutex_unlock(&(mon_D1->verrou[i]));
		}
		printf("Je suis le troisième thread j'ai fini mon travail.\n");
	  pthread_exit(NULL);
 }

 void * Fonction4(void*  par){
	 int ma_fonction_numero = 4;
	 struct data *mon_D1 = (struct data*)par;
		for(int i=0;i<TAILLE;i++){
			pthread_mutex_lock(&(mon_D1->verrou[i]));
			while(mon_D1->DernierThreadSurLaZone[i]!=ma_fonction_numero){//Tant que les fonction précédents n'ont pas fais leurs travails
				pthread_cond_wait(&(mon_D1->condition[i]),&(mon_D1->verrou[i]));
			}
			mon_D1->IMAGE[i]=mon_D1->IMAGE[i]+5;
			strcat(mon_D1->mot,"la ");
			printf("Le thread numero %i travail sur la partie %i\n",4,i);
			mon_D1->DernierThreadSurLaZone[i]++;
			pthread_cond_broadcast(&(mon_D1->condition[i]));
			pthread_mutex_unlock(&(mon_D1->verrou[i]));
		}
		printf("Je suis %i le quatrième thread j'ai fini mon travail.\n",ma_fonction_numero);
		pthread_exit(NULL);
 }

 void * Fonction5( void * par){
   int ma_fonction_numero = 5;
   struct data *mon_D1 = (struct data*)par;
    for(int i=0;i<TAILLE;i++){
      pthread_mutex_lock(&(mon_D1->verrou[i]));
      while(mon_D1->DernierThreadSurLaZone[i]!=ma_fonction_numero){//Tant que les fonction précédents n'ont pas fais leurs travails
        pthread_cond_wait(&(mon_D1->condition[i]),&(mon_D1->verrou[i]));
      }
      mon_D1->IMAGE[i]=mon_D1->IMAGE[i]+5;
			strcat(mon_D1->mot,"pute");
      printf("Le thread numero %i travail sur la partie %i\n",5,i);
      mon_D1->DernierThreadSurLaZone[i]++;
      pthread_cond_broadcast(&(mon_D1->condition[i]));
      pthread_mutex_unlock(&(mon_D1->verrou[i]));


    }
    printf("Je suis le second thread j'ai fini mon travail.\n");
    pthread_exit(NULL);
 }




	int main(){
	srand(time(NULL));
	pthread_t T1,T2,T3,T4,T5;

	struct data D1;
	for(int i=0;i<TAILLE;i++){
		D1.DernierThreadSurLaZone[i]=0;
		D1.IMAGE[i]=0;
	}

	D1.verrou=malloc(sizeof(pthread_mutex_t)*TAILLE);
 	D1.condition=malloc(sizeof(pthread_cond_t)*TAILLE);

	for(size_t i=0;i<TAILLE;i++){
			pthread_mutex_init(&(D1.verrou[i]),NULL);
	    pthread_cond_init(&(D1.condition[i]),NULL);
	}



		pthread_create(&T2,NULL,Fonction2,&D1);//Second thread
		pthread_create(&T3,NULL,Fonction3,&D1);//Troisième thread
		pthread_create(&T4,NULL,Fonction4,&D1);//Quatrième thread
		pthread_create(&T1,NULL,Fonction1,&D1);//Premier thread
    pthread_create(&T5,NULL,Fonction5,&D1);


		pthread_join(T1,NULL);
		pthread_join(T2,NULL);
		pthread_join(T3,NULL);
		pthread_join(T4,NULL);
    pthread_join(T5,NULL);
		printf("IMAGE(");
		for(int i=0;i<TAILLE-1;i++){
			printf("%i,",D1.IMAGE[i]);
		}
		printf("%i)\n\n\n",D1.IMAGE[TAILLE-1]);
for(int i=0;i<TAILLE*21;i++){
	printf("%c",D1.mot[i]);
}
printf("\n\n");

return 0;
	}
