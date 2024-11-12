#pragma once

#include <iostream>
#include <string>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Pion {
private:
    string type;
    string couleur;
    int ligne = 0;
    int colonne = 0;
    int z = 0;  // dimension verticale dans le plateau
public:
    Pion(string t, string c) : type(t), couleur(c) {}

    const string& getType() const { return type; }
    string getCouleur() const { return couleur; }
    int getLigne() const { return ligne; }
    void setLigne(int new_ligne) { ligne = new_ligne; }
    int getColonne() const { return colonne; }
    void setColonne(int new_colonne) { colonne = new_colonne; }
    int getZ() const { return z; }
    void setZ(int new_z) { z = new_z; }

};