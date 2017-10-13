//ça marche soucis lorsque un thread est verrouillé il n'est jamais libéré
#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <pthread.h>
int NMBR_PLACE_DISPONIBLE=3;
struct n_verrou{
  pthread_mutex_t verrou;
  pthread_cond_t condition;
  int nmbr_place_max;
};

int traitement1(){sleep(1); return 0;}
int  traitement2(){sleep(2);return 0;}
int  traitement3(){sleep(1);return 0;}


int n_verrou_init(struct n_verrou* v, int n){
  v->nmbr_place_max=n;
  if(!pthread_mutex_init(&v->verrou,NULL)) {return -1;}
  if(!pthread_cond_init(&v->condition,NULL)) {return -1;}
  if(v->nmbr_place_max!=n){return -1;}
  return 0;
}
int n_verrou_lock(struct n_verrou *v){

  pthread_mutex_lock(&(v->verrou));
  pthread_t moi=pthread_self();
  v->nmbr_place_max--;
  if(v->nmbr_place_max<0){//Si on a atteint le max de thread on wait
    printf("Je peux pas executer traitement2 j'attend\n");
    v->nmbr_place_max++;
    pthread_cond_wait(&(v->condition),&(v->verrou));

  }

    printf("Je suis le thread %d j'éxecute traitement2\n",(int)moi);
    printf("Il reste %d places\n",(v->nmbr_place_max));
    pthread_mutex_unlock(&(v->verrou));
    traitement2();
    pthread_mutex_lock(&(v->verrou));
    printf("Je suis le thread %d j'ai fini le traitement2\n",(int)moi);
    struct n_verrou *mon_D1 = (struct n_verrou*)v;
    n_verrou_unlock(mon_D1);
  return 0;
}


int n_verrou_unlock(struct n_verrou *v){
  pthread_t moi= pthread_self();
  if(v->nmbr_place_max<0){
    printf("Erreur il n'y a pas de thread a libérer\n");
    return -1;
  }
  else{
    v->nmbr_place_max++;
    printf("Je libére un thread il reste %d places\n",  v->nmbr_place_max);
    pthread_mutex_unlock(&(v->verrou));
    pthread_cond_broadcast(&(v->condition));
    return 0;
  }
}

int n_verrou_destroy(struct n_verrou *v){
  free(v);
  return 0;
}

void * LesTraitements(void * par){
  struct n_verrou *mon_D1 = (struct n_verrou*)par;
  int moi=pthread_self();
  printf("Je suis le thread %d j'execute traitement1\n",(int)moi);
  traitement1();
  n_verrou_lock(mon_D1);
  //n_verrou_unlock(mon_D1);
  printf("Je suis le thread %d j'execute traitement3\n",(int)moi);
  pthread_exit(NULL);
}
int main(){
  struct n_verrou mon_mut;
  n_verrou_init(&mon_mut,NMBR_PLACE_DISPONIBLE);
  pthread_t T[5];
  for(int i=0;i<5;i++){
    pthread_create(&T[i],NULL,LesTraitements,&mon_mut);
  }

  for(int i=0;i<5;i++){
    pthread_join(T[i],NULL);
  }

  return 0;
}
