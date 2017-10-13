#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>


void *test1(){
  for(int i=0;i<1000;i++){
  printf("o");
  }

  pthread_exit(NULL);
}
void * test2(){
  for(int i=0;i<1000;i++){
    printf("k");
  }
  pthread_exit(NULL);
}



int main(){
  pthread_t idT1, idT2;

  if(pthread_create(&idT1,NULL,test1,NULL)!=0) printf("Can't create thread\n");
  if(pthread_create(&idT2,NULL,test2,NULL)!=0) printf("Can't create thread\n");
  int res=pthread_join(idT1,NULL);
      res=pthread_join(idT2,NULL);
    return 0;

}
