#ifndef GESTION_H
#define GESTION_H

#include <stdbool.h>
#include "stats.h"
#include "struct.h"


typedef struct Operateur {
    int index;               
    bool isOperateurLibre;   
    Clients *client;         
    int heureDeLiberation;    
} Operateur;


void dmdNombreOperateur(int *nombreOperateur);


Operateur* initOperateur(int *nombreOperateur);


void libérerOperateur(Operateur *operateurs, int nombreOperateur, N_Noeud *client);


void calculerHeureDeLiberation(Operateur *operateur, Clients *client);


Operateur* trouverOperateurDisponibleLePlusTot(Operateur *operateurs, int nombreOperateurs, int heureClient);


void associerClientsAuxOperateurs(ListeAttente *liste, Operateur *operateurs, int nombreOperateurs, Stats *stats);


void affecterClient(ListeAttente *liste, Operateur *operateurs, int nombreOperateurs, N_Noeud *clientNode, int heureLiberationMinimale, Stats *stats);


void affecterOperateur(ListeAttente *liste, Operateur *operateurs, int nombreOperateurs, Stats *stats);


void gestionOp(ListeAttente *liste, Stats *stats);


int calculerHeureDeLiberationMinimale(Operateur *operateurs, int nombreOperateurs);

#endif
