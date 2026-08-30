#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gamelib.h"

time_t t;

//Dichiarazioni delle variabili globali:
static struct Mago giocatore1;
static struct Mago giocatore2;
static unsigned int numero_carteG1 = 0;
static unsigned int numero_carteG2 = 0;
static unsigned int n_carte_manoG1 = 5;
static unsigned int n_carte_manoG2 = 5;
static unsigned int n_carte_campoG1 = 0;
static unsigned int n_carte_campoG2 = 0;
extern unsigned short gioco_impostato;
static int attacco_effettuato = 0;

//Dichiarazioni delle funzioni static:
static void inizializzazione_campo(struct Mago *giocatore);
static void inizializzazione_mano(struct Mago *giocatore);
static void pesca(struct Mago *giocatore, unsigned int *n_carte_mano);
static void stampa_mazzo(struct Mago *mago);
static void gioca_carta(struct Mago *giocatore, struct Mago *giocatore0, unsigned int *n_carte_mano, unsigned int *n_carte_campoG, unsigned int *n_carte_campoG0);
static void attacca(struct Mago *giocatore, struct Mago *giocatore0, unsigned int *n_carte_campoG, unsigned int *n_carte_campoG0);
static void crea_mazzo(struct Mago *giocatore);
static void crea_mano(struct Mago *giocatore);
static void pulizia_memoria(struct Mago *giocatore);
static void assegnazione_punti(struct Mago *giocatore, unsigned int n_carte);
static void stampa_mano(unsigned int n_carte, struct Mago *giocatore);
static void stampa_campo(unsigned int n_carte_campoG, struct Mago *giocatore);

void imposta_gioco(void) {
  printf("\n");

  if (gioco_impostato == 1) {
    //La memoria dinamica viene deallocata se si imposta il gioco più volte
    pulizia_memoria(&giocatore1);
    pulizia_memoria(&giocatore2);
  }

  srand((unsigned) time(&t));

  giocatore1.inizioMazzo = NULL;
  giocatore1.fineMazzo = NULL;

  giocatore2.inizioMazzo = NULL;
  giocatore2.fineMazzo = NULL;

  //Inizializzazione delle mano dei giocatori:
  inizializzazione_mano(&giocatore1);
  inizializzazione_mano(&giocatore2);

  //Inizializzazione del campo da gioco:
  inizializzazione_campo(&giocatore1);
  inizializzazione_campo(&giocatore2);

  printf("\n");
  printf("Salve giocatori!\n");
  printf("\n");

  printf("Inserisci qui il nome del primo mago: ");
  fgets(giocatore1.nome_giocatore, 256, stdin);
  giocatore1.nome_giocatore[strlen(giocatore1.nome_giocatore) - 1] = 0;

  printf("\n");

  printf("Inserisci qui il nome del secondo mago: ");
  fgets(giocatore2.nome_giocatore, 256, stdin);
  giocatore2.nome_giocatore[strlen(giocatore2.nome_giocatore) - 1] = 0;

  printf("\n");

  printf("%s e %s benvenuti in \"Tragic: Un gioco di carte\"\n", giocatore1.nome_giocatore, giocatore2.nome_giocatore);

  printf("\n");

  printf("Inserite ora la classe del vostro mago\nPotete scegliere tra tenebre, vita e luce, premendo rispettivamente 1/2/3:\n\n%s: ", giocatore1.nome_giocatore);
  scanf("%u", &giocatore1.mago);
  while(getchar() != '\n');

  while(giocatore1.mago != 1 && giocatore1.mago != 2 && giocatore1.mago != 3) {
    printf("Non hai inserito un valore corretto per favore ritenta: \n");
    scanf("%u", &giocatore1.mago);
    while(getchar() != '\n');
  }

  printf("\n");
  printf("%s ora tocca a te: ", giocatore2.nome_giocatore);
  scanf("%u", &giocatore2.mago);
  while(getchar() != '\n');

  while(giocatore2.mago != 1 && giocatore2.mago != 2 && giocatore2.mago != 3) {
    printf("Non hai inserito un valore corretto per favore ritenta: \n");
    scanf("%u", &giocatore2.mago);
    while(getchar() != '\n');
  }

  printf("\n");

  giocatore1.punti_vita = 20;
  giocatore2.punti_vita = 20;

  printf("I punti vita di entrambi i giocatori sono inizializzati a 20\n\n");

  printf("%s inserisci il numero di carte del tuo mazzo (minimo: 6 carte / massimo: 60 carte): ", giocatore1.nome_giocatore);
  scanf("%u", &numero_carteG1);
  while (getchar() != '\n');

  while(numero_carteG1 <= 5 || numero_carteG1 > 60) {
    printf("Non hai inserito un valore corretto\nProva di nuovo: \n");
    scanf("%u", &numero_carteG1);
    while(getchar() != '\n');
  }

  printf("%s inserisci il numero di carte del tuo mazzo (minimo: 6 carte / massimo: 60 carte): ", giocatore2.nome_giocatore);
  scanf("%u", &numero_carteG2);
  while (getchar() != '\n');

  while(numero_carteG2 <= 5 || numero_carteG2 > 60) {
    printf("Non hai inserito un valore corretto\nProva di nuovo: \n");
    scanf("%u", &numero_carteG2);
    while(getchar() != '\n');
  }

  //Chiamata della funzione crea mazzo per entrambi i giocatori:
  for (int i = 0; i < numero_carteG1 + 5; i++) {
    crea_mazzo(&giocatore1);
  }

  for (int i = 0; i < numero_carteG2 + 5; i++) {
    crea_mazzo(&giocatore2);
  }

  assegnazione_punti(&giocatore1, numero_carteG1);
  assegnazione_punti(&giocatore2, numero_carteG2);

  crea_mano(&giocatore1);
  crea_mano(&giocatore2);

  printf("\n");

  printf("Mazzo di %s:\n", giocatore1.nome_giocatore);
  stampa_mazzo(&giocatore1);
  printf("\n");

  printf("Mazzo di %s:\n", giocatore2.nome_giocatore);
  stampa_mazzo(&giocatore2);
  printf("\n");

  printf("Mano iniziale di %s:\n", giocatore1.nome_giocatore);
  stampa_mano(n_carte_manoG1, &giocatore1);
  printf("\n");

  printf("Mano iniziale di %s:\n", giocatore2.nome_giocatore);
  stampa_mano(n_carte_manoG2, &giocatore2);
  printf("\n");

}

