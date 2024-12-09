#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "./modules/h/struct.h"

#include "./modules/h/stats.h"
#include "./modules/h/clients.h"
#include "./modules/h/gestion.h"


void simulerJournee(Stats *statsGlobales) {
    Heure maxsrv = {19, 0};
    ListeAttente liste;
    Stats stats;
    initialiserStats(&stats);

    printf("Génération du fichier clients.txt...\n");
    écrire_clients();

    printf("Création de la liste d'attente...\n");
    creation_liste_attente(&liste, &stats);

    printf("Génération des arrivées des clients\n");
    ajout_arrivée_clients(&liste);

    printf("Supprimer les clients en retard\n");
    supprimerClientsApresHeure(&liste, maxsrv, &stats);

    printf("Liste d'attente triée :\n");
    afficherListe(&liste);

    printf("Affectation aux opérateurs\n");
    gestionOp(&liste, &stats);

    printf("Liste après affectation aux opérateurs :\n");
    afficherListe(&liste);

    printf("\nLibération de la mémoire...\n");
    libererListe(&liste);

    printf("Liste finale :\n");
    afficherListe(&liste);

    // Mise à jour des statistiques globales
    statsGlobales->tailleMin = (stats.tailleMin < statsGlobales->tailleMin) ? stats.tailleMin : statsGlobales->tailleMin;
    statsGlobales->tailleMax = (stats.tailleMax > statsGlobales->tailleMax) ? stats.tailleMax : statsGlobales->tailleMax;
    statsGlobales->sommeTaille += stats.sommeTaille;
    statsGlobales->nbMesures += stats.nbMesures;
    statsGlobales->tempsAttenteMin = (stats.tempsAttenteMin < statsGlobales->tempsAttenteMin) ? stats.tempsAttenteMin : statsGlobales->tempsAttenteMin;
    statsGlobales->tempsAttenteMax = (stats.tempsAttenteMax > statsGlobales->tempsAttenteMax) ? stats.tempsAttenteMax : statsGlobales->tempsAttenteMax;
    statsGlobales->sommeTempsAttente += stats.sommeTempsAttente;
    statsGlobales->nbClientsAvecAttente += stats.nbClientsAvecAttente;
    statsGlobales->clientsPrisEnCharge += stats.clientsPrisEnCharge;
    statsGlobales->clientsNonPrisEnCharge += stats.clientsNonPrisEnCharge;
    statsGlobales->totalAppelsPrisEnCharge += stats.totalAppelsPrisEnCharge;
    statsGlobales->tempsTotal += stats.tempsTotal;
}


int main() {
    int nombre_journee;
    Stats statsGlobales;
    initialiserStats(&statsGlobales);
    printf("Combien de journées voulez-vous simuler ? ");
    scanf("%d", &nombre_journee);

    for (int i = 1; i <= nombre_journee; i++) {
        printf("\n--- Journée %d ---\n", i);
        simulerJournee(&statsGlobales);
    }

    // Affichage des statistiques finales après toutes les journées
    printf("\n--- Statistiques finales après %d journées ---\n", nombre_journee);
    printf("Taille minimale de la file d'attente : %d\n", statsGlobales.tailleMin);
    printf("Taille maximale de la file d'attente : %d\n", statsGlobales.tailleMax);
    printf("Taille moyenne de la file d'attente : %.2f\n", 
           (float)statsGlobales.sommeTaille / statsGlobales.nbMesures);
    printf("Temps minimum d'attente : %d minutes\n", statsGlobales.tempsAttenteMin);
    printf("Temps maximum d'attente : %d minutes\n", statsGlobales.tempsAttenteMax);
    printf("Temps moyen d'attente : %.2f minutes\n", 
           (float)statsGlobales.sommeTempsAttente / statsGlobales.nbClientsAvecAttente);
    printf("Taux de clients pris en charge : %.2f%%\n", calculerTauxPrisEnCharge(&statsGlobales));
    printf("Taux de clients non pris en charge : %.2f%%\n", calculerTauxNonPrisEnCharge(&statsGlobales));

    return 0;
}
