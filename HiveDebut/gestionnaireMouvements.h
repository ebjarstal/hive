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
    static bool cassageRuche(Pion& p, Plateau& plateau); // s'occupe du cassage de ruche
    static std::vector<std::tuple<Pion*, int, int, int>> getPionsBougeables(Plateau& plateau);
    static std::vector<Mouvement*> emplacementsPossibles(Pion& p, Plateau& plateau); // quand pose le pion sur le plateau
    static std::vector<Mouvement*> filtrerDeplacementsValides(const std::vector<Mouvement*>& emplacements, Pion* pion, Plateau& plateau);     // filtrer les deplacements valides
    static bool deplacementCasseRuche(Pion* pion, int newLigne, int newColonne, int newZ, Plateau& plateau);     // Verifier si le deplacement casse la ruche

private:
    friend class Plateau;
};

