#pragma once

#include "pions.h"

#include <iostream>
#include <vector>
#include <tuple>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

// D�claration de Plateau au pr�alable pour �viter les probl�mes de compilation
class Plateau;

class GestionnaireVoisins {
public:
    std::vector<Pion*> getVoisins(Pion& p, Plateau& plateau) const;  // Recuperer les voisins d'un pion avec une reference Pion
    std::vector<Pion*> getVoisins(int ligne, int colonne, Plateau& plateau, int z = 0) const;  // Recuperer les voisins d'un pion avec ses coordonnees
    std::vector<std::tuple<int, int, int>> getVoisinsCoords(int ligne, int colonne, Plateau& plateau, int z = 0) const;  // Recuperer les coordonnees des voisins d'un pion avec ses coordonnees
    std::vector<std::tuple<int, int, int>> getCasesVidesAutour(Pion& p,Plateau& plateau); // Recuperer les cases vides autour d'un pion avec une r�f�rence Pion
    std::vector<std::tuple<int, int, int>> getCasesVidesAutour(int ligne, int colonne, int z, Plateau& plateau); // Recuperer les cases vides autour d'un pion avec ses coordonn�es

    int nombreVoisins(Pion& p, Plateau& plateau) const;  // Recuperer le nombre de voisin d'un pion
    bool hasVoisin(Pion& p, Plateau& plateau) const;  // Booleen si le pion a des voisins
    bool sontVoisin(Pion& p1, Pion& p2, Plateau& plateau) const;  // booleen si les pions sont voisins

    std::vector<Pion*> getRuche(Pion* p, Plateau& plateau) const;  // Recuperer l'entierete de la ruche du pion p
private:

    friend class Plateau;
};

