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
    bool cassageRuche(Pion& p, Plateau& plateau); // s'occupe du cassage de ruche
    std::vector<std::tuple<Pion*, int, int, int>> getPionsBougeables(Plateau& plateau, Joueur& j);
    list<Mouvement*> emplacementsPossibles(Pion& p, Plateau& plateau); // quand pose le pion sur le plateau
    std::list<Mouvement*> deplacementPossibles(Pion& p, Plateau& plateau);
    std::list<Mouvement*> deplacementPossiblesArraigne(Pion& p, Plateau& plateau);
    // M thode pour filtrer les deplacements valides
    std::vector<Mouvement*> genererTousLesMouvements(Plateau& p, Joueur& joueur);
    // M thode pour v rifier si le deplacement casse la ruche
    bool deplacementCasseRuche(Pion* pion, int newLigne, int newColonne, int newZ, Plateau& plateau);


private:
    friend class Plateau;
    friend class Joueur;
};

