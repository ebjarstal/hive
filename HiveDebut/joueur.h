#pragma once

#include "plateau.h"
#include "pions.h"
#include "mouvement.h"

#include <list>
#include <iostream>
#include <vector>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Joueur {
protected:
    friend class Partie;
    friend class PoserPionCommand;
    friend class DeplacerPionCommand;
    vector<Pion*> pionsEnMain;
    string couleur;
    Joueur(vector<Pion*> pEm, string c); // uniquement Partie peut le creer et avec param car extension
public:
    string getCouleur() { return couleur; }
    bool peutBougerPions();
    bool isMainVide();
    virtual bool estIA() const = 0;
    virtual Mouvement* Jouer(Plateau& plateau) = 0; // Methode virtuelle pure
    virtual ~Joueur(); // Destructeur virtuel pour eviter des fuites memoire
};

class JoueurHumain : public Joueur {
private:
    // M thode pour poser un pion en demandant les informations au joueur humain
    Mouvement* poserPionHumain(Plateau& plateau);

    // M thode pour d placer un pion du joueur sur le plateau
    Mouvement* deplacerPionHumain(Plateau& plateau);

    // M thode pour choisir un pion
    int choisirPion(const std::vector<Pion*>& pions);

    // M thode pour choisir un emplacement
    int choisirEmplacement(const std::list<Mouvement*>& emplacements);

    // M thode pour afficher les pions
    void afficherPions(const std::vector<Pion*>& pions);

    // M thode pour afficher les emplacements
    void afficherEmplacements(const std::list<Mouvement*>& emplacements);

    // M thode pour afficher les pions sur le plateau
    void afficherPionsSurPlateau(const std::vector<std::tuple<Pion*, int, int, int>>& pionsSurPlateau);

    // M thode pour choisir un pion sur le plateau
    int choisirPionSurPlateau(const std::vector<std::tuple<Pion*, int, int, int>>& pionsSurPlateau);

public:
    JoueurHumain(std::vector<Pion*> pionsEnMain, string couleur) : Joueur(pionsEnMain, couleur) {}
    Mouvement* Jouer(Plateau& plateau) override;
    bool estIA() const override { return false; }
    ~JoueurHumain() override = default;
};


class JoueurIA : public Joueur {
public:
    JoueurIA(std::vector<Pion*> pionsEnMain, string couleur) : Joueur(pionsEnMain, couleur) {}
    Mouvement* Jouer(Plateau& plateau) override {
        // Implementation specifique pour un joueur IA
        std::cout << "L intelligence artificielle joue son tour." << std::endl;
        return nullptr;
    }
    bool estIA() const override { return true; }
    ~JoueurIA() override = default;
};
