
#include "struct.h" // Inclure la définition de ListeAttente

// Structure pour stocker les statistiques
typedef struct Stats {
    int tailleMin;    // Taille minimale de la file d'attente
    int tailleMax;    // Taille maximale de la file d'attente
    int sommeTaille;  // Somme des tailles (pour le calcul de la moyenne)
    int nbMesures;    // Nombre de mesures effectuées
} Stats;

// Initialiser les statistiques
void initialiserStats(Stats *stats);

// Mettre à jour les statistiques
void mettreAJourStatistiques(ListeAttente *liste, Stats *stats);

// Calculer la moyenne
float calculerMoyenne(Stats *stats);
