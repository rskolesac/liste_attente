#include "./modules/c/clients.c"
#include "./modules/c/gestion.c"


void simulerJournee() {
    ListeAttente liste;
    printf("Génération du fichier clients.txt...\n");
    écrire_clients();

    printf("Création de la liste d'attente...\n");
    creation_liste_attente(&liste);

    printf("génération des arrivées des clients\n");
    ajout_arrivée_clients(&liste); 

    // 5. Afficher la liste d'attente triée
    printf("Liste d'attente triée :\n");
    afficherListe(&liste);

    printf("Affection aux opérateurs\n");
    gestionOp(&liste);

    printf("Liste après affectation aux opérateurs :\n");
    afficherListe(&liste);

    // 6. Libérer la mémoire allouée
    printf("\nLibération de la mémoire...\n");
    libererListe(&liste);

    printf("Liste finale :\n");
    afficherListe(&liste);
}

int main() {
    int nombre_journee;
    printf("combien de journée voulez vous simuler");
    scanf("%d", &nombre_journee);
    for (int i = 1; i < nombre_journee + 1; i++) {
        printf("Journee %d\n", i);
        simulerJournee();
    } 

    return 0;
}


