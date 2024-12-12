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

// Déclaration de Plateau au préalable pour éviter les problèmes de compilation
class Plateau;

class GestionnaireVoisins {
public:
    static std::vector<Pion*> getVoisins(Pion& p, Plateau& plateau);  // Recuperer les voisins d'un pion avec une reference Pion
    static std::vector<Pion*> getVoisins(int ligne, int colonne, Plateau& plateau, int z = 0);  // Recuperer les voisins d'un pion avec ses coordonnees
    static std::vector<std::tuple<int, int, int>> getVoisinsCoords(int ligne, int colonne, Plateau& plateau, int z = 0);  // Recuperer les coordonnees des voisins d'un pion avec ses coordonnees

    static std::vector<std::tuple<int, int, int>> getCasesVidesAutour(Pion& p, Plateau& plateau); // Recuperer les cases vides autour d'un pion avec une référence Pion
    static std::vector<std::tuple<int, int, int>> getCasesVidesAutour(int ligne, int colonne, int z, Plateau& plateau); // Recuperer les cases vides autour d'un pion avec ses coordonnées
    static std::vector<std::tuple<int, int, int>> getCasesVidesCommunes(Pion& p, Plateau& plateau); //Recuperer les cases vides communes à un pion et ses voisins directs

    static int nombreVoisins(Pion& p, Plateau& plateau);  // Recuperer le nombre de voisin d'un pion
    static bool hasVoisin(Pion& p, Plateau& plateau);  // Booleen si le pion a des voisins
    static bool sontVoisin(Pion& p1, Pion& p2, Plateau& plateau);  // booleen si les pions sont voisins

    static std::vector<Pion*> getRuche(Pion* p, Plateau& plateau);  // Recuperer l'entierete de la ruche du pion p
};

