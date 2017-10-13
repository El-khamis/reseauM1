#include <stdio.h>
#include <stdlib.h>

int * extract(int * T, int n, int a, int b, int *taille){
   int z=0;   
   int p=0;
   int *newT=NULL;
   for(int i=0; i<n;i++){
     if(T[i]>a && T[i]<b){
       z++;
     }
   }
   newT=malloc(sizeof(int)*z);	 //cree tableau      
   for(int i=0; i<n;i++){
     if(T[i]>a && T[i]<b){
       newT[p]=T[i];
       p++;
     }
   }
   *taille=p;

  return newT;
	       }
int main(){
  int taille;
  int T[14] = {1000, 2, 3, 7, 50, 25, 45, 89, 12, 0, 5, 24, 120, 10000};
  int *P=extract(T,14,12,256,&taille);
  printf("\n [");
  for(int i=0;i<taille;i++){
    printf("%d ",P[i]);
  }
  printf("]\n");
  
     return 0;
}
