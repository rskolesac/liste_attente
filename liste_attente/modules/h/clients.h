#ifndef CLIENTS_H
#define CLIENTS_H

#include "struct.h"

void écrire_clients();

void creation_liste_attente(ListeAttente *liste, Stats *stats);

void ajout_arrivée_clients(ListeAttente *liste);

void supprimerClientsApresHeure(ListeAttente *liste, Heure heureMax, Stats *stats);

void afficherListe(ListeAttente *liste);

void libererListe(ListeAttente *liste);

#endif