#pragma once

#include "pions.h"
#include "gestionnairePions.h"
#include "gestionnaireMouvements.h"
#include "gestionnaireVoisins.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <stack>
#include <vector>
#include <tuple>
#include <deque>
#include <fstream>
#include <sstream>
#include <set>
#include <filesystem>
#include <algorithm>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Plateau {
public:
    Plateau(unsigned int l, unsigned int c, unsigned int z);
    ~Plateau();

    void afficher();
    bool isVide() const;
    void redimensionnerPlateau();
    bool estValide(int ligne, int colonne, int z = 0) const;  // M�thode non statique car elle d�pend des dimensions du Plateau

    unsigned int getNbLignes() const { return nb_lignes; }
    unsigned int getNbColonnes() const { return nb_colonnes; }
    unsigned int getNbCouches() const { return nb_couches; }
    std::vector<std::vector<std::vector<Pion*>>>& getGrille() { return grille; }

private:
    unsigned int nb_lignes, nb_colonnes, nb_couches;
    std::vector<std::vector<std::vector<Pion*>>> grille;

};