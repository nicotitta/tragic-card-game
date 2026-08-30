#include <stdio.h>
#include "gamelib.h"
#include <string.h>

unsigned short gioco_impostato = 0; //Variabile utilizzata per controllare se il gioco è stato impostato

int main() {
  int scelta = 0; //Variabile utilizzata per registrare l'opzione scelta dall'utente

  do {
    if ((scelta == 1 || (scelta == 2)) || (gioco_impostato == 1)) {
      scelta = 0;
      //In questo caso il giocatore è stato riportato al menù principale dopo aver scelto una delle prime due opzioni

      printf("                     BENTORNATO:\n");
      printf("\n");
      printf("Puoi reimpostare il gioco, combattere o uscire dal gioco\n");
      printf("Premere rispettivamente 1/2/3: \n");
    }
    else{
      printf("\n");
      printf("                     BENVENUTO IN:\n");
      printf("\n");
      printf("               \"TRAGIC: UN GIOCO DI CARTE\"\n");
      printf("\n");
      printf("                    Menù di gioco:\n");
      printf("\n");
      printf("Premere il numero corrispondente per l'azione desiderata:\n");
      printf("\n");
      printf("1] ---> Imposta gioco\n");
      printf("2] ---> Combatti\n");
      printf("3] ---> Termina gioco\n");
      printf("\n");
      printf("Inserisci qui la tua scelta: \n");
    }
    scanf("%d", &scelta);
    while(getchar() != '\n');
    switch (scelta) {
      case 1:
        imposta_gioco();
        gioco_impostato = 1; //Il gioco è stato impostato con successo e si aggiorna la variabile che ne tiene traccia
        break;
      case 2:
        if (gioco_impostato == 0) {
          printf("\nNon puoi combattere se il gioco non è stato prima impostato!\n");
          scelta = 0;
          break;
        }
        else {
          combatti();
          break;
        }
      case 3:
        termina_gioco();
        break;
      default:
        printf("\nNon hai inserito un valore corretto, per favore ritenta\n\n");
    }
  } while(scelta != 3); //Il menù non viene più mandato in stampa solo dopo che l'utente ha scelto l'opzione 3
}


