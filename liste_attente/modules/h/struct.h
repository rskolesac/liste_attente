#ifndef STRUCT_H  
#define STRUCT_H   

typedef struct {
   char debut_service;
   char fin_service;
} Service;

typedef struct Heure {
    int heure;
    int minute;
    struct Heure *suivant;
} Heure;

typedef struct Temps {
    Heure h;             
    Heure durée; 
} Temps;

typedef struct Clients {
    int indice;
    Temps t; 
    Heure départ;     
    int estPrisEnCharge;
    int clientrefuse;
} Clients;

typedef struct Noeud {
    Clients client;      
    struct Noeud *suivant;
} N_Noeud;

typedef struct ListeAttente {
    N_Noeud *premier; 
    int taille;
} ListeAttente;


typedef struct Stats {
    int tailleMin; 
    int tailleMax;   
    int sommeTaille;  
    int nbMesures;    


    int tempsAttenteMin;  
    int tempsAttenteMax;  
    int sommeTempsAttente;  
    int nbClientsAvecAttente; 


    int totalAppelsPrisEnCharge; 
    int tempsTotal;         

    int clientsPrisEnCharge;     
    int clientsNonPrisEnCharge;  
} Stats;


#endif