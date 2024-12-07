#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./struct.h"
#include "./alea.c"


void écrire_clients() {
    int nombre_client;
    FILE* fichier;
    fichier = fopen("./modules/texte/clients.txt", "w");
    printf("combien de client allons-nous mettre aujourd'hui ");
    scanf("%d", &nombre_client);
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
    while (fscanf(fichier, "%d", &c.indice) == 1) {  // Lecture de l'indice du client
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
    double lambda;
    do {
        printf("Entrez le paramètre lambda pour les arrivées (entre 0 et 1) : \n");
        scanf("%lf", &lambda);

        if (lambda < 0 || lambda > 1) {
            printf("Erreur : lambda doit être compris entre 0 et 1.\n");
        }
    } while (lambda < 0 || lambda > 1);

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

void organisation_liste_attente(ListeAttente *liste) {
    // Créer une nouvelle liste d'attente triée
    N_Noeud *trie = NULL;
    
    // Récupérer la liste d'attente non triée à partir de son premier élément
    N_Noeud *courant = liste->premier;

    while (courant != NULL) {
        N_Noeud *suivant = courant->suivant;

        // Si la liste triée est vide ou l'élément courant doit être inséré en tête de la liste triée
        if (trie == NULL || 
            // Si l'heure du courant est plus petite que celle de la tête de la liste triée
            (courant->client.t.h.heure < trie->client.t.h.heure ||
            (courant->client.t.h.heure == trie->client.t.h.heure &&
             courant->client.t.h.minute < trie->client.t.h.minute))) {
            // Insérer l'élément en début de liste triée
            courant->suivant = trie;
            trie = courant;
        }
        else {
            // Trouver la position d'insertion dans la liste triée
            N_Noeud *temp = trie;
            while (temp->suivant != NULL && 
                   (temp->suivant->client.t.h.heure < courant->client.t.h.heure ||
                   (temp->suivant->client.t.h.heure == courant->client.t.h.heure &&
                    temp->suivant->client.t.h.minute <= courant->client.t.h.minute))) {
                temp = temp->suivant;
            }

            // Insérer l'élément au bon endroit dans la liste triée
            courant->suivant = temp->suivant;
            temp->suivant = courant;
        }
        
        // Passer au client suivant dans la liste non triée
        courant = suivant;
    }

    // Mettre à jour la tête de la liste avec la liste triée
    liste->premier = trie;
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

