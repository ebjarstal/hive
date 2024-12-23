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

class Plateau;
class Joueur;

class Pion {
protected:
    static std::map<int, Pion*> pions;
    std::string type;
    std::string couleur;
    std::string nom;

    static int prochainId;
    int id;

    int ligne = -1;
    int colonne = -1;
    int z = -1;  // dimension verticale dans le plateau

public:
    virtual ~Pion() { auto it = pions.find(id); if (it != pions.end()) pions.erase(it); }
    Pion(int id, std::string t, std::string c, std::string n) : id(id), type(t), couleur(c), nom(n) {}
    Pion(std::string t, std::string c, std::string n) : id(prochainId++), type(t), couleur(c), nom(n) {}

    static void ajouterPion(Pion* pion) { pions[pion->getId()] = pion; }
    static Pion* getPionById(int id) { return pions[id]; }
    static void resetPion(Pion* p, int id) { pions[id] = p; }

    void reset() { ligne = -1; colonne = -1; z = -1; }

    int getId() const { return id; }
    const std::string& getType() const { return type; }
    std::string getCouleur() const { return couleur; }
    const std::string getNom() const { return nom; }
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
    Reine(int id, std::string c) : Pion(id, "R", c, "Reine") {}
    Reine(std::string c) : Pion("R", c, "Reine") {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Sauterelle : public Pion {
public:
    Sauterelle(int id, std::string c) : Pion(id, "S", c, "Sauterelle") {}
    Sauterelle(std::string c) : Pion("S", c, "Sauterelle") {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Araignee : public Pion {
public:
    Araignee(int id, std::string c) : Pion(id, "A", c, "Araignee") {}
    Araignee(std::string c) : Pion("A", c, "Araignee") {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};


class Fourmi : public Pion {
public:
    Fourmi(int id, std::string c) : Pion(id, "F", c, "Fourmi") {}
    Fourmi(std::string c) : Pion("F", c, "Fourmi") {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Scarabee : public Pion {
public:
    Scarabee(int id, std::string c) : Pion(id, "K", c, "Scarabee") {}
    Scarabee(std::string c) : Pion("K", c, "Scarabee") {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Coccinelle : public Pion {
public:
    Coccinelle(int id, std::string c) : Pion(id, "X", c, "Coccinelle") {}
    Coccinelle(std::string c) : Pion("X", c, "Coccinelle") {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Cloporte : public Pion {
public:
    Cloporte(int id, std::string c) : Pion(id, "C", c, "Cloporte") {}
    Cloporte(std::string c) : Pion("C", c, "Cloporte") {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};

class Moustique : public Pion {
public:
    Moustique(int id, std::string c) : Pion(id, "M", c, "Moustique") {}
    Moustique(std::string c) : Pion("M", c, "Moustique") {}
    virtual std::vector<Mouvement*> deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) override;
};
