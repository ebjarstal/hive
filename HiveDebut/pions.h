#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include "mouvement.h"

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Plateau;
class Joueur;

class Pion {
protected:
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

    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) = 0;
    virtual std::vector<Mouvement*> deplacementsPossibles(Joueur& j, Plateau& plateau);
    virtual std::vector<Mouvement*> emplacementsPossibles(Pion& p, Plateau& plateau);
};

class Reine : public Pion {
public:
    Reine(int id, string c) : Pion(id, "R", c) {}
    Reine(string c) : Pion("R", c) {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Sauterelle : public Pion {
public:
    Sauterelle(int id, string c) : Pion(id, "S", c) {}
    Sauterelle(string c) : Pion("S", c) {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Araignee : public Pion {
public:
    Araignee(int id, string c) : Pion(id, "A", c) {}
    Araignee(string c) : Pion("A", c) {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};


class Fourmi : public Pion {
public:
    Fourmi(int id, string c) : Pion(id, "F", c) {}
    Fourmi(string c) : Pion("F", c) {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Scarabee : public Pion {
public:
    Scarabee(int id, string c) : Pion(id, "K", c) {}
    Scarabee(string c) : Pion("K", c) {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Coccinelle : public Pion {
public:
    Coccinelle(int id, string c) : Pion(id, "X", c) {}
    Coccinelle(string c) : Pion("X", c) {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Cloporte : public Pion {
public:
    Cloporte(int id, string c) : Pion(id, "C", c) {}
    Cloporte(string c) : Pion("C", c) {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Moustique : public Pion {
public:
    Moustique(int id, string c) : Pion(id, "M", c) {}
    Moustique(string c) : Pion("M", c) {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};
