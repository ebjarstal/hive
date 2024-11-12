#pragma once

#include <iostream>
#include <tuple>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Mouvement {
private:
    tuple<int, int, int> coordDest;
    tuple<int, int, int> coordSrc;
public:
    Mouvement(int ligne, int colonne, int z, int oldLigne, int oldColonne, int oldZ)
        : coordDest(make_tuple(ligne, colonne, z)), coordSrc(make_tuple(oldLigne, oldColonne, oldZ)) {}

    int getLigne() const { return get<0>(coordDest); }
    int getColonne() const { return get<1>(coordDest); }
    int getZ() const { return get<2>(coordDest); }
    int getOldLigne() const { return get<0>(coordSrc); }
    int getOldColonne() const { return get<1>(coordSrc); }
    int getOldZ() const { return get<2>(coordSrc); }
};
