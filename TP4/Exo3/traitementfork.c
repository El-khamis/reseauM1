#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define TAILLE_IMAGE 20
//Travail sur une image le fils 1 taff en premier ensuite le fils 2 ensuite le 3 et le 4
struct monImage{
  int zone[TAILLE_IMAGE];
}monimage;

void my_err(char *message) {
    perror(message);
    exit(1);
};
int main(){
  int sem_id=0;
  int shm_id=0;
  int pid=-1;
  int identifiant=0;
  struct monImage *addrMain;
  key_t maCle=ftok("./Fichier",11);
  sem_id=semget(maCle,1,IPC_CREAT|IPC_EXCL|0666);//Semaphore créer
  if(sem_id==-1){
    my_err("Err semget");
  }
  if(semctl(sem_id, 0, SETVAL, 1) == -1){
    my_err("Impossible d'initialiser le sémaphore 0\n");
  }
  shm_id= shmget(maCle,sizeof(monimage),IPC_CREAT|IPC_EXCL|0666);//Shared memory créer
  if(shm_id==-1){
    my_err("Err shared memory");
  }
  addrMain=shmat(shm_id,NULL,0);
  if((void *)addrMain==(void*)-1){
    my_err("Can't attach\n");
  }
  for(int i=0;i<TAILLE_IMAGE;i++){
    addrMain->zone[i]=0;
  }
  for(int i=0;i<4;i++){
    pid=fork();
    if(pid==0){
      identifiant=i;
      break;
    }
  }
  if(pid==0){
    //Chaque processus s'attache à la zone mémoire.
    struct monImage *addr;
    addr=shmat(shm_id,NULL,0);
    if((void *)addrMain==(void*)-1){
      my_err("Can't attach\n");
    }
    sem_id = semget(maCle, 1, IPC_CREAT | 0666); //Obtention d'un identifiant de sémaphore
    struct sembuf opp;
    struct sembuf opv;
    opp.sem_num=0;    opp.sem_op=-1;    opp.sem_flg=0;
    opv.sem_num=0;    opv.sem_op=1;    opv.sem_flg=0;

    for(int i=0;i<TAILLE_IMAGE;i++){
      switch(identifiant){
      case 0:
        if(semop(sem_id,&opp,1)==-1){
          my_err("impossible de faire opp");
        }
          addr->zone[i]=10;
          printf(" fils %d zone[%d]=%d\n",identifiant,i,addr->zone[i]);
          if(semop(sem_id,&opv,1)==-1){
            my_err("Impossible de faire opv");
          }
        break;
      case 1:
        if(semop(sem_id,&opp,1)==-1){
          my_err("impossible de faire opp");
        }
        if(addr->zone[i]==10){
          addr->zone[i]=20;
          printf(" fils %d zone[%d]=%d\n",identifiant,i,addr->zone[i]);
          if(semop(sem_id,&opv,1)==-1){
            my_err("Impossible de faire opv");
          }
        }
        else{
          i--;
          printf("Jpeux pas faire ça pour le moment %d",identifiant);
          if(semop(sem_id,&opv,1)==-1){
            my_err("Impossible de faire opv");
          }
        }
        break;
      case 2:
        if(semop(sem_id,&opp,1)==-1){
          my_err("impossible de faire opp");
        }
        if(addr->zone[i]==20){
          addr->zone[i]=30;
          printf(" fils %d zone[%d]=%d\n",identifiant,i,addr->zone[i]);
          if(semop(sem_id,&opv,1)==-1){
            my_err("Impossible de faire opv");
          }
        }
        else{
          i--;
          printf("Je suis le fils %d je peux pas faire ça mtn \n",identifiant);
          if(semop(sem_id,&opv,1)==-1){
            my_err("Impossible de faire opv");
          }
        }
        break;
      case 3:
        if(semop(sem_id,&opp,1)==-1){
          my_err("impossible de faire opp");
        }
        if(addr->zone[i]==30){
          addr->zone[i]=40;
          printf(" fils %d zone[%d]=%d\n",identifiant,i,addr->zone[i]);
          if(semop(sem_id,&opv,1)==-1){
            my_err("Impossible de faire opv");
          }
        }
        else{
          i--;
          printf("Je suis le fils %d je peux pas faire ça mtn\n",identifiant);
          if(semop(sem_id,&opv,1)==-1){
            my_err("Impossible de faire opv");
          }
        }
        break;
      }
    }


  }
  else{
    while(wait(NULL)>0){
    }
    printf("Je suis le père %d",getpid());
    for(int i=0;i<TAILLE_IMAGE;i++){
      printf("Zone[%d]=%d\n",i,addrMain->zone[i]);
    }
  }
}
