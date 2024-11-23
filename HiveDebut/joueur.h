#pragma once

#define NOMINMAX

#include "plateau.h"
#include "pions.h"
#include "mouvement.h"
#include "gestionnaireCommand.h"

#include <list>
#include <iostream>
#include <vector>
#include <utility>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Partie;

class Joueur {
protected:
    Partie& partie;
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
    virtual void Jouer(Plateau& plateau) = 0; // Methode virtuelle pure
    virtual ~Joueur(); // Destructeur virtuel pour eviter des fuites memoire
};

class JoueurHumain : public Joueur {
private:

    Pion* choisirPion(const std::vector<Pion*>& pions);  // Le joueur choisit le pion parmi pions
    Pion* choisirPionEnMain();  // Le joueur choisit le pion parmi sa main
    Mouvement* choisirEmplacement(const std::vector<Mouvement*>& emplacements);  // Le joueur choisit un emplacement parmi emplacements
    Pion* choisirPionSurPlateau(Plateau& plateau); // Le joueur choisit un pion sur le plateau

    void afficherPions(const std::vector<Pion*>& pions); // Afficher les pions
    void afficherPionsEnMain(); // Afficher les pions en main
    void afficherEmplacements(const std::vector<Mouvement*>& emplacements); // Afficher les emplacements
    void afficherPionsSurPlateau(Plateau& plateau); // Afficher les pions sur le plateau

    Mouvement* poserPionHumain(Plateau& plateau);  // Poser un pion
    Mouvement* deplacerPionHumain(Plateau& plateau);  // Deplacer un pion

public:
    JoueurHumain(std::vector<Pion*> pionsEnMain, string couleur, Partie& p) : Joueur(pionsEnMain, couleur, p) {}
    void Jouer(Plateau& plateau) override;
    bool estIA() const override { return false; }
    ~JoueurHumain() override = default;
};


class JoueurIA : public Joueur {
public:
    JoueurIA(std::vector<Pion*> pionsEnMain, string couleur, Partie& p) : Joueur(pionsEnMain, couleur, p) {}
    void Jouer(Plateau& plateau) override {
        // Implementation specifique pour un joueur IA
        std::cout << "L intelligence artificielle joue son tour." << std::endl;
        return;
    }

    Mouvement* trouverMeilleurMouvement(Plateau& plateau, Joueur& joueurCourant, int profondeurMax);
    int calculerBlocageAbeille(Plateau& plateau, Joueur& joueur, bool isMaximizingPlayer);
    int evaluerPartie(Plateau& plateau, Joueur& j, bool isMaximizingPlayer);
    int calculerScoreBlocage(Plateau& plateau, Joueur& joueur, bool isMaximizingPlayer);
    int minimax(Plateau& plateau, int profondeur, Joueur& joueurCourant, bool isMaximizingPlayer, int alpha, int beta);

    bool estIA() const override { return true; }
    ~JoueurIA() override = default;
};
