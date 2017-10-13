#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>



void *test1(void * par){
  pthread_t me= pthread_self();
  int * test=(int *)(par);
  printf("Je suis %d et la vaiable vaut %d\n",(int)me,*test);
  /*for(int i=0;i<1000;i++){
  printf("o");
}*/

  pthread_exit(NULL);
}
void * test2(void * par){
  pthread_t me= pthread_self();
  int * test=(int *)(par);
  (*test)=(*test);
  printf("Je suis %d et la vaiable vaut %d\n",(int)me,*test);

/*  for(int i=0;i<1000;i++){
    printf("k");
  } */
  pthread_exit(NULL);
}



int main(){
  int test= 14;
  pthread_t idT1, idT2;
  if(pthread_create(&idT1,NULL,test1,&test)!=0) printf("Can't create thread\n");
  if(pthread_create(&idT2,NULL,test2,&test)!=0) printf("Can't create thread\n");
  int res=pthread_join(idT1,NULL);
      res=pthread_join(idT2,NULL);
    return 0;

}
