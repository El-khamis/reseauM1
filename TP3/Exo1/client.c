# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>


struct calcul{
    int x;
    int y;
    char operateur;
};

void sortie(char erreur[]){
    perror(erreur);
    exit(EXIT_FAILURE);
}

int main(){
  key_t ma_cle;
  int IdDuFichier=0,resultat=0;
  ma_cle=ftok("./MonFichier",1);
  IdDuFichier= msgget(ma_cle,0666|IPC_CREAT);
  if (IdDuFichier == -1)
        sortie("Erreur creation de file\n");
  struct calcul message;
  while(1){
    printf("Veuillez entrez votre calcul dans l'ordre suivant : x operateur y \n");
    scanf("%i",&((message.x)));
    scanf(" %c",&(message.operateur));
    scanf("%i",&(message.y));
    long size=sizeof(message);
    msgsnd(IdDuFichier, &message, sizeof(message), 0);
    msgrcv(IdDuFichier, &resultat, sizeof(resultat),0, 0);
    printf("Le resultat est %i\n",resultat);
    }
  return 0;
}
