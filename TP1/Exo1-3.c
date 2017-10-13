#include <stdio.h>
#include <stdlib.h>

  int sommeRec(int* Tab, int taille){
    int som=Tab[taille];
    if(taille==0){
      return som;
    }
    else{
      return som+sommeRec(Tab,taille-1);
	}
  }

int main(){
  int taille=0;
  printf("\nEntrez la taille de votre Tableau\n");
  scanf("%d",&taille);
  int *Tableau=malloc(sizeof(int)*taille);
  for(int i=0; i<taille;i++){
    printf("Entre la %d-éme valeur de votre tableau ",i);
    scanf("%d",&Tableau[i]);
  }
  int som=sommeRec(Tableau, taille);
  printf("La somme de vos éléments est de %d \n",som);
  
  return 0;
}
