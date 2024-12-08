#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./struct.h"
#include "./alea.c"


void écrire_clients() {
    int nombre_client;
    nombre_client = rand() % 100;
    printf("le nombre de clients recu dans la liste d'attente aujourd'hui est de %d\n", nombre_client);
    FILE* fichier;
    fichier = fopen("./modules/texte/clients.txt", "w");
    for (int i = 1; i < nombre_client +1; i++) {
        fprintf(fichier, "%d\n", i);
    } 
    fclose(fichier);
}

void creation_liste_attente(ListeAttente *liste) {
    Clients c;
    liste->premier = NULL;  // Initialisation de la tête de la liste à NULL
    FILE* fichier = fopen("./modules/texte/clients.txt", "r");  // Ouverture du fichier
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier\n");
        return;  // Quitter la fonction si le fichier ne peut pas être ouvert
    }

    // Insérer les clients dans la liste chaînée
    while (fscanf(fichier, "%d", &c.indice) == 1) {
        liste->taille++;
        // Allouer de la mémoire pour un nouveau noeud
        N_Noeud *nouveau = (N_Noeud *)malloc(sizeof(N_Noeud));
        if (nouveau == NULL) {
            printf("Erreur de mémoire\n");
            fclose(fichier);
            return;
        }

        // Copie des données du client dans le nouveau noeud
        nouveau->client = c;
        nouveau->suivant = NULL;  // Initialisation du pointeur suivant à NULL

        // Si la liste est vide, le nouveau client devient le premier
        if (liste->premier == NULL) {
            liste->premier = nouveau;
        } else {
            // Si la liste n'est pas vide, trouver la position pour insérer en ordre croissant
            N_Noeud *courant = liste->premier;
            N_Noeud *précédent = NULL;

            // Trouver l'endroit où insérer le client en respectant l'ordre croissant
            while (courant != NULL && courant->client.indice < c.indice) {
                précédent = courant;
                courant = courant->suivant;
            }

            // Insertion en tête si la liste est vide ou si l'indice est plus petit que le premier
            if (précédent == NULL) {
                nouveau->suivant = liste->premier;
                liste->premier = nouveau;
            } else {
                // Insertion après le nœud précédent
                précédent->suivant = nouveau;
                nouveau->suivant = courant;
            }
        }

        // Initialisation de la durée du client (ou autres champs si nécessaire)
        nouveau->client.t.durée.heure = 0;
        nouveau->client.t.durée.minute = 0;
    }

    fclose(fichier);  // Fermeture du fichier
}



void ajout_arrivée_clients(ListeAttente *liste) {
    if (liste == NULL || liste->premier == NULL) {
        printf("Erreur : la liste est vide ou non initialisée.\n");
        return;
    }
    Heure minsrv = {8, 0};
    Heure maxsrv = {19, 0};
    float lambda = (float)rand() / (float)RAND_MAX;

    N_Noeud *courant = liste->premier;
    N_Noeud *précédent = NULL;

    courant->client.t.h = minsrv;
    // on génère une heure d'arrivé aléatoire pour le premier type de la liste d'attente
    int temps_arrivée_premier = frequence_arrive(lambda);
    while (temps_arrivée_premier > 60) {
        courant->client.t.h.heure += 1;
        temps_arrivée_premier -=60;
    }
    courant->client.t.h.minute = temps_arrivée_premier;

    précédent = courant;
    courant = courant->suivant;

    while (courant != NULL) {
        int temps_arrivée = frequence_arrive(lambda);
        // on va générer des arrivées successives par rapport à l'heure d'arrivée du type avant.
        courant->client.t.h.heure = précédent->client.t.h.heure;
        courant->client.t.h.minute = précédent->client.t.h.minute;
        while (temps_arrivée > 60) {
            courant->client.t.h.heure += 1;
            temps_arrivée -= 60;
        }
        courant->client.t.h.minute += temps_arrivée;
        while (courant->client.t.h.minute > 60) { 
            // conversion heure minute
            courant->client.t.h.heure +=1;
            courant->client.t.h.minute -= 60;
        }  
        précédent = courant;
        courant = courant->suivant;

    }
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

