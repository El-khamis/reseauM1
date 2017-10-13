#include <stdio.h>

int main(){

  int a=10;
  int b=25;
  int *p=&b;
  int *pp=&a;
  
  printf("%d\n%d\n%d\n%d",*(&(*(*(&p)))),*(p-1),*(*(&p)-1),*(*(&pp)+1));
  printf("\n Resultats des differentes variables\n");
  *(*(&p)-1);
  printf("%d\n%d\n%d\n%d\n",a,b,p ,pp);
  return 0;
}
