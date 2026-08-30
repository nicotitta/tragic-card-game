
enum classe_Mago {tenebre = 1, vita, luce};

enum Tipo_carta {creatura, rimuovi_creatura, infliggi_danno, guarisci_danno};

struct Carta {
  enum Tipo_carta tipo_carta;
  int punti_vita;
  struct Carta *carta_successiva;
};

struct Mago {
  char nome_giocatore[256];
  int punti_vita; //Questo campo è stato inizializzato a 20 in gamelib.c
  enum classe_Mago mago;
  struct Carta *inizioMazzo;
  struct Carta *fineMazzo;
  struct Carta *campo[4];
  struct Carta *mano[6];
};


void imposta_gioco(void);
void combatti(void);
void termina_gioco(void);



