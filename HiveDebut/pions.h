#pragma once

#include <iostream>
#include <string>
#include <map>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Pion {
private:
    static std::map<int, Pion*> pions;
    string type;
    string couleur;

    static int prochainId;
    int id;

    int ligne = -1;
    int colonne = -1;
    int z = -1;  // dimension verticale dans le plateau

public:
    Pion(int id, string t, string c) : id(id), type(t), couleur(c) {}
    Pion(string t, string c) : id(prochainId++), type(t), couleur(c) {}

    static void ajouterPion(Pion* pion) { pions[pion->getId()] = pion; }
    static Pion* getPionById(int id) { return pions[id]; }
    static void resetPion(Pion* p, int id) { pions[id] = p; }

    void reset() { ligne = -1; colonne = -1; z = -1; }

    int getId() const { return id; }
    const string& getType() const { return type; }
    string getCouleur() const { return couleur; }
    int getLigne() const { return ligne; }
    void setLigne(int new_ligne) { ligne = new_ligne; }
    int getColonne() const { return colonne; }
    void setColonne(int new_colonne) { colonne = new_colonne; }
    int getZ() const { return z; }
    void setZ(int new_z) { z = new_z; }

};