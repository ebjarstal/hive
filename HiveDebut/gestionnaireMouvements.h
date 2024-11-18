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

class GestionnaireMouvements {
public:
    bool cassageRuche(Pion& p, Plateau& plateau); // s'occupe du cassage de ruche
    std::vector<std::tuple<Pion*, int, int, int>> getPionsBougeables(Plateau& plateau);
    list<Mouvement*> emplacementsPossibles(const Pion& p, Plateau& plateau); // quand pose le pion sur le plateau

    // M thode pour filtrer les deplacements valides
    std::list<Mouvement*> filtrerDeplacementsValides(const std::list<Mouvement*>& emplacements, Pion* pion, Plateau& plateau);
    // M thode pour v rifier si le deplacement casse la ruche
    bool deplacementCasseRuche(Pion* pion, int newLigne, int newColonne, int newZ, Plateau& plateau);


private:
    friend class Plateau;
};

