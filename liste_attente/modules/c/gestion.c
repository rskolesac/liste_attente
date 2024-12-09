#include <stdbool.h>
#include <stdlib.h>
#include "stats.h"
#include "struct.h"
#include "./alea2.c"



int nombreOperateur = -1; 

typedef struct Operateur{
    int index;
    bool isOperateurLibre; 
    Clients *client; 
    int heureDeLiberation; 
} Operateur;


void dmdNombreOperateur(int *nombreOperateur) {
    if (*nombreOperateur == -1) {
        *nombreOperateur = (rand() % 15) + 1; 
        printf("nombre opérateur aujourd'hui : %d\n", *nombreOperateur);
    }
}


Operateur* initOperateur(int *nombreOperateur) {
    dmdNombreOperateur(nombreOperateur); 
    Operateur *operateurs = (Operateur *)malloc(sizeof(Operateur) * (*nombreOperateur));

    if (operateurs == NULL) {
        printf("Erreur d'allocation mémoire pour les opérateurs\n"); 
    }

    for (int i = 0; i < *nombreOperateur; i++) {  
        operateurs[i].index = i + 1; 
        operateurs[i].isOperateurLibre = true;
        operateurs[i].client = NULL;  
    }

    return operateurs;
}


void libérerOperateur(Operateur *operateurs, int nombreOperateur, N_Noeud *client) {
    int heure_fin = client->client.départ.heure + client->client.t.durée.heure;
    int minute_fin = client->client.départ.minute + client->client.t.durée.minute;

    if (minute_fin >= 60) {
        heure_fin += minute_fin / 60;
        minute_fin %= 60;
    }
    client->client.départ.heure = heure_fin;
    client->client.départ.minute = minute_fin;

    printf("Operateur n°%d termine son échange avec le client %d à %dh%d\n", 
        operateurs[client->client.indice].index, client->client.indice, heure_fin, minute_fin);
}

void calculerHeureDeLiberation(Operateur *operateur, Clients *client) {

    client->départ.heure = client->t.h.heure + client->t.durée.heure;
    client->départ.minute = client->t.h.minute + client->t.durée.minute;

    if (client->départ.minute >= 60) {
        client->départ.heure += client->départ.minute / 60;
        client->départ.minute %= 60;
    }

    int heureFinOperateur = client->départ.heure * 60 + client->départ.minute;
        
    operateur->heureDeLiberation = heureFinOperateur;
}

int calculerHeureDeLiberationMinimale(Operateur *operateurs, int nombreOperateurs) {
    int heureMinimale = 24 * 60; 
        
    for (int i = 0; i < nombreOperateurs; i++) {
        if (!operateurs[i].isOperateurLibre) {
            int heureLiberation = operateurs[i].heureDeLiberation;
            if (heureLiberation < heureMinimale) {
                heureMinimale = heureLiberation;
            }
        }
    }
    return heureMinimale; 
}


int calculerTempsAttente(Heure heureArrivee, Heure heureDebutTraitement) {
    int arriveeMinutes = heureArrivee.heure * 60 + heureArrivee.minute;
    int debutMinutes = heureDebutTraitement.heure * 60 + heureDebutTraitement.minute;
    return debutMinutes - arriveeMinutes;
}




void associerClientsAuxOperateurs(ListeAttente *liste, Operateur *operateurs, int nombreOperateurs, Stats *stats) {
    N_Noeud *courant = liste->premier; 
    int clientsPris = 0;  
    int clientsNonPris = 0;  

    for (int i = 0; i < nombreOperateurs; i++) {
        if (courant == NULL) {
            printf("Il n'y a plus de clients à associer aux opérateurs.\n");
            break;
        }

        Heure durée_échange = créer_durée();
        printf("Durée de l'échange pour le client n°%d : %dh%d\n", courant->client.indice, durée_échange.heure, durée_échange.minute);

        Heure heureDebutTraitement = (operateurs[i].heureDeLiberation == 0)
            ? courant->client.t.h
            : (Heure){operateurs[i].heureDeLiberation / 60, operateurs[i].heureDeLiberation % 60};

        int tempsAttente = calculerTempsAttente(courant->client.t.h, heureDebutTraitement);
        printf("Temps d'attente pour le client n°%d : %d minutes\n", courant->client.indice, tempsAttente);

        mettreAJourTempsAttente(stats, tempsAttente);

        operateurs[i].isOperateurLibre = false;
        operateurs[i].client = &courant->client; 
        courant->client.estPrisEnCharge = 1; 
        clientsPris++;  

        courant->client.t.durée = durée_échange;

        calculerHeureDeLiberation(&operateurs[i], &courant->client);

        printf("Opérateur n°%d est associé au client n°%d à %dh%d\n", 
            operateurs[i].index, courant->client.indice, courant->client.t.h.heure, courant->client.t.h.minute);
        printf("L'opérateur n°%d sera libéré à %dh%d.\n", operateurs[i].index,
            operateurs[i].heureDeLiberation / 60, operateurs[i].heureDeLiberation % 60);

        liste->taille--;
        liste->premier = courant->suivant;
        free(courant);
        courant = liste->premier; 
    }

    // Mise à jour des statistiques globales
    mettreAJourTauxEtDebit(stats, 0, clientsPris, clientsNonPris); 
    printf("Clients pris en charge dans associerClientsAuxOperateurs : %d\n", clientsPris);
}






Operateur* trouverOperateurDisponibleLePlusTot(Operateur *operateurs, int nombreOperateurs, int heureClient) {
    Operateur* operateurLibre = NULL;
        
    for (int i = 0; i < nombreOperateurs; i++) {
        if (operateurs[i].isOperateurLibre || operateurs[i].heureDeLiberation <= heureClient) {
            if (operateurLibre == NULL || operateurs[i].heureDeLiberation < operateurLibre->heureDeLiberation) {
                operateurLibre = &operateurs[i];
            }
        }
    }

    return operateurLibre;
}

void affecterClient(ListeAttente *liste, Operateur *operateurs, int nombreOperateurs, N_Noeud *courant, int heureLiberationMinimale, Stats *stats) {
    Heure durée_échange = créer_durée();
    printf("Durée de l'échange pour le client n°%d : %dh%d\n", courant->client.indice, durée_échange.heure, durée_échange.minute);

    Operateur* operateurLibre = NULL;
    for (int i = 0; i < nombreOperateurs; i++) {
        if (operateurs[i].isOperateurLibre || operateurs[i].heureDeLiberation <= heureLiberationMinimale) {
            operateurLibre = &operateurs[i];
            break;
        }
    }

    if (operateurLibre != NULL) {
        Heure heureDebutTraitement = (operateurLibre->heureDeLiberation == 0) ? courant->client.t.h : (Heure){operateurLibre->heureDeLiberation / 60, operateurLibre->heureDeLiberation % 60};

        if (courant->client.t.h.heure > heureDebutTraitement.heure || 
            (courant->client.t.h.heure == heureDebutTraitement.heure && courant->client.t.h.minute > heureDebutTraitement.minute)) {
            heureDebutTraitement = courant->client.t.h;
        }

        printf("Début traitement : %02d:%02d\n", heureDebutTraitement.heure, heureDebutTraitement.minute);
        printf("Heure arrivée client : %02d:%02d\n", courant->client.t.h.heure, courant->client.t.h.minute);

        int tempsAttente = calculerTempsAttente(courant->client.t.h, heureDebutTraitement);
        printf("Temps d'attente pour le client n°%d : %d minutes\n", courant->client.indice, tempsAttente);

        mettreAJourTempsAttente(stats, tempsAttente);

        operateurLibre->isOperateurLibre = false;
        operateurLibre->client = &courant->client;
        courant->client.estPrisEnCharge = 1;

        // Calculer l'heure de libération après l'échange
        int lib_heure = heureDebutTraitement.heure + durée_échange.heure;
        int lib_minute = heureDebutTraitement.minute + durée_échange.minute;

        if (lib_minute >= 60) {
            lib_heure += lib_minute / 60;
            lib_minute %= 60;
        }

        operateurLibre->heureDeLiberation = lib_heure * 60 + lib_minute;

        printf("L'opérateur n°%d est associé au client n°%d à %02dh%02d\n", 
               operateurLibre->index, courant->client.indice, 
               heureDebutTraitement.heure, heureDebutTraitement.minute);
        printf("L'opérateur n°%d sera libéré à %02dh%02d.\n\n", operateurLibre->index, lib_heure, lib_minute);

        // Mettre à jour les statistiques globales pour les clients pris
        mettreAJourTauxEtDebit(stats, 0, 1, 0); 

    } else {
        printf("Aucun opérateur disponible pour le client n°%d.\n", courant->client.indice);

        mettreAJourTauxEtDebit(stats, 0, 0, 1);
    }
}

void affecterOperateur(ListeAttente *liste, Operateur *operateurs, int nombreOperateurs, Stats *stats) {
    N_Noeud *courant = liste->premier;

    if (courant == NULL) {
        printf("Il n'y a plus de clients à affecter.\n");
        return;
    }

    while (courant != NULL) {
        int heureLiberationMinimale = calculerHeureDeLiberationMinimale(operateurs, nombreOperateurs);

        affecterClient(liste, operateurs, nombreOperateurs, courant, heureLiberationMinimale, stats);

        liste->premier = courant->suivant;
        liste->taille--;
        free(courant);

        courant = liste->premier;
    }
}







void gestionOp(ListeAttente *liste, Stats *stats) {
    // Initialisation des opérateurs
    Operateur *operateurs = initOperateur(&nombreOperateur);

    printf("Début de l'association des clients aux opérateurs.\n");
    associerClientsAuxOperateurs(liste, operateurs, nombreOperateur, stats);

    // Traitement de tous les clients restants
    while (liste->premier != NULL) {
        affecterOperateur(liste, operateurs, nombreOperateur, stats);
    }

    // Gérer les clients non pris en charge restants
    if (liste->taille > 0) {
        stats->clientsNonPrisEnCharge += liste->taille; 
        printf("Clients restants non pris en charge : %d\n", liste->taille);
    }
    stats->tempsTotal = 9 * 60; // 9 heures converties en minutes

    // Calcul du débit (clients pris en charge par minute)
    double debit = 0.0;
    if (stats->tempsTotal > 0) {
        debit = (double)stats->clientsPrisEnCharge / stats->tempsTotal;
    }

    // Résumé de la journée
    printf("Résumé de la journée :\n");
    printf(" - Clients pris en charge : %d\n", stats->clientsPrisEnCharge);
    printf(" - Clients non pris en charge : %d\n", stats->clientsNonPrisEnCharge);
    printf(" - Débit : %.2f clients/minute\n", debit);

    // Libération des ressources
    free(operateurs);
}













