Simuler le comportement d’un système de caisse : arrivée des clients, files d’attente, ouverture/fermeture des caisses, temps de service, départs.
Evaluer KPI : temps d’attente moyen, temps total dans le système.
Tester stratégies (nombre variable de caissiers, priorités, horaires dynamiques).
Architecture (vue d’ensemble)
Génération / Ingestion d’événements : arrival events (horaire client), paramètres de service (distribution).
Moteur de simulation : logique de queue (FIFO, priorité).
Gestion des ressources : état des caissiers (disponible/occupé/break), ouverture/fermeture planifiée.
Collecte de métriques : latences, files, taux d’utilisation, abandons.
Orchestration : script CLI pour lancer des expériences et comparer scénarios.
