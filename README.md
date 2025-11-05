 Cashier System Simulation
Objectif
Ce projet simule le comportement d’un système de caisse en magasin :
arrivée des clients,
files d’attente,
ouverture et fermeture des caisses,
temps de service,
et départs des clients.
L’objectif est d’évaluer les indicateurs de performance (KPI) tels que :
⏱ Temps d’attente moyen,
 Temps total passé dans le système,
 Taux d’utilisation des caissiers,
 Taux d’abandon (clients qui quittent avant d’être servis).
Le moteur permet aussi de tester plusieurs stratégies d’organisation :
nombre variable de caissiers,
priorités clients (VIP, files rapides),
horaires dynamiques ou fermetures automatiques,
règles d’ouverture de nouvelles caisses selon la charge.

+--------------------------------------------+
|         Simulation Engine (Core)           |
|--------------------------------------------|
|  - File d’attente (FIFO / Priorité)        |
|  - Gestion des caissiers (état, pauses)    |
|  - Planification (ouverture/fermeture)     |
|  - Traitement des événements (arrivals,    |
|    starts, ends, abandons)                 |
+--------------------------------------------+
 Composants principaux
1. Génération & ingestion d’événements
Génération d’événements d’arrivée clients selon une loi de probabilité (ex : Poisson).
Définition de temps de service selon une distribution (ex : exponentielle, normale).
Lecture possible depuis un fichier CSV / JSON pour rejouer des scénarios réels.
2. Moteur de simulation
Gestion des queues (FIFO, priorité, multi-files).
Simulation discrète dans le temps (événements successifs).
Attribution dynamique des clients aux caissiers disponibles.
Gestion du temps de pause et de l’état des caissiers (disponible / occupé / fermé).
3. Gestion des ressources
Représentation de chaque caisse avec son propre état et planning.
Règles de fermeture / ouverture planifiées.
Possibilité d’ajouter des règles dynamiques (ouvrir une caisse si > N clients en attente).
4. Collecte de métriques
Calcul automatique des principaux indicateurs :
Temps moyen d’attente
Temps total dans le système
Longueur moyenne des files
Taux d’occupation des caissiers
Taux d’abandon
Export des résultats vers CSV / JSON / dashboard interactif.
