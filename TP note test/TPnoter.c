//Comment faire cet exercice sans les verrou ?

#include <time.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <pthread.h>
int NMBR_THREAD=0;
struct n_mutex{
  pthread_mutex_t verrou;
  pthread_cond_t condition;
  int nmbr_thread_max;
};

int n_mutex_init(struct n_mutex* v, int n){
  v->nmbr_thread_max=n;
  if(!pthread_mutex_init(&v->verrou,NULL)) {return -1;}
  if(!pthread_cond_init(&v->condition,NULL)) {return -1;}
  if(v->nmbr_thread_max!=n){return -1;}
  return 0;
}
int n_mutex_lock(struct n_mutex *v){
  pthread_mutex_lock(&(v->verrou));
  pthread_t moi=pthread_self();
  if(v->nmbr_thread_max!=NMBR_THREAD){
    NMBR_THREAD++;
    printf("Je suis le thread %d j'attend dans la salle\n",(int)moi);
    printf("Il reste %d places\n",(v->nmbr_thread_max - NMBR_THREAD));
    pthread_mutex_unlock(&(v->verrou));
    pthread_cond_wait(&(v->condition),&(v->verrou));
  }
  else{
    printf("Il n'y a plus de place, sortez\n");
    pthread_mutex_unlock(&(v->verrou));
    pthread_exit(NULL);
    printf("Je suis la\n");
  }
  return 0;
}


int n_mutex_unlock(struct n_mutex *v){
  pthread_t moi= pthread_self();
  printf("le unlock a été lancer\n");
  pthread_mutex_lock(&(v->verrou));
  if(NMBR_THREAD==0){
    pthread_mutex_unlock(&v->verrou);
    printf("Erreur il n'y a pas de thread a libérer\n");
    return -1;
  }
  else{
    NMBR_THREAD--;
    printf("Je libére le thread %d il reste %d places\n",(int)moi,NMBR_THREAD);
    pthread_mutex_unlock(&v->verrou);
    return (pthread_cond_signal(&(v->condition)));
  }
}

int n_mutex_destroy(struct n_mutex *v){
  free(v);
  return 0;
}

void * utiliserParThread(void * par){
  struct n_mutex *mon_D1 = (struct n_mutex*)par;
  int moi=pthread_self();
  printf("Je suis le thread %d j'essaie de rentrer dans la salle\n",(int)moi);
  n_mutex_lock(mon_D1);
  sleep(2);
  n_mutex_unlock(mon_D1);
  printf("Jsuis laaa\n");
  pthread_exit(NULL);
}
int main(){
struct n_mutex mon_mut;
n_mutex_init(&mon_mut,2);
pthread_t T[6];
for(int i=0;i<6;i++){
  pthread_create(&T[i],NULL,utiliserParThread,&mon_mut);
}

for(int i=0;i<6;i++){
  pthread_join(T[i],NULL);
}

return 0;
}
