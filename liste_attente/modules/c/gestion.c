    #include <stdbool.h>
    #include <stdlib.h>
    #include "./struct.h"
    #include "./alea2.c"

    int nombreOperateur = -1; 



    typedef struct Operateur{
        int index;  // Identifiant de l'opérateur
        bool isOperateurLibre;  // Indique si l'opérateur est libre ou occupé
        Clients *client;  // Pointeur vers le client que l'opérateur gère actuellement
        int heureDeLiberation;  // Heure de libération de l'opérateur (en minutes)
    } Operateur;


    void dmdNombreOperateur(int *nombreOperateur) {
        if (*nombreOperateur == -1) {
            *nombreOperateur = rand() % 100; // Modifier la valeur pointée
        }
    }


    Operateur* initOperateur(int *nombreOperateur) {
        dmdNombreOperateur(nombreOperateur);  // Demander le nombre d'opérateurs à l'utilisateur
        Operateur *operateurs = (Operateur *)malloc(sizeof(Operateur) * (*nombreOperateur));

        if (operateurs == NULL) {
            printf("Erreur d'allocation mémoire pour les opérateurs\n");
            exit(1);  // Quitter en cas d'erreur d'allocation mémoire
        }

        // Initialiser tous les opérateurs comme disponibles
        for (int i = 0; i < *nombreOperateur; i++) {  // Corrigé pour commencer à l'indice 0
            operateurs[i].index = i + 1;  // Attribuer un numéro d'opérateur (commence à 1)
            operateurs[i].isOperateurLibre = true;
            operateurs[i].client = NULL;  // Aucun client n'est encore associé
        }

        return operateurs;
    }


    void libérerOperateur(Operateur *operateurs, int nombreOperateur, N_Noeud *client) {
        // Calculer l'heure de fin pour cet opérateur
        int heure_fin = client->client.départ.heure + client->client.t.durée.heure;
        int minute_fin = client->client.départ.minute + client->client.t.durée.minute;

        // Si les minutes dépassent 60, ajuster l'heure et les minutes
        if (minute_fin >= 60) {
            heure_fin += minute_fin / 60;
            minute_fin %= 60;
        }

        // Mettre à jour l'heure de fin dans le client
        client->client.départ.heure = heure_fin;
        client->client.départ.minute = minute_fin;

        printf("Operateur n°%d termine son échange avec le client %d à %dh%d\n", 
            operateurs[client->client.indice].index, client->client.indice, heure_fin, minute_fin);
    }

    void calculerHeureDeLiberation(Operateur *operateur, Clients *client) {
        // Calcul de l'heure de départ du client (heure d'arrivée + durée de l'échange)
        client->départ.heure = client->t.h.heure + client->t.durée.heure;
        client->départ.minute = client->t.h.minute + client->t.durée.minute;

        // Ajuster l'heure si les minutes dépassent 60
        if (client->départ.minute >= 60) {
            client->départ.heure += client->départ.minute / 60;
            client->départ.minute %= 60;
        }

        // Calcul de l'heure de libération de l'opérateur (en minutes)
        int heureFinOperateur = client->départ.heure * 60 + client->départ.minute;
        
        // Mettre à jour l'heure de libération de l'opérateur
        operateur->heureDeLiberation = heureFinOperateur;
    }

    int calculerHeureDeLiberationMinimale(Operateur *operateurs, int nombreOperateurs) {
        int heureMinimale = 24 * 60;  // 24h en minutes, donc max possible (c'est une heure impossible)
        
        for (int i = 0; i < nombreOperateurs; i++) {
            if (!operateurs[i].isOperateurLibre) {
                // L'opérateur est occupé, donc on considère son heure de libération
                int heureLiberation = operateurs[i].heureDeLiberation;
                if (heureLiberation < heureMinimale) {
                    heureMinimale = heureLiberation;
                }
            }
        }
        return heureMinimale;  // Retourner l'heure minimale
    }

    void associerClientsAuxOperateurs(ListeAttente *liste, Operateur *operateurs, int nombreOperateurs) {
        N_Noeud *courant = liste->premier;  // Pointer sur le premier client de la liste

        // Première boucle pour affecter les opérateurs aux clients
        for (int i = 0; i < nombreOperateurs; i++) {
            if (courant == NULL) {
                printf("Il n'y a plus de clients à associer aux opérateurs.\n");
                break;
            }

            // Créer la durée de l'échange pour ce client
            Heure durée_échange = créer_durée();
            printf("Durée de l'échange pour le client n°%d : %dh%d\n", courant->client.indice, durée_échange.heure, durée_échange.minute);

            // Associer l'opérateur au client
            operateurs[i].isOperateurLibre = false;  // L'opérateur devient occupé
            operateurs[i].client = &courant->client;  // Associer le client à l'opérateur

            // Affectation de la durée de l'échange au client
            courant->client.t.durée = durée_échange;

            // Calculer l'heure de libération de l'opérateur
            calculerHeureDeLiberation(&operateurs[i], &courant->client);

            // Afficher l'heure de libération de cet opérateur
            printf("Opérateur n°%d est associé au client n°%d à %dh%d\n", 
                operateurs[i].index, courant->client.indice, courant->client.t.h.heure, courant->client.t.h.minute);
            printf("L'opérateur n°%d sera libéré à %dh%d.\n", operateurs[i].index,
                operateurs[i].heureDeLiberation / 60, operateurs[i].heureDeLiberation % 60);

            // Suppression du client de la liste d'attente
            liste->taille--;
            liste->premier = courant->suivant;
            free(courant);
            courant = liste->premier;  // Passer au client suivant
        }

        // Mettre à jour la tête de la liste d'attente pour pointer sur les clients restants
        liste->premier = courant;
    }




    Operateur* trouverOperateurDisponibleLePlusTot(Operateur *operateurs, int nombreOperateurs, int heureClient) {
        Operateur* operateurLibre = NULL;
        
        // Recherche de l'opérateur libre le plus tôt
        for (int i = 0; i < nombreOperateurs; i++) {
            if (operateurs[i].isOperateurLibre || operateurs[i].heureDeLiberation <= heureClient) {
                if (operateurLibre == NULL || operateurs[i].heureDeLiberation < operateurLibre->heureDeLiberation) {
                    operateurLibre = &operateurs[i];
                }
            }
        }

        return operateurLibre;  // Retourne NULL si aucun opérateur n'est disponible
    }

    void affecterClient(Operateur *operateurs, int nombreOperateurs, N_Noeud *clientNode, int heureLiberationMinimale) {
        // Créer la durée de l'échange pour ce client
        Heure durée_échange = créer_durée();
        printf("Durée de l'échange pour le client n°%d : %dh%d\n", clientNode->client.indice, durée_échange.heure, durée_échange.minute);
        
        // Trouver l'opérateur avec l'heure de libération la plus proche de l'heure minimale calculée
        Operateur* operateurLibre = NULL;
        for (int i = 0; i < nombreOperateurs; i++) {
            if (operateurs[i].isOperateurLibre || operateurs[i].heureDeLiberation <= heureLiberationMinimale) {
                operateurLibre = &operateurs[i];
                break;
            }
        }

        if (operateurLibre != NULL) {
            // Associer le client à l'opérateur
            operateurLibre->isOperateurLibre = false;
            operateurLibre->client = &clientNode->client;

            // Calculer l'heure de libération après l'échange
            calculerHeureDeLiberation(operateurLibre, &clientNode->client);

            // Afficher l'heure de prise en charge de l'opérateur
            printf("L'opérateur n°%d est associé au client n°%d à %dh%d\n", 
                operateurLibre->index, clientNode->client.indice, 
                heureLiberationMinimale / 60, heureLiberationMinimale % 60);

            // Calcul de l'heure de libération de l'opérateur, en gérant l'overflow des minutes
            int lib_heure = heureLiberationMinimale / 60 + durée_échange.heure;
            int lib_minute = heureLiberationMinimale % 60 + durée_échange.minute;

            // Si les minutes dépassent 60, on ajuste l'heure et les minutes
            if (lib_minute >= 60) {
                lib_heure += lib_minute / 60;
                lib_minute %= 60;  // Garde la partie des minutes en dessous de 60
            }

            operateurLibre->heureDeLiberation = lib_heure * 60 + lib_minute;
            // Afficher l'heure de libération de l'opérateur
            printf("L'opérateur n°%d sera libéré à %dh%d.\n\n", operateurLibre->index, lib_heure, lib_minute);

        } else {
            // Aucun opérateur disponible
            printf("Aucun opérateur disponible pour le client n°%d.\n", clientNode->client.indice);
        }
    }





    void affecterOperateur(ListeAttente *liste, Operateur *operateurs, int nombreOperateurs) {
        N_Noeud *courant = liste->premier;  // Pointer sur le premier client de la liste

        // Si la liste est vide, il n'y a pas de clients à affecter
        if (courant == NULL) {
            printf("Il n'y a plus de clients à affecter.\n");
            return;
        }

        // Parcourir les clients dans la liste et affecter un opérateur
        while (courant != NULL) {
            
            int heureLiberationMinimale = calculerHeureDeLiberationMinimale(operateurs, nombreOperateurs);
            affecterClient(operateurs, nombreOperateurs, courant, heureLiberationMinimale);

            // Suppression du client de la liste d'attente
            liste->taille--; // Incrémenter la taille
            liste->premier = courant->suivant;
            free(courant);
            courant = liste->premier;  // Passer au client suivant
        }
    }





    void gestionOp(ListeAttente *liste) {
        Operateur *operateurs = initOperateur(&nombreOperateur);  // Initialisation des opérateurs

        // Étape 1: Associer les premiers clients aux opérateurs disponibles dès le début
        printf("Début de l'association des clients aux opérateurs.\n");
        associerClientsAuxOperateurs(liste, operateurs, nombreOperateur);

        // Étape 2: Si des clients restent dans la liste d'attente, les affecter à des opérateurs
        printf("Début de l'affectation des clients restants aux opérateurs.\n");
        affecterOperateur(liste, operateurs, nombreOperateur);

        // Libérer la mémoire allouée pour les opérateurs
        free(operateurs);
    }







