#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL));
      FILE * fp;
  int taille=0;
  char str[12];


printf("Entrez la taille des vecters a généré \n");
  scanf("%d",&taille);
  sprintf(str,"%d\n", taille);
   fp = fopen ("file.txt", "w+");
   fputs(str,fp);
  for(int i=0;i<taille*2;i++){
    sprintf(str,"%d\n",rand()%20);
    fputs(str,fp);
}
return 0;
}
