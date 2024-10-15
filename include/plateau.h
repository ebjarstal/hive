#ifndef PLATEAU_H
#define PLATEAU_H

#include <iostream>
#include <string>
#include <vector>

class Pion {
private:
    std::string nom;
public:
    Pion(std::string n) : nom(n) {}
    inline std::string getNom() const { return nom; }
};

class Plateau {
public:
    Plateau(int lignes, int colonnes);
    ~Plateau();
    Pion* getPion(int ligne, int colonne) const;
    void setPion(int ligne, int colonne, Pion* pion);
    void deletePion(int ligne, int colonne);
    void afficher() const;
    std::vector<Pion*> getVoisins(int ligne, int colonne) const;
    bool isVide() const;
    bool hasVoisin(int ligne, int colonne) const;

private:
    int lignes, colonnes;
    std::vector<std::vector<Pion*>> grille;
    bool estValide(int ligne, int colonne) const;
};

#endif
