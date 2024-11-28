#pragma once

#include "mouvement.h"
#include "pions.h"

#include <iostream>
#include <list>
#include <vector>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

// Déclaration de Plateau au préalable pour éviter les problèmes de compilation
class Plateau;
class Joueur;

class GestionnaireMouvements {
public:
    static bool cassageRuche(Pion& p, Plateau& plateau); // s'occupe du cassage de ruche
    static std::vector<std::tuple<Pion*, int, int, int>> getPionsBougeables(Plateau& plateau, Joueur& joueur);
    static std::vector<Mouvement*> deplacementsPossibles(Pion& p, Plateau& plateau); // deplacement
    
    static std::vector<Mouvement*> genererTousLesMouvements(Plateau& p, Joueur& joueur);
    static bool deplacementCasseRuche(Pion* pion, int newLigne, int newColonne, int newZ, Plateau& plateau);     // Verifier si le deplacement casse la ruche
};