void combatti(void) {

  printf("\n");
  printf("                     REGOLE PER LA GIOCATA DEL TURNO:\n\n");
  printf("-> In ogni turno le azioni a disposizione di ogni mago sono 7:\n");
  printf("-> Pescare\n-> Giocare\n-> Attaccare\n-> Visualizzare la propria mano e il campo di entrambi i giocatori\n-> Passare il turno e visualizzare il punteggio di ogni mago\n\n");
  printf("-> Ogni mago può al massimo pescare, giocare e attaccare una sola volta ogni turno\n");
  printf("-> La visualizzazione del campo da gioco, della propria mano e dei punti vita sono invece sempre disponibili\n\n");

  //In modo casuale si stabilisce chi dei 2 giocatori inizia:
  srand((unsigned) time(&t));
  unsigned short int primo_turno = rand() % 2; //Variabile utilizzata per stabilire chi dei due giocatori inizia
  unsigned short int turno = 0; //Variabile utilizzata per tenere il conto dei turni

  //Count1, count2 e count3 sono variabili utilizzate per contare quante volte il giocatore effettua una determinata giocata
  //Tali variabili si riferisco solamente alle seguenti giocate: count1: pesca, count2: gioca carta, count3: attacca
  unsigned int count1 = 0;
  unsigned int count2 = 0;
  unsigned int count3 = 0;

  if (primo_turno == 0) {

    //Inizia il giocatore1:

    turno = 2;
    printf("\n");
    printf("Il primo mago ad iniziare è %s\n", giocatore1.nome_giocatore);
    printf("\n");

    unsigned short int opzione_utente = 0;

    do {
      printf("Azioni:\n1] --> Pesca\n2] --> Gioca una carta\n3] --> Attacca\n4] --> Visualizza la tua mano\n5] --> Visualizza il campo da gioco\n6] --> Passa il turno\n7] --> Visualizza punti vita:\n");
      scanf("%hd", &opzione_utente);
      while(getchar() != '\n');

      switch (opzione_utente) {
          case 1:
            if (count1 > 0) {
              printf("Non pui pescare più di una volta\n\n");
              break;
            }
            pesca(&giocatore1, &n_carte_manoG1);
            count1++;
            printf("\n");
            break;
          case 2:
            if (count2 > 0) {
              printf("Non pui giocare una carta più di una volta\n\n");
              break;
            }
            gioca_carta(&giocatore1, &giocatore2, &n_carte_manoG1, &n_carte_campoG1, &n_carte_campoG2);
            count2++;
            printf("\n");
            break;
          case 3:
            if (count3 > 0 && attacco_effettuato == 1) {
              printf("Non pui attaccare più di una volta\n\n");
              break;
            }
            attacca(&giocatore1, &giocatore2, &n_carte_campoG1, &n_carte_campoG2);
            printf("\n");
            count3++;
            break;
          case 4:
            printf("\nStampo la mano:\n");
            stampa_mano(n_carte_manoG1, &giocatore1);
            opzione_utente = 0;
            printf("\n");
            break;
          case 5:
            printf("\nCampo di %s\n", giocatore1.nome_giocatore);
            stampa_campo(n_carte_campoG1, &giocatore1);
            printf("\n");
            printf("Campo di %s\n", giocatore2.nome_giocatore);
            stampa_campo(n_carte_campoG2, &giocatore2);
            opzione_utente = 0;
            printf("\n");
            break;
          case 6:
            printf("\nTurno terminato\n\n");
            turno++;
            break;
          case 7:
            printf("\nI tuoi punti vita sono: %d\n", giocatore1.punti_vita);
            printf("I punti vita di %s sono: %d\n\n", giocatore2.nome_giocatore, giocatore2.punti_vita);
            opzione_utente = 0;
            break;
          default:
            printf("\nNon hai inserito un valore corretto, per favore ritenta\n\n");
        }
      } while(opzione_utente != 6);

      }
      else {

        //Altrimenti inizia il giocatore2:
        turno = 1;
        printf("\n");
        printf("Il primo mago ad iniziare è %s\n", giocatore2.nome_giocatore);
        printf("\n");

        unsigned short int opzione_utente = 0;

        do {

          printf("Azioni:\n1] --> Pesca\n2] --> Gioca una carta\n3] --> Attacca\n4] --> Visualizza la tua mano\n5] --> Visualizza il campo da gioco\n6] --> Passa il turno\n7] --> Visualizza punti vita:\n");
          scanf("%hd", &opzione_utente);
          while(getchar() != '\n');

          switch (opzione_utente) {
            case 1:
              if (count1 > 0) {
                printf("Non pui pescare più di una volta\n\n");
                break;
              }
              pesca(&giocatore2, &n_carte_manoG2);
              count1 ++;
              printf("\n");
              break;
            case 2:
              if (count2 > 0) {
                printf("Non pui giocare una carta più di una volta\n\n");
                break;
              }
              gioca_carta(&giocatore2, &giocatore1, &n_carte_manoG2, &n_carte_campoG2, &n_carte_campoG1);
              count2++;
              printf("\n");
              break;
            case 3:
              if (count3 > 0 && attacco_effettuato == 1) {
                printf("Non pui attaccare più di una volta\n\n");
                break;
              }
              attacca(&giocatore2, &giocatore1, &n_carte_campoG2, &n_carte_campoG1);
              printf("\n");
              count3++;
              break;
            case 4:
              printf("\nStampo la mano:\n");
              stampa_mano(n_carte_manoG2, &giocatore2);
              opzione_utente = 0;
              printf("\n");
              break;
            case 5:
              printf("\nCampo di %s\n", giocatore2.nome_giocatore);
              stampa_campo(n_carte_campoG2, &giocatore2);
              printf("\n");
              printf("Campo di %s\n", giocatore1.nome_giocatore);
              stampa_campo(n_carte_campoG1, &giocatore1);
              opzione_utente = 0;
              printf("\n");
              break;
            case 6:
              printf("\nTurno terminato\n\n");
              turno ++;
              break;
            case 7:
              printf("\nI tuoi punti vita sono: %d\n", giocatore2.punti_vita);
              printf("I punti vita di %s sono: %d\n\n", giocatore1.nome_giocatore, giocatore1.punti_vita);
              opzione_utente = 0;
              break;
            default:
              printf("\nNon hai inserito un valore corretto, per favore ritenta\n\n");
          }
        } while(opzione_utente != 6);
     }

     while ((giocatore1.punti_vita > 0 && giocatore2.punti_vita > 0) && (giocatore1.inizioMazzo != NULL && giocatore2.inizioMazzo != NULL)) {

       if (turno % 2 == 0) {

         //Se il turno è pari gioca il giocatore1

         unsigned int count1 = 0;
         unsigned int count2 = 0;
         unsigned int count3 = 0;

         unsigned short int opzione_utente = 0;
         attacco_effettuato = 0;

         printf("Il turno è di %s\n", giocatore1.nome_giocatore);
         printf("\n");

         do {

           if ((giocatore1.punti_vita <= 0 || giocatore2.punti_vita <= 0) || (giocatore1.inizioMazzo == NULL || giocatore2.inizioMazzo == NULL)) {
             break;
           }

           printf("Azioni:\n1] --> Pesca\n2] --> Gioca una carta\n3] --> Attacca\n4] --> Visualizza la tua mano\n5] --> Visualizza il campo da gioco\n6] --> Passa il turno\n7] --> Visualizza punti vita:\n");
           scanf("%hd", &opzione_utente);
           while(getchar() != '\n');

           switch (opzione_utente) {
               case 1:
                 if (count1 > 0) {
                   printf("Non pui pescare più di una volta\n\n");
                   break;
                 }
                 pesca(&giocatore1, &n_carte_manoG1);
                 count1++;
                 printf("\n");
                 break;
               case 2:
                 if (count2 > 0) {
                   printf("Non pui giocare una carta più di una volta\n\n");
                   break;
                 }
                 gioca_carta(&giocatore1, &giocatore2, &n_carte_manoG1, &n_carte_campoG1, &n_carte_campoG2);
                 printf("\n");
                 count2++;
                 break;
               case 3:
                 if (count3 > 0 && attacco_effettuato == 1) {
                   printf("%d", attacco_effettuato);
                   printf("Non pui attaccare più di una volta\n\n");
                   break;
                 }
                 attacca(&giocatore1, &giocatore2, &n_carte_campoG1, &n_carte_campoG2);
                 printf("\n");
                 count3++;
                 break;
               case 4:
                 printf("\nStampo la mano:\n");
                 stampa_mano(n_carte_manoG1, &giocatore1);
                 opzione_utente = 0;
                 printf("\n");
                 break;
               case 5:
                 printf("\nCampo di %s\n", giocatore1.nome_giocatore);
                 stampa_campo(n_carte_campoG1, &giocatore1);
                 printf("\n");
                 printf("Campo di %s\n", giocatore2.nome_giocatore);
                 stampa_campo(n_carte_campoG2, &giocatore2);
                 opzione_utente = 0;
                 printf("\n");
                 break;
               case 6:
                 printf("\nTurno terminato\n\n");
                 turno++;
                 break;
                case 7:
                   printf("\nI tuoi punti vita sono: %d\n", giocatore1.punti_vita);
                   printf("I punti vita di %s sono: %d\n\n", giocatore2.nome_giocatore, giocatore2.punti_vita);
                   opzione_utente = 0;
                   break;
               default:
                 printf("\nNon hai inserito un valore corretto, per favore ritenta\n\n");
             }
           } while(opzione_utente != 6);
       }

       else if (turno % 2 == 1) {

         //Se invece il turno è dispari gioca il giocatore2

         unsigned int count1 = 0;
         unsigned int count2 = 0;
         unsigned int count3 = 0;

         unsigned short int opzione_utente = 0;
         attacco_effettuato = 0;

         printf("Il turno è di %s\n", giocatore2.nome_giocatore);
         printf("\n");

         do {
           if ((giocatore1.punti_vita <= 0 || giocatore2.punti_vita <= 0) || (giocatore1.inizioMazzo == NULL || giocatore2.inizioMazzo == NULL)) {
             break;
           }
           printf("Azioni:\n1] --> Pesca\n2] --> Gioca una carta\n3] --> Attacca\n4] --> Visualizza la tua mano\n5] --> Visualizza il campo da gioco\n6] --> Passa il turno\n7] --> Visualizza punti vita:\n");
           scanf("%hd", &opzione_utente);
           while(getchar() != '\n');
           
           switch (opzione_utente) {
             case 1:
               if (count1 > 0) {
                 printf("Non pui pescare più di una volta\n\n");
                 break;
               }
               pesca(&giocatore2, &n_carte_manoG2);
               count1++;
               printf("\n");
               break;
             case 2:
               if (count2 > 0) {
                 printf("Non pui giocare una carta più di una volta\n\n");
                 break;
               }
               gioca_carta(&giocatore2, &giocatore1, &n_carte_manoG2, &n_carte_campoG2, &n_carte_campoG1);
               printf("\n");
               count2++;
               break;
             case 3:
               if (count3 > 0 && attacco_effettuato == 1) {
                 printf("Non pui attaccare più di una volta\n\n");
                 break;
               }
               attacca(&giocatore2, &giocatore1, &n_carte_campoG2, &n_carte_campoG1);
               printf("\n");
               count3++;
               break;
             case 4:
               printf("\nStampo la mano:\n");
               stampa_mano(n_carte_manoG2, &giocatore2);
               opzione_utente = 0;
               printf("\n");
               break;
             case 5:
               printf("\nCampo di %s\n", giocatore2.nome_giocatore);
               stampa_campo(n_carte_campoG2, &giocatore2);
               printf("\n");
               printf("Campo di %s\n", giocatore1.nome_giocatore);
               stampa_campo(n_carte_campoG1, &giocatore1);
               opzione_utente = 0;
               printf("\n");
               break;
             case 6:
               printf("\nTurno terminato\n\n");
               turno ++;
               break;
              case 7:
                printf("\nI tuoi punti vita sono: %d\n", giocatore2.punti_vita);
                printf("I punti vita di %s sono: %d\n\n", giocatore1.nome_giocatore, giocatore1.punti_vita);
                opzione_utente = 0;
                break;
             default:
               printf("\nNon hai inserito un valore corretto, per favore ritenta\n\n");
           }
         } while(opzione_utente != 6);
       }
     }

     printf("Il duello è terminato!\n");

    //Vengono eseguiti tutti i controlli per determinare il vincitore del combattimento:

    //Nel caso in cui uno dei due maghi finisca i PV:
    if (giocatore1.punti_vita <= 0 || giocatore2.punti_vita <= 0) {

      if (giocatore1.punti_vita > giocatore2.punti_vita) {
        printf("%s ha esaurito i suoi punti vita ed ha perso il combattimento\n", giocatore2.nome_giocatore);
        printf("%s hai vinto COMPLIMENTI!!!\n\n", giocatore1.nome_giocatore);
      }
      else if (giocatore2.punti_vita > giocatore1.punti_vita) {
        printf("%s ha esaurito i suoi punti vita ed ha perso il combattimento\n", giocatore1.nome_giocatore);
        printf("%s hai vinto COMPLIMENTI!!!\n\n", giocatore2.nome_giocatore);
      }
    }

    //Nel caso in cui uno dei due maghi termini le carte nel mazzo si confrontano i PV dei giocatori:

    else if (giocatore1.inizioMazzo == NULL || giocatore2.inizioMazzo == NULL) {

      if (giocatore1.inizioMazzo == NULL) {
          printf("%s ha esaurito le sue carte nel mazzo\nVince chi ha più punti vita:\n\n", giocatore1.nome_giocatore);
      }
      else {
        printf("%s ha esaurito le sue carte nel mazzo\nVince chi ha più punti vita:\n\n", giocatore2.nome_giocatore);
      }

      //Si conntrollano i PV dei due maghi:

      if (giocatore1.punti_vita > giocatore2.punti_vita) {
        printf("Punti vita di %s: %d\n", giocatore1.nome_giocatore, giocatore1.punti_vita);
        printf("Punti vita di %s: %d\n\n", giocatore2.nome_giocatore, giocatore2.punti_vita);
        printf("%s hai più punti vita ed hai vinto COMPLIMENTI!!!\n\n", giocatore1.nome_giocatore);
      }
      else if (giocatore1.punti_vita < giocatore2.punti_vita) {
        printf("Punti vita di %s: %d\n", giocatore2.nome_giocatore, giocatore1.punti_vita);
        printf("Punti vita di %s: %d\n\n", giocatore1.nome_giocatore, giocatore1.punti_vita);
        printf("%s hai più punti vita ed hai vinto COMPLIMENTI!!!\n\n", giocatore2.nome_giocatore);
      }
      else {
        printf("%s e %s avete gli stessi punti vita\nNon c'è nessun vincitore\n\n", giocatore1.nome_giocatore, giocatore2.nome_giocatore);
      }
    }
}

void termina_gioco(void) {
  printf("\n");
  printf("Grazie per aver giocato a \"Tragic: Un gioco di carte\"\nA presto!\n\n");
  return;
}

void inizializzazione_campo(struct Mago *giocatore) {
  for (int i = 0; i < 4; i++) {
    giocatore->campo[i] = NULL;
  }
}

void inizializzazione_mano(struct Mago *giocatore) {
  for (int i = 0; i < 6; i++) {
    giocatore->mano[i] = NULL;
  }
}

void pulizia_memoria(struct Mago *giocatore) {

  free(giocatore->inizioMazzo);
  giocatore->inizioMazzo = NULL;
  giocatore->fineMazzo = NULL;

}

void stampa_mazzo(struct Mago *mago) {
  printf("\n");
  if (mago->inizioMazzo == NULL) {
    printf("Non ci sono carte nel mazzo\n");
  }
  else {
    struct Carta *carta_temp = mago->inizioMazzo;
    do {
      if (carta_temp->tipo_carta == 0) {
        printf("Punti vita: %d e Tipo carta: creatura\n", carta_temp->punti_vita);
      }
      else if (carta_temp->tipo_carta == 1) {
        printf("Tipo carta: rimuovi creatura\n");
      }
      else if (carta_temp->tipo_carta == 2) {
        printf("Punti vita: %d e Tipo carta: infliggi danno\n", carta_temp->punti_vita);
      }
      else if(carta_temp->tipo_carta == 3) {
        printf("Punti vita: %d e Tipo carta: guarisci danno\n", carta_temp->punti_vita);
      }

      carta_temp = carta_temp->carta_successiva;
    } while(carta_temp != NULL);
  }
}

void crea_mazzo(struct Mago *giocatore) {

    struct Carta *nuova_carta = (struct Carta*)(malloc(sizeof(struct Carta))); //Creazione della nuova carta

    //Assegnazione della tipologia della carta:

    unsigned int probabilita = rand() % 101;

    if (probabilita <= 40) {
      nuova_carta->tipo_carta = 0;
    }
    else if (probabilita > 40 && probabilita <= 65) {
      nuova_carta->tipo_carta = 1;
    }
    else if (probabilita > 65 && probabilita <= 85) {
      nuova_carta->tipo_carta = 2;
    }
    else if (probabilita > 85 && probabilita <= 100) {
      nuova_carta->tipo_carta = 3;
    }

    nuova_carta->punti_vita = 0;

    nuova_carta->carta_successiva = NULL;

    if (giocatore->inizioMazzo == NULL) { //Se nel mazzo non ci sono carte la nuova carta diventa sia la prima che l'ultima
      giocatore->inizioMazzo = nuova_carta;
      giocatore->fineMazzo = nuova_carta;
    }
    else {
      giocatore->fineMazzo->carta_successiva = nuova_carta; //l'ultima carta punta alla nuova carta che è diventata l'ultima
      giocatore->fineMazzo = nuova_carta; //all'ultima carta del mazzo viene assegnata la nuova carta inserita
  }
}

void assegnazione_punti(struct Mago *giocatore, unsigned int n_carte) {

  //Le ultime 3 carte nel mazzo, ossia quelle che verranno pescate per ultime, hanno punti vita tra 7 e 8, senza considerare le classi del mago che possono aumentare i punti vita della carta
  struct Carta *carta_attuale = giocatore->inizioMazzo;
  for (int i = 0; i < 3; i++) {
    carta_attuale->punti_vita = (rand() % (8 - 7 + 1)) + 7;
    if (giocatore->mago == 2) { //Se il mago ha classe vita:
      if (carta_attuale->tipo_carta == 0) //Le carte di tipo creatura:
        carta_attuale->punti_vita = carta_attuale->punti_vita + (carta_attuale->punti_vita / 2); //hanno la metà dei punti vita in più
    }
    else if (giocatore->mago == 1) { //Se il mago ha classe tenebre:
      if (carta_attuale->tipo_carta == 2) { //Le sue carte infliggi danno:
        carta_attuale->punti_vita = carta_attuale->punti_vita * 2; //hanno il doppio dei punti vita
      }
    }
    else if (giocatore->mago == 3) { //Se il mago ha classe luce:
      if (carta_attuale->tipo_carta == 3) { //Le sue carte guarisci danno hanno 3 volte e mezzo il valore dei punti vita
        carta_attuale->punti_vita = (carta_attuale->punti_vita * 3) / 2;
      }
    }
    if (carta_attuale->tipo_carta == 1) { //Se la carta è di tipo rimuovi creatura questa non ha punti vita
      carta_attuale->punti_vita = 0;
    }
    carta_attuale = carta_attuale->carta_successiva;
  }

  //Le carte nel mezzo del mazzo hanno punti vita che variano da 4 a 6 incluso, senza considerare le classi del mago che possono aumentare i punti vita della carta
  struct Carta *attuale_agg = carta_attuale;
  for (int i = 0; i < ((n_carte + 5) - 7); i++) {
    attuale_agg->punti_vita = (rand() % (6 - 4 + 1)) + 4;
    if (giocatore->mago == 2) { //Se il mago ha classe vita:
      if (attuale_agg->tipo_carta == 0) //Le carte di tipo creatura:
        attuale_agg->punti_vita = attuale_agg->punti_vita + (attuale_agg->punti_vita / 2); //hanno la metà dei punti vita in più
    }
    else if (giocatore->mago == 1) { //Se il mago ha classe tenebre:
      if (attuale_agg->tipo_carta == 2) { //Le sue carte infliggi danno:
        attuale_agg->punti_vita = attuale_agg->punti_vita * 2; //hanno il doppio dei punti vita
      }
    }
    else if (giocatore->mago == 3) { //Se il mago ha classe luce:
      if (attuale_agg->tipo_carta == 3) { //Le sue carte guarisci danno hanno 3 volte e mezzo il valore dei punti vita
        attuale_agg->punti_vita = (attuale_agg->punti_vita * 3) / 2;
      }
    }
    if (attuale_agg->tipo_carta == 1) { //Se la carta è di tipo rimuovi creatura questa non ha punti vita
      attuale_agg->punti_vita = 0;
    }
    attuale_agg = attuale_agg->carta_successiva;
  }

  //Le prime carte che si pescano hanno invece punti vita bassi, tra 1 e 3 incluso
  struct Carta *carta_finale = attuale_agg;
  while (carta_finale != NULL) {
    carta_finale->punti_vita = (rand() % (3 - 1 + 1)) + 1;

    if (giocatore->mago == 2) { //Se il mago ha classe vita:
      if (carta_finale->tipo_carta == 0) //Le carte di tipo creatura:
        carta_finale->punti_vita = carta_finale->punti_vita + (carta_finale->punti_vita / 2); //hanno la metà dei punti vita in più
    }
    else if (giocatore->mago == 1) { //Se il mago ha classe tenebre:
      if (carta_finale->tipo_carta == 2) { //Le sue carte infliggi danno:
        carta_finale->punti_vita = carta_finale->punti_vita * 2; //hanno il doppio dei punti vita
      }
    }
    else if (giocatore->mago == 3) { //Se il mago ha classe luce:
      if (carta_finale->tipo_carta == 3) { //Le sue carte guarisci danno hanno 3 volte e mezzo il valore dei punti vita
        carta_finale->punti_vita = (carta_finale->punti_vita * 3) / 2;
      }
    }
    if (carta_finale->tipo_carta == 1) { //Se la carta è di tipo rimuovi creatura questa non ha punti vita
      carta_finale->punti_vita = 0;
    }
    carta_finale = carta_finale->carta_successiva;
  }
}

