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
#include <direct.h>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Plateau {
public:
    Plateau(unsigned int l, unsigned int c, unsigned int z, GestionnairePions& gP, GestionnaireMouvements& gM, GestionnaireVoisins& gV);
    ~Plateau();

    void afficher() const;
    bool isVide() const;
    void redimensionnerPlateau();
    bool estValide(int ligne, int colonne, int z = 0) const;  // Méthode non statique car elle dépend des dimensions du Plateau
    
    GestionnairePions& getGestionnairePions() const { return gestionnairePions; }
    GestionnaireVoisins& getGestionnaireVoisins() const { return gestionnaireVoisins; }
    GestionnaireMouvements& getGestionnaireMouvements() const { return gestionnaireMouvements; }

private:
    unsigned int nb_lignes, nb_colonnes, nb_couches;
    std::vector<std::vector<std::vector<Pion*>>> grille;
    GestionnairePions& gestionnairePions;
    GestionnaireVoisins& gestionnaireVoisins;
    GestionnaireMouvements& gestionnaireMouvements;

    // Accès aux attributs privés pour Partie, GestionnairePions, GestionnaireVoisins, GestionnaireMouvements, JoueurHumain
    friend class Partie;
    friend class GestionnairePions;
    friend class GestionnaireVoisins;
    friend class GestionnaireMouvements;
    friend class JoueurHumain;
    friend class JoueurIA;
};