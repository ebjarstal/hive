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

class Partie;

class Joueur {
protected:
    friend class Partie;
    Partie* partie;
    vector<Pion*> pionsEnMain;
    string couleur;
    Joueur(vector<Pion*> pEm, string c, Partie& p); // uniquement Partie peut le creer et avec param car extension
public:
    string getCouleur() { return couleur; }
    vector<Pion*>& getPionsEnMain() { return pionsEnMain; }
    bool peutBougerPions();
    bool isMainVide();
    std::vector<Pion*> getPionsEnJeu(Plateau& plateau);
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
    JoueurHumain(std::vector<Pion*> pionsEnMain, string couleur, Partie& p) : Joueur(pionsEnMain, couleur, p) {}
    Mouvement* Jouer(Plateau& plateau) override;
    bool estIA() const override { return false; }
    ~JoueurHumain() override = default;
};


class JoueurIA : public Joueur {
public:
    JoueurIA(std::vector<Pion*> pionsEnMain, string couleur, Partie& p) : Joueur(pionsEnMain, couleur, p) {}
    Mouvement* Jouer(Plateau& plateau) override {
        // Implementation specifique pour un joueur IA
        std::cout << "L intelligence artificielle joue son tour." << std::endl;
        return nullptr;
    }

    Mouvement* trouverMeilleurMouvement(Plateau& plateau, Joueur& joueurCourant, int profondeurMax);
    int calculerBlocageAbeille(Plateau& plateau, Joueur& joueur, bool isMaximizingPlayer);
    int evaluerPartie(Plateau& plateau, Joueur& j, bool isMaximizingPlayer);
    int calculerScoreBlocage(Plateau& plateau, Joueur& joueur, bool isMaximizingPlayer);
    int minimax(Plateau& plateau, int profondeur, Joueur& joueurCourant, bool isMaximizingPlayer, int alpha, int beta);

    bool estIA() const override { return true; }
    ~JoueurIA() override = default;
};