void crea_mano(struct Mago *giocatore) {
  int i = 0;
  for (i = 0; i < 5; i++) {
    if (giocatore->inizioMazzo == NULL) {
      printf("Non ci sono carte nel mazzo!\n");
    }
    else {
      struct Carta *carta_prev = NULL;
      struct Carta *carta_scan = giocatore->inizioMazzo;
      if (carta_scan->carta_successiva == NULL) {
          giocatore->mano[i] = carta_scan;
          giocatore->inizioMazzo = NULL;
          break;
      }
      else {
        do {
          if (carta_scan->carta_successiva == giocatore->fineMazzo) {
            carta_prev = carta_scan;
            break;
          }
          else {
            carta_scan = carta_scan->carta_successiva;
          }
        } while(carta_scan->carta_successiva != NULL);
        giocatore->mano[i] = carta_prev->carta_successiva;
        carta_prev->carta_successiva = NULL;
        giocatore->fineMazzo = carta_prev;
      }
    }
  }
}

void stampa_mano(unsigned int n_carte, struct Mago *giocatore) {
  printf("\n");

  struct Carta *carta = giocatore->mano[0];

  if (carta == NULL) {
    printf("Non ci sono carte nella mano\n");
  }
  else {
    for (int i = 0; i < n_carte; i++) {
      carta = giocatore->mano[i];
      if (carta->tipo_carta == 0) {
        printf("Carta numero %d --> Punti vita: %d e Tipo carta: creatura\n", i+ 1, carta->punti_vita);
      }
      else if (carta->tipo_carta == 1) {
        printf("Carta numero %d --> Tipo carta: rimuovi creatura\n", i+ 1);
      }
      else if (carta->tipo_carta == 2) {
        printf("Carta numero %d --> Punti vita: %d e Tipo carta: infliggi danno\n", i+ 1, carta->punti_vita);
      }
      else if(carta->tipo_carta == 3) {
        printf("Carta numero %d --> Punti vita: %d e Tipo carta: guarisci danno\n", i+ 1, carta->punti_vita);
      }
    }
  }
}

void stampa_campo(unsigned int n_carte_campoG,struct Mago *giocatore) {
  struct Carta *carta = giocatore->campo[0];

  if (carta == NULL) {
    printf("Non ci creature nel campo\n");
  }
  else {
    for (int i = 0; i < n_carte_campoG; i++) {
      carta = giocatore->campo[i];
      printf("Carta numero %d --> Punti vita: %d\n", i + 1, carta->punti_vita);
    }
  }
}

void pesca(struct Mago *giocatore, unsigned int *n_carte_mano) {

  if (giocatore->inizioMazzo == NULL) {
    printf("Non ci sono carte nel mazzo!\n");
    printf("Vince chi ha più punti vita\n");
  }
  else {

    struct Carta *carta_prev = NULL;
    struct Carta *carta_scan = giocatore->inizioMazzo;

    if (carta_scan->carta_successiva == NULL) { //Nel caso in cui ci sia una sola carta nel mazzo:
      int i = 0;
      for (i = 0; i < 6; i++) { //Scorro la mano
        if (giocatore->mano[i] == NULL) { //Appena si trova uno slot libero della mano lo si riempe con l'unica carta
          giocatore->mano[i] = carta_scan;
          *n_carte_mano = *n_carte_mano + 1;
          break;
        }
      }
      if (i == 6) {
        printf("Non c'è spazio nella mano. Elimino la carta\n");
        free(carta_scan);
        giocatore->inizioMazzo = NULL;
      }
      giocatore->inizioMazzo = NULL;

    }

    else { //Se invece ci sono almeno 2 carte nel mazzo:

      do {
        if (carta_scan->carta_successiva == giocatore->fineMazzo) { //Voglio raggiungere l'ultima carta nel mazzo
          carta_prev = carta_scan; //Ho raggiunto l'ultima carta nel mazzo
          break;
        }
        else {
          carta_scan = carta_scan->carta_successiva; //Itero fino a quando non raggiungo l'ultima carta nel mazzo
        }
      } while(carta_scan->carta_successiva != NULL);

      int i = 0;
      for (i = 0; i < 6; i++) { //Scorro la mano
        if (giocatore->mano[i] == NULL) {
          giocatore->mano[i] = carta_prev->carta_successiva;
          *n_carte_mano = *n_carte_mano + 1;
          break;
        }
      }
      if (i == 6) {
        printf("Non c'è spazio nella mano. Elimino la carta\n");
        free(carta_prev->carta_successiva);
        carta_prev->carta_successiva = NULL;
      }

      carta_prev->carta_successiva = NULL;
      giocatore->fineMazzo = carta_prev;
    }
  }
}

void gioca_carta(struct Mago *giocatore, struct Mago *giocatore0, unsigned int *n_carte_mano, unsigned int *n_carte_campoG, unsigned int *n_carte_campoG0) {

  //Il puntatore giocatore è il mago che attacca mentre giocatore0 è il mago che subisce gli attacchi
  //Il puntatore n_carte_campoG si riferisce al numero delle creature nel campo del mago che attacca
  //Il puntatore n_carte_campoG0 si riferisce al numero delle creature nel campo del mago che subisce l'attacco
  //Il punatatore n_carte_mano rappresenta il numero delle carte nella mano del mago che attacca

  struct Carta *carta_giocata = NULL;
  static int danno_creatura = 0;

  //Si effettua un controllo per verificare se nella mano del mago ci siano carte:
  int i = 0;
  for (i = 0; i < *n_carte_mano; i++) {
    if (giocatore->mano[i] != NULL) {
      break;
    }
  }

  if (i == *n_carte_mano) {
    printf("Non ci sono più carte nella mano\n");
  }
  else {
    printf("Scegli una delle carte presenti nella tua mano. Per farlo digita il numero corrispondente alla carta:\n");
    stampa_mano(*n_carte_mano, giocatore);
    printf("\n");
    int carta_da_giocare = 0;
    printf("Inserisci qui il numero della carta che vuoi giocare: \n"); //Il giocatore sceglie la carta da giocare identificata con un numero che va da 1 al numero delle carte nella mano
    scanf("%d", &carta_da_giocare);
    while(getchar() != '\n');

    //Controllo per verificare che l'utente abbia scelto una delle carte nella sua mano:
    while(carta_da_giocare < 1 || carta_da_giocare > *n_carte_mano) {
      printf("\nNon hai inserito un valore corretto\nScegli una delle carte presenti nella tua mano: ");
      scanf("%d", &carta_da_giocare);
      while(getchar() != '\n');
    }

    for (int i = 1; i <= *n_carte_mano; i++) {
      if (carta_da_giocare == i) {
        carta_giocata = giocatore->mano[i - 1]; //Con questa linea viene individuata la carta da giocare
        break;
      }
    }

    if (carta_giocata->tipo_carta == 0) { //La carta scelta è di tipo CREATURA e la si inserisce nel campo se c'è spazio libero

      int i = 0;
      for (i = 0; i < 4; i++) { //Si scorre il campo per trovare uno slot libero
        if (giocatore->campo[i] == NULL) {
          giocatore->campo[i] = carta_giocata;
          *n_carte_campoG = *n_carte_campoG + 1; //Si aumentano le carte nel campo del giocatore che attacca
          break;
        }
      }
      if (i == 4) {
        printf("Non c'è spazio libero nel campo, la carta viene reinserita nella mano\n");
        printf("\n");
      }
      else if (i != 4) {

        //Si effettua l'aggiornamento della mano del mago che ha giocato la creatura:
        for (int i = carta_da_giocare - 1; i < *n_carte_mano; i++) {
            giocatore->mano[i] = giocatore->mano[i + 1];
        }
        for (int i = 0; i < 6; i++) {
          if (i == 5) {
            giocatore->mano[i] = NULL;
          }
        }
        *n_carte_mano = *n_carte_mano - 1;
      }
      printf("\n");
      printf("Creatura inserita nel campo\n");

    }

    else if (carta_giocata->tipo_carta == 1) { //La carta scelta è di tipo RIMUOVI CREATURA:

      //Il ciclo viene utilizzato per controllare se ci sono o meno creature da rimuovere nel campo avversario
      int i = 0;
      for (i = 0; i < 4; i++) {
        if (giocatore0->campo[i] == NULL) {
        }
        else {
          break; //Se c'è almeno una creatura nel campo si esce dal ciclo e si prosegue con la sua eliminazione
        }
      }
      if (i == 4) {
        printf("Non puoi giocare una carta di tipo rimuovi creatura perchè non ci sono creature nel campo avversario\n");
      }
      else {

        //Il giocatore sceglie un bersaglio identificato da un numero che identifica la creatura da rimuovere
        unsigned int numero_bersaglio = 0;
        struct Carta *creatura_da_rimuovere = giocatore0->campo[0];
        printf("\n");

        printf("Scegli un bersaglio su cui giocare la tua carta (1/2/3/4): \n");
        printf("Campo dell'avversario:\n");
        stampa_campo(*n_carte_campoG0, giocatore0);
        printf("\n");
        printf("Inserisci qui il numero della creatura da rimuovere: \n");
        scanf("%u", &numero_bersaglio);
        while(getchar() != '\n');

        //Controllo per verificare che l'utente abbia scelto una creatura presente nel campo nemico:
        while(numero_bersaglio < 1 || numero_bersaglio > *n_carte_campoG0) {
          printf("Non hai inserito un valore corretto\nScegli una delle carte presenti nel campo avversario: \n");
          scanf("%u", &numero_bersaglio);
          while(getchar() != '\n');
        }

        //Il ciclo serve per individuare la creatura da rimuovere
        for (int i = 1; i <= *n_carte_campoG0; i++) {
          if (numero_bersaglio == i) {
            creatura_da_rimuovere = giocatore0->campo[i - 1];
            break;
          }
        }

        //Eliminazione della creatura e aggiornamento del campo:
        for (int i = numero_bersaglio - 1; i < *n_carte_campoG0; i++) {
          giocatore0->campo[i] = giocatore0->campo[i + 1];
        }

        free(creatura_da_rimuovere);
        creatura_da_rimuovere = NULL;

        for (int i = 0; i < 4; i++) {
          if (i == 3) {
            giocatore0->campo[i] = NULL;
            break;
          }
        }
        *n_carte_campoG0 = *n_carte_campoG0 - 1;
        printf("La creatura è stata rimossa dal campo nemico con successo\n");

        //Anche la carta rimuovi creatura viene atuomaticamente eliminata una volta giocata e si aggiorna la mano:
        for (int i = carta_da_giocare - 1; i < *n_carte_mano; i++) {
          giocatore->mano[i] = giocatore->mano[i + 1];
        }

        free(carta_giocata);
        carta_giocata = NULL;

        for (int i = 0; i < 6; i++) {
          if (i == 5) {
            giocatore->mano[i] = NULL;
            break;
          }
        }
        *n_carte_mano = *n_carte_mano - 1;

      }
    }

    else if (carta_giocata->tipo_carta == 2) { //La carta scelta è di tipo INFLIGGI DANNO:

      unsigned int bersaglio = 2; //Variabile che utilizzata per controllare se il mago attacca il mago nemico o una delle sue creature
      printf("\n");
      printf("Stai per giocare una carta di tipo infliggi danno!\nPer infliggere danno al mago nemico premere 0\nPer infliggere danno ad una creatura nemica premere1. Scelta: ");
      scanf("%u", &bersaglio);
      while(getchar() != '\n');

      //Controllo per verificare che l'utente abbia inserito un valore corretto, tra 0 e 1:

      while(bersaglio != 0 && bersaglio != 1) {
        printf("Non hai inserito un valore corretto\nScegli tra 0 e 1: \n");
        scanf("%u", &bersaglio);
        while(getchar() != '\n');
      }

      //Il giocatore ha deciso di giocare la carta contro il mago nemico:
      if (bersaglio == 0) {
        giocatore0->punti_vita = giocatore0->punti_vita - carta_giocata->punti_vita;
        printf("\n");
        printf("La carta ha esaurito il suo effetto con successo!\n");

        //La carta infliggi danno viene elimanata dalla mano:
        for (int i = carta_da_giocare - 1; i < *n_carte_mano; i++) {
          giocatore->mano[i] = giocatore->mano[i + 1];
        }

        free(carta_giocata);
        carta_giocata = NULL;

        for (int i = 0; i < 6; i++) {
          if (i == 5) {
            giocatore->mano[i] = NULL;
            break;
          }
        }

        *n_carte_mano = *n_carte_mano - 1;

      }

      else { //Se il giocatore decide di togliere punti ad una creatura nel campo dell'avversario:

        //Controllo per verificare che ci siano creature nel campo del mago avversario:
        int i = 0;
        for (i = 0; i < 4; i++) {
          if (giocatore0->campo[i] == NULL) {
          }
          else {
            break;
          }
        }

        if (i == 4) {
          printf("\n");
          printf("Non ci sono creature nel campo del mago avversario\n");
          printf("La carta viene reinserita nella mano\n");
        }

        else { //Se invece c'è almeno una creatura nel campo:

          int creatura_bersaglio = 0;

          //La carta si rifersice alla creatura alla quale deve essere inflitto il danno
          struct Carta *carta = giocatore0->campo[0];
          printf("\n");

          printf("Scegli una creatura a cui vuoi infliggere danno premendo il numero a cui corrisponde(1/2/3/4):\nCampo di %s:\n", giocatore0->nome_giocatore);
          stampa_campo(*n_carte_campoG0, giocatore0);
          printf("\n");
          printf("Inserisci qui il numero della creatura: \n");
          scanf("%d", &creatura_bersaglio);
          while(getchar() != '\n');

          //Controllo per verificare che l'utente abbia scelto una delle creature presenti nel campo avversario:
          while(creatura_bersaglio < 1 || creatura_bersaglio > *n_carte_campoG0) {
            printf("Non hai inserito un valore corretto\nScegli una delle creature nel campo nemico: \n");
            scanf("%d", &creatura_bersaglio);
            while(getchar() != '\n');
          }

          carta = giocatore0->campo[creatura_bersaglio - 1];
          carta->punti_vita = carta->punti_vita - carta_giocata->punti_vita;

          if (carta->punti_vita <= 0) {

            printf("\n");
            printf("La carta è stata eliminata dal campo definitivamente\n");

            //Aggiornamento campo avversario ed elimanzione della carta dal campo
            for (int i = creatura_bersaglio - 1; i < *n_carte_campoG0; i++) {
              giocatore0->campo[i] = giocatore0->campo[i + 1];
            }

            free(carta);
            carta = NULL;

            for (int i = 0; i < 4; i++) {
              if (i == 3) {
                giocatore0->campo[i] = NULL;
                break;
              }
            }

            *n_carte_campoG0 = *n_carte_campoG0 - 1;

            //Aggiornamento mano del mago che attacca:
            for (int i = carta_da_giocare - 1; i < *n_carte_mano; i++) {
              giocatore->mano[i] = giocatore->mano[i + 1];
            }

            free(carta_giocata);
            carta_giocata = NULL;

            for (int i = 0; i < 6; i++) {
              if (i == 5) {
                giocatore->mano[i] = NULL;
                break;
              }
            }

            *n_carte_mano = *n_carte_mano - 1;
          }

          else {

            //Aggiornamento mano del mago che attacca:
            for (int i = carta_da_giocare - 1; i < *n_carte_mano; i++) {
              giocatore->mano[i] = giocatore->mano[i + 1];
            }

            free(carta_giocata);
            carta_giocata = NULL;

            for (int i = 0; i < 6; i++) {
              if (i == 5) {
                giocatore->mano[i] = NULL;
                break;
              }
            }
            *n_carte_mano = *n_carte_mano - 1;
          }
        }
      }
    }

    else if (carta_giocata->tipo_carta == 3) {  //La carta scelta è di tipo GUARISCI DANNO:

      unsigned int bersaglio = 2;
      printf("\nStai per giocare una carta di tipo guarisci danno!\nPuoi giocare questa carta a favore del tuo mago o di una tua creatura nel campo\n");
      printf("Premere rispettivamente 0 o 1: ");
      scanf("%u", &bersaglio);
      while(getchar() != '\n');

      //Controllo per verificare che l'utente abbia inserito un valore tra 0 e 1:
      while(bersaglio != 0 && bersaglio != 1) {
        printf("Non hai inserito un valore corretto\nScegli tra 0 e 1: \n");
        scanf("%u", &bersaglio);
        while(getchar() != '\n');
      }

      //Se il giocatore ha deciso di giocare la carta a favore del proprio mago si aggiungono i punti vita della carta solo se i punti finali non superano i 20 iniziali
      if (bersaglio == 0) {
        if (giocatore->punti_vita == 20) {
          printf("Non puoi andare oltre i 20 PV iniziali, la carta non ha effetto e viene reinserita nella mano\n");
        }
        else {
          //Al giocatore si aggiunge solo la differenza in caso di eccesso di punti vita
          if ((giocatore->punti_vita + carta_giocata->punti_vita) > 20) {
            printf("Non puoi aumentare i tuoi punti vita oltre il limite di 20!\n");
            giocatore->punti_vita = giocatore->punti_vita + carta_giocata->punti_vita - ((giocatore->punti_vita + carta_giocata->punti_vita) - 20);
            printf("\n");
            printf("La carta ha avuto il suo effetto!\n");

            //Aggiornamento della mano del mago che attacca:
            for (int i = carta_da_giocare - 1; i < *n_carte_mano; i++) {
              giocatore->mano[i] = giocatore->mano[i + 1];
            }

            free(carta_giocata);
            carta_giocata = NULL;

            for (int i = 0; i < 6; i++) {
              if (i == 5) {
                giocatore->mano[i] = NULL;
                break;
              }
            }

            *n_carte_mano = *n_carte_mano - 1;
          }

          else { //Se non vengono superati i 20 PV la carta esaurisce il suo effetto nel totale
            giocatore->punti_vita = giocatore->punti_vita + carta_giocata->punti_vita;
            printf("\n");
            printf("La carta ha avuto il suo effetto!\n");

            //Aggiornamento mano del mago che attacca:
            for (int i = carta_da_giocare - 1; i < *n_carte_mano; i++) {
              giocatore->mano[i] = giocatore->mano[i + 1];
            }

            free(carta_giocata);
            carta_giocata = NULL;

            for (int i = 0; i < 6; i++) {
              if (i == 5) {
                giocatore->mano[i] = NULL;
                break;
              }
            }

            *n_carte_mano = *n_carte_mano - 1;
          }
        }
    }

    //Se il giocatore ha scelto di guarire una delle proprie creature:
    else {

      //Controllo se nel campo del giocatore ci sono o meno creature
      int i = 0;
      for (i = 0; i < 4; i++) {
        if (giocatore->campo[i] == NULL) {
        }
        else {
          break;
        }
      }
      if (i == 4) {
        printf("Non ci sono creature nel tuo campo\n");
        printf("La carta viene reinserita nella mano\n");
      }

      else { //Se invece c'è almeno una creatura nel campo:

          int creatura_bersaglio = 0;
          struct Carta *carta = giocatore->campo[0];
          printf("\n");

          printf("\nScegli una creatura che intendi guarire premendo il numero a cui corrisponde(1/2/3/4):\nIl tuo campo:\n\n");
          stampa_campo(*n_carte_campoG, giocatore);
          scanf("%d", &creatura_bersaglio);
          while(getchar() != '\n');

          //Controllo per verificare che l'utente abbia scelto una creatura presente nel suo campo:
          while(creatura_bersaglio < 1 || creatura_bersaglio > *n_carte_campoG) {
            printf("\nNon hai inserito un valore corretto\nScegli una delle creature presenti nel tuo campo: \n");
            scanf("%d", &creatura_bersaglio);
            while(getchar() != '\n');
          }

          carta = giocatore->campo[creatura_bersaglio - 1];

          if (carta_giocata->punti_vita + carta->punti_vita > 12) {
            printf("\nLa creatura non può superare 12 punti vita\n");
            carta->punti_vita = carta->punti_vita + carta_giocata->punti_vita - ((carta->punti_vita + carta_giocata->punti_vita) - 12);
            printf("\nLa carta ha avuto il suo effetto\n");
          }
          else {
            carta->punti_vita = carta->punti_vita + carta_giocata->punti_vita;
            printf("\nLa carta ha avuto il suo effetto\n");
          }

          //Aggiornamento mano del mago che attacca:
          for (int i = carta_da_giocare - 1; i < *n_carte_mano; i++) {
            giocatore->mano[i] = giocatore->mano[i + 1];
          }

          free(carta_giocata);
          carta_giocata = NULL;

          for (int i = 0; i < 6; i++) {
            if (i == 5) {
              giocatore->mano[i] = NULL;
              break;
            }
          }

          *n_carte_mano = *n_carte_mano - 1;
      }
    }
   }
  }
}

void attacca(struct Mago *giocatore, struct Mago *giocatore0, unsigned int *n_carte_campoG, unsigned int *n_carte_campoG0) {
  //Il puntatore giocatore si riferisci al mago che attacca
  //Il puntatore giocatore0 identifica il mago che subisce l'attacco
  //Il puntatore n_carte_campoG si riferisce al numero di carte nel campo del mago che attacca
  //Il puntatore n_carte_campoG0 si riferisce al numero di carte nel campo del mago che subisce l'attacco

  //Si controlla se nel campo del mago attaccante ci siano creature:
  int i = 0;
  for (i = 0; i < 4; i++) {
    if (giocatore->campo[i] == NULL) {

    }
    else {
      break;
    }
  }
  if (i == 4) {
    printf("Non è possibile effettuare l'attacco perchè non ci sono creature nel tuo campo\n\n");
  }
  else {

    struct Carta *carta_giocata = giocatore->campo[0]; //Creatura utilizzata per l'attacco

    unsigned int bersaglio = 2;
    printf("\n");
    printf("Per attaccare il mago premere 0\nPer attaccare una delle creature del mago nemico premere 1\nScelta: \n");
    scanf("%u", &bersaglio);
    while(getchar() != '\n');
    printf("\n");

    //Controllo per verificare che l'utente abbia inserito un valore corretto:
    while(bersaglio != 0 && bersaglio != 1) {
      printf("Non hai inserito un valore corretto\nScegli tra 0 e 1: \n");
      scanf("%u", &bersaglio);
      while(getchar() != '\n');
    }

    //Se il giocatore sceglie di attaccare il mago avversario:
    if (bersaglio == 0) {

        printf("Campo di %s:\n", giocatore->nome_giocatore);
        stampa_campo(*n_carte_campoG, giocatore);
        printf("\n");

        unsigned int carta_attaccante = 0;
        printf("Specifica il numero della creatura con la quale effettuare l'attacco: ");
        scanf("%u", &carta_attaccante);
        while(getchar() != '\n');

        //Controllare che il giocatore abbia scelto una sua creatura presente nel campo:
        while(carta_attaccante < 1 || carta_attaccante > *n_carte_campoG) {
          printf("Non hai inserito un valore corretto\n\nScegli tra le creature che ci sono nel tuo campo: \n");
          scanf("%u", &carta_attaccante);
          while(getchar() != '\n');
        }

        //Con il ciclo si individua la creatura da impiegare per l'attacco
        for (int i = 1; i <= *n_carte_campoG; i++) {
          if (carta_attaccante == i) {
            carta_giocata = giocatore->campo[i - 1];
            break;
          }
        }

        giocatore0->punti_vita = giocatore0->punti_vita - carta_giocata->punti_vita;
        printf("L'attacco con la tua creatura è avvenuto con successo!\n");
        printf("\n");
        attacco_effettuato = 1;
      }

      //Se invece il giocatore ha deciso di attaccare una creatura nemica:
      else {

        printf("Campo di %s:\n", giocatore->nome_giocatore);
        stampa_campo(*n_carte_campoG, giocatore);
        printf("\n");
        printf("Campo di %s:\n", giocatore0->nome_giocatore);
        stampa_campo(*n_carte_campoG0, giocatore0);
        printf("\n");

        //Si effettua un controllo anche per verificare se nel campo del mago che viene attaccato ci siano creature:
        int i2 = 0;
        for (i2 = 0; i2 < 4; i2++) {
          if (giocatore0->campo[i2] == NULL) {

          }
          else {
            break;
          }
        }
        if (i2 == 4) {
          printf("Non ci sono creature nel campo del mago avversario!\n\n");
        }
        else { //Se invece c'è almeno una creatura nel campo del mago che attacca:

          struct Carta *carta_attaccata = giocatore0->campo[0];

          unsigned int carta_attaccante = 0;
          printf("Specifica il numero della creatura con la quale effettuare l'attacco: ");
          scanf("%u", &carta_attaccante);
          while(getchar() != '\n');

          //Si ripete il controllo dell'input del mago attaccante:
          while(carta_attaccante < 1 || carta_attaccante > *n_carte_campoG) {
            printf("Non hai inserito un valore corretto\nScegli una delle creature presenti nel tuo campo: \n");
            scanf("%u", &carta_attaccante);
            while(getchar() != '\n');
          }

          //Ciclo che individua la creatura da utilizzare per l'attacco
          for (int i = 1; i <= *n_carte_campoG; i++) {
            if (carta_attaccante == i) {
              carta_giocata = giocatore->campo[i - 1];
              break;
            }
          }

          //Si chiede al mago attaccante quale creatura intende attaccare:

          unsigned int carta_da_attaccare = 0;
          printf("\nInserisci il numero della creatura da attaccare: ");
          scanf("%u", &carta_da_attaccare);
          while(getchar() != '\n');

          //Si ripete il controllo dell'input del mago attaccante:
          while(carta_da_attaccare < 1 || carta_da_attaccare > *n_carte_campoG0) {
            printf("Non hai inserito un valore corretto\nScegli una delle creature nel campo nemico: \n");
            scanf("%u", &carta_da_attaccare);
            while(getchar() != '\n');
          }

          //Si individua la creatura nemica da attaccare
          for (int i = 1; i <= *n_carte_campoG0; i++) {
            if (carta_da_attaccare == i) {
              carta_attaccata = giocatore0->campo[i - 1];
              break;
            }
          }

          carta_attaccata->punti_vita = carta_attaccata->punti_vita - carta_giocata->punti_vita;
          if (carta_attaccata->punti_vita <= 0) {
            printf("La creatura attaccata ha esaurito i suoi punti vita ed è stata distrutta dal campo di gioco\n");

            //La creatura del mago avversario viene eliminata dal campo:
            for (int i = carta_da_attaccare - 1; i < *n_carte_campoG0; i++) {
              giocatore0->campo[i] = giocatore0->campo[i + 1];
            }

            free(carta_attaccata);
            carta_attaccata = NULL;

            for (int i = 0; i < 4; i++) {
              if (i == 3) {
                giocatore0->campo[i] = NULL;
              }
            }

            *n_carte_campoG0 = *n_carte_campoG0 - 1;
          }
          printf("\nL'attacco con la tua creatura è avvenuto con successo!\n");
          attacco_effettuato = 1;
      }
    }
  }
}



