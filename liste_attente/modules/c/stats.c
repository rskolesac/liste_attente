#include "../h/stats.h"
#include "../h/struct.h"
#include <stdlib.h>
#include <stdio.h>

void initialiserStats(Stats *stats) {
    stats->tailleMin = 0;
    stats->tailleMax = 0;
    stats->sommeTaille = 0;
    stats->nbMesures = 0;

    stats->tempsAttenteMin = 0;
    stats->tempsAttenteMax = 0;
    stats->sommeTempsAttente = 0;
    stats->nbClientsAvecAttente = 0;

    stats->totalAppelsPrisEnCharge = 0;
    stats->tempsTotal = 0;

    stats->clientsPrisEnCharge = 0;
    stats->clientsNonPrisEnCharge = 0;
}


void mettreAJourStatistiques(ListeAttente *liste, Stats *stats) {
    if (liste == NULL) return;

    int tailleActuelle = liste->taille;

    if (tailleActuelle < 0) {
        printf("Erreur : taille de la liste négative (%d).\n", tailleActuelle);
        return;
    }
    if (tailleActuelle < stats->tailleMin) {
        stats->tailleMin = tailleActuelle;
    }

    if (tailleActuelle > stats->tailleMax) {
        stats->tailleMax = tailleActuelle;
    }

    stats->sommeTaille += tailleActuelle;
    stats->nbMesures++;
}


float calculerMoyenne(Stats *stats) {
    if (stats->nbMesures == 0) return 0;
    return (float)stats->sommeTaille / stats->nbMesures;
}

void mettreAJourTempsAttente(Stats *stats, int tempsAttente) {

    if (tempsAttente < stats->tempsAttenteMin) {
        stats->tempsAttenteMin = tempsAttente;
    }

    if (tempsAttente > stats->tempsAttenteMax) {
        stats->tempsAttenteMax = tempsAttente;
    }

    stats->sommeTempsAttente += tempsAttente;
    stats->nbClientsAvecAttente++;
}


void mettreAJourTauxEtDebit(Stats *stats, int tempsSimulation, int clientsPris, int clientsNonPris) {
    stats->tempsTotal += tempsSimulation;
    stats->totalAppelsPrisEnCharge += clientsPris;
    stats->clientsPrisEnCharge += clientsPris;
    stats->clientsNonPrisEnCharge += clientsNonPris;
}


float calculerTempsAttenteMoyen(Stats *stats) {
    if (stats->nbClientsAvecAttente == 0) return 0.0;
    return (float)stats->sommeTempsAttente / stats->nbClientsAvecAttente;
}

float calculerDebitJournalier(Stats *stats) {
    if (stats->tempsTotal == 0) return 0.0;
    return (float)stats->totalAppelsPrisEnCharge / stats->tempsTotal;
}

float calculerTauxPrisEnCharge(Stats *stats) {
    int totalClients = stats->clientsPrisEnCharge + stats->clientsNonPrisEnCharge;
    if (totalClients == 0) return 0.0;
    return (float)stats->clientsPrisEnCharge / totalClients * 100;
}

float calculerTauxNonPrisEnCharge(Stats *stats) {
    int totalClients = stats->clientsPrisEnCharge + stats->clientsNonPrisEnCharge;
    if (totalClients == 0) return 0.0;
    return (float)stats->clientsNonPrisEnCharge / totalClients * 100;
}
