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
    bool peutBouger(Pion& p, Plateau& plateau); // s'occupe du cassage de ruche
    list<Mouvement*> emplacementsPossibles(const Pion& p, Plateau& plateau); // quand pose le pion sur le plateau

private:
    friend class Plateau;
};

