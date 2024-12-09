
#ifndef STATS_H
#define STATS_H

#include "struct.h"

void initialiserStats(Stats *stats);

void mettreAJourStatistiques(ListeAttente *liste, Stats *stats);

float calculerMoyenne(Stats *stats);

void mettreAJourTempsAttente(Stats *stats, int tempsAttente);

void mettreAJourTauxEtDebit(Stats *stats, int tempsSimulation, int clientsPris, int clientsNonPris);

float calculerTempsAttenteMoyen(Stats *stats);

float calculerDebitJournalier(Stats *stats);

float calculerTauxPrisEnCharge(Stats *stats);

float calculerTauxNonPrisEnCharge(Stats *stats);

#endif 

