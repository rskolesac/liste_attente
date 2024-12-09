#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../h/alea.h"
#include "../h/stats.h"


void écrire_clients() {
    int nombre_client;
    nombre_client = (rand() % 101) + 100;

    printf("le nombre de clients recu dans la liste d'attente aujourd'hui est de %d\n", nombre_client);
    FILE* fichier;
    fichier = fopen("./modules/texte/clients.txt", "w");
    for (int i = 1; i < nombre_client +1; i++) {
        fprintf(fichier, "%d\n", i);
    } 
    fclose(fichier);
}

void creation_liste_attente(ListeAttente *liste, Stats *stats) {
    Clients c;
    liste->premier = NULL;  
    liste->taille = 0;  
    FILE* fichier = fopen("./modules/texte/clients.txt", "r");  
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;  
    }
    while (fscanf(fichier, "%d", &c.indice) == 1) {
        N_Noeud *nouveau = (N_Noeud *)malloc(sizeof(N_Noeud));
        if (nouveau == NULL) {
            printf("Erreur de mémoire\n");
            fclose(fichier);
            return;
        }

        nouveau->client = c;
        nouveau->suivant = NULL;  


        if (liste->premier == NULL) {
            liste->premier = nouveau;
        } else {

            N_Noeud *courant = liste->premier;
            N_Noeud *précédent = NULL;

            while (courant != NULL && courant->client.indice < c.indice) {
                précédent = courant;
                courant = courant->suivant;
            }

            if (précédent == NULL) {
                nouveau->suivant = liste->premier;
                liste->premier = nouveau;
            } else {
                précédent->suivant = nouveau;
                nouveau->suivant = courant;
            }
        }

        nouveau->client.t.durée.heure = 0;
        nouveau->client.t.durée.minute = 0;
        liste->taille++;
        mettreAJourStatistiques(liste, stats);
    }

    fclose(fichier);
}


void ajout_arrivée_clients(ListeAttente *liste) {
    if (liste == NULL || liste->premier == NULL) {
        printf("Erreur : la liste est vide ou non initialisée.\n");
        return;
    }

    Heure minsrv = {8, 0}; 
    float lambda = 0.2 * ((float)rand() / (float)(RAND_MAX + 1.0)) + 0.000001;

    N_Noeud *courant = liste->premier;

    courant->client.t.h = minsrv;
    int temps_arrivée_premier = frequence_arrive(lambda);

    courant->client.t.h.minute += temps_arrivée_premier;
    while (courant->client.t.h.minute >= 60) {
        courant->client.t.h.heure += 1;
        courant->client.t.h.minute -= 60;
    }

    N_Noeud *précédent = courant;
    courant = courant->suivant;

    while (courant != NULL) {
        int temps_arrivée = frequence_arrive(lambda);

        courant->client.t.h.heure = précédent->client.t.h.heure;
        courant->client.t.h.minute = précédent->client.t.h.minute + temps_arrivée;

        while (courant->client.t.h.minute >= 60) {
            courant->client.t.h.heure += 1;
            courant->client.t.h.minute -= 60;
        }

        précédent = courant;
        courant = courant->suivant;
    }

    printf("Heures d'arrivée assignées à tous les clients.\n");
}


void supprimerClientsApresHeure(ListeAttente *liste, Heure heureMax, Stats *stats) {
    if (liste == NULL || liste->premier == NULL) {
        printf("La liste d'attente est vide ou non initialisée.\n");
        return;
    }

    N_Noeud *courant = liste->premier;
    N_Noeud *précédent = NULL;

    while (courant != NULL) {
        if (courant->client.t.h.heure > heureMax.heure ||
            (courant->client.t.h.heure == heureMax.heure && courant->client.t.h.minute > heureMax.minute)) {

            if (précédent == NULL) {
                liste->premier = courant->suivant;
                free(courant);
                courant = liste->premier;
            } else {
                précédent->suivant = courant->suivant;
                free(courant);
                courant = précédent->suivant;
            }

            liste->taille--;
            
            stats->clientsNonPrisEnCharge++;
        } else {
            précédent = courant;
            courant = courant->suivant;
        }
    }
    printf("On a supprimé %d clients\n", stats->clientsNonPrisEnCharge);
}






void afficherListe(ListeAttente *liste) {
    N_Noeud *courant = liste->premier;
    while (courant != NULL) {
        printf("Clients n'%d, Heure: %02d:%02d, Durée de l'échange: %d:%d\n",
               courant->client.indice,
               courant->client.t.h.heure, courant->client.t.h.minute,
               courant->client.t.durée.heure, courant->client.t.durée.minute);
        courant = courant->suivant;
    }
}

void libererListe(ListeAttente *liste) {
    N_Noeud *courant = liste->premier;
    while (courant != NULL) {
        N_Noeud *temp = courant;
        courant = courant->suivant;
        free(temp);
    }
    liste->premier = NULL;
}

