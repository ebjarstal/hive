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

class GestionnairePions {
public:
    static Pion* getPion(int ligne, int colonne, const Plateau& plateau, int z = 0);  // Recuperer les infos d'un pion par ses coordonnees
    static std::vector<std::tuple<Pion*, int, int, int>> getPions(Plateau& plateau);  // Recuperer les infos de tous les pions du plateau
    static std::vector<Pion*> getPionsEnJeu(Plateau& plateau, string couleur);

    static void setPion(int ligne, int colonne, int z, Pion* p, Plateau& plateau);  // Poser un pion
    static void deletePion(Pion& p, Plateau& plateau);  // Supprimer un pion
    static void movePion(int ligne, int colonne, int z, Pion* p, Plateau& plateau);  // Deplacer un pion
    static bool estPose(Pion& p);  // Booleen si le pion est sur le plateau

private:

    friend class Plateau;
};