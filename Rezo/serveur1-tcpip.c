// Exercice 1 :

typedef enum { E_LIBRE, E_LIBRE_REQUETE, E_ECRIRE_REPONSE } Etat;

typedef struct {
  Etat etat;
  int soc;
  struct sockaddr_in adr;
} Slot;

void init_slot (Slot *o) {
  memset(&o->adr, 0, sizeof(o->adr));
  o->soc = -1;
  o->etat = E_LIBRE;
}

int slot_est_libre (Slot *o) {
  if(o->etat = E_LIBRE)
    return 1;

  return -1;
}

void liberer_slot (Slot *o) {
  if(slot_est_libre(o))
    return;

  o->etat = E_LIBRE;
  o->soc.close();
  init_slot(o);
}

// Exercice 2

#define SLOTS_NB 32
typedef struct {
  Slot slots[SLOTS_NB];
  int soc_ec;
  struct sockaddr_in adr;
} Serveur;

void init_serveur(Serveur *ser) {
  serv->soc_ec = -1;
  for(int i = 0, i < SLOTS_NB; i++)
    init_slot(&ser->slots[i]);
}

int slot_est_libre(Slot *o) {
  return o->etat == E_LIBRE;
}

int chercher_slot_libre(Serveur *ser) {
  for(int i = 0; i < SLOTS_NB; i++)
    if(slot_est_libre(&ser->slots[i]))
      return ser->slot[i];

  return -1;
}

// Exercice 3

int demarrer_serveur(Serveur *ser, int port) {
    init_serveur(ser);
    ser->soc_ec = bor_create_socket_in(SOCK_STREAM, port, &ser->adr);
    if(ser->soc_ec < 0)
      return -1;
    if(bor_listen(ser->soc_ec, 8)) {
      close(ser->soc_ec);
      ser->soc_ec = -1;
      return -1
    }

    return 0;
}
void fermer_serveur(Serveur *ser) {
  close(ser->soc_ec);
  serv->soc_ec = -1;

  for(int i = 0; i < SLOTS_NB, i++)
    liberer_slot(&ser->slot[i]);
}

// Exercice 4

int accepter_connexion(Serveur *ser) {
  printf("Serveur : connexion en cours...\n");
  int soc_se = bor_accept_in(ser->soc_ec, &adr_client);

  if(soc_se < 0) return -1;

  int i = chercher_slot_libre(ser);

  if(i < 0) {
    close(soc_se);
    printf("Serveur : Connexion refusée avec %s : trop de clients\n"
                                      , bor_adrtoa_in(&adr_client));

    return 0;
  }

  printf("Serveur[%d] : connexion établie avec %s", i, bor_adrtoa_in(&adr_client));

  Slot *o = &ser->slot[i];
  o->soc = soc_se;
  o->adr = adr_client;
  o->etat = E_LIBRE_REQUETE;

  return 1;
}

// Exercice 5 :

int proceder_lecture__requete(Slot *o) {

}

int proceder_ecriture_reponse(Slot *o) {

}

void traiter_slot_si_eligible(Slot*o, fd_set *set_read, fd_set *set_write) {

  int k = 1;

  if(slot_est_libre(o)) return;

  switch(o->etat) {
    case E_ECRIRE_REPONSE :
        if(FD_ISSET(o->soc, set_write))
          k = proceder_ecriture_reponse(o);
      break;
    case E_LIBRE_REQUETE :
        if(FD_ISSET(o->soc, set_read))
          k = proceder_lecture__requete(o);
      break;
    default :
      printf("Serveur : état invalide....\n");
      break;
  }

  if(k <= 0) {
    printf("Serveur[%d] : libération u slot\n", o->soc);
    liberer_slot(o);
  }
}

// Exercice 6 :

void inser_fd(int fd, fd_set *set, int *maxfd) {
  FD_SET(fd, set);
  if(*maxfd < fd) *maxfd = fd;
}

void preparer_select (Serveur *ser, int *maxfd, fd_set *set_read, fd_set *set_write) {
  FD_ZERO(set_read);
  FD_ZERO(set_write);
  *maxfd = -1;

  for(int i = 0; i < SLOTS_NB; i++) {
    Slot *o = &ser->slots[i];
    if(slot_est_libre(o)) continue;
    switch(o->etat) {
      case E_LIBRE_REQUETE :
        inser_fd(o->soc, set_read, maxfd);
        break;
      case E_ECRIRE_REPONSE :
        inser_fd(o->soc, set_write, maxfd);
        break;
      default : ;
    }
  }
}
