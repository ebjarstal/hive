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

class GestionnairePions {
public:
    Pion* getPion(int ligne, int colonne, const Plateau& plateau, int z = 0) const;  // Recuperer les infos d'un pion par ses coordonnees
    std::vector<std::tuple<Pion*, int, int, int>> getPions(Plateau& plateau) const;  // Recuperer les infos de tous les pions du plateau

    void setPion(int ligne, int colonne, int z, Pion* p, Plateau& plateau);  // Poser un pion
    void deletePion(Pion& p, Plateau& plateau);  // Supprimer un pion
    void movePion(int ligne, int colonne, int z, Pion* p, Plateau& plateau);  // Deplacer un pion
    bool estPose(Pion& p) const;  // Booleen si le pion est sur le plateau

private:

    friend class Plateau;
};