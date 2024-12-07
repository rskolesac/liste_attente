#include "./modules/c/clients.c"
#include "./modules/c/gestion.c"
/* 
   // 1. Générer le fichier clients.txt1
    printf("Génération du fichier clients.txt...\n");
    ajout_client();
 */
int main() {
    ListeAttente liste;
    printf("Génération du fichier clients.txt...\n");
    écrire_clients();

    printf("Création de la liste d'attente...\n");
    creation_liste_attente(&liste);

    printf("génération des arrivées des clients\n");
    ajout_arrivée_clients(&liste); 


    printf("Liste d'attente non triée :\n");
    afficherListe(&liste);

    // 4. Trier la liste d'attente par heure
    printf("\nOrganisation de la liste d'attente...\n");
    organisation_liste_attente(&liste);

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

    return 0;
}


