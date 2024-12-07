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
} Clients;

typedef struct Noeud {
    Clients client;      
    struct Noeud *suivant;
} N_Noeud;

typedef struct ListeAttente {
    N_Noeud *premier;    
} ListeAttente;

#endif