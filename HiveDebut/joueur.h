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
    string nom;
    unsigned int nbUndo;
    Joueur(vector<Pion*> pEm, string c, Partie& p, string n, unsigned int nb); // uniquement Partie peut le creer et avec param car extension
public:
    string getCouleur() { return couleur; }
    string getNom() { return nom; }
    unsigned int getNbUndo() { return nbUndo; }
    bool canUndo();
    void setNbUndo(unsigned int nb) { nbUndo = nb; }
    vector<Pion*>& getPionsEnMain() { return pionsEnMain; }
    bool peutBougerPions();
    bool isMainVide();
    virtual bool estIA() const = 0;
    virtual void Jouer(Plateau& plateau, Partie& partie) = 0; // Methode virtuelle pure
    virtual ~Joueur(); // Destructeur virtuel pour eviter des fuites memoire
};

class JoueurHumain : public Joueur {
private:

    Pion* choisirPion(const std::vector<Pion*>& pions);  // Le joueur choisit le pion parmi pions
    Pion* choisirPionEnMain();  // Le joueur choisit le pion parmi sa main
    Mouvement* choisirEmplacement(const std::vector<Mouvement*>& emplacements);  // Le joueur choisit un emplacement parmi emplacements
    Pion* choisirPionSurPlateau(Plateau& plateau, std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau); // Le joueur choisit un pion sur le plateau

    void afficherPions(const std::vector<Pion*>& pions); // Afficher les pions
    void afficherPionsEnMain(); // Afficher les pions en main
    void afficherEmplacements(const std::vector<Mouvement*>& emplacements); // Afficher les emplacements
    void afficherPionsSurPlateau(Plateau& plateau, std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau); // Afficher les pions sur le plateau

    Mouvement* poserPionHumain(Plateau& plateau, Partie& partie);  // Poser un pion
    Mouvement* deplacerPionHumain(Plateau& plateau);  // Deplacer un pion
    Mouvement* poserReineHumain(Plateau& plateau); //Poser le pion Reine si ne l'a pas fait après 4 tours
    void JouerMouvement(Partie& partie, Mouvement* emplacementChoisi);

public:
    JoueurHumain(string n, std::vector<Pion*> pionsEnMain, string couleur, unsigned int nb, Partie& p) : Joueur(pionsEnMain, couleur, p, n, nb) {}
    void Jouer(Plateau& plateau, Partie& partie) override;
    bool estIA() const override { return false; }
    ~JoueurHumain() override = default;
};


class JoueurIA : public Joueur {
public:
    JoueurIA(string n, std::vector<Pion*> pionsEnMain, string couleur, Partie& p, unsigned int nb) : Joueur(pionsEnMain, couleur, p, n, nb) {}
    void Jouer(Plateau& plateau, Partie& partie) override;

    Mouvement* trouverMeilleurMouvement(Plateau& plateau, Joueur& joueurCourant, int profondeurMax);
    int evaluerPartie(Plateau& plateau, Joueur& j, bool isMaximizingPlayer);
    int minimax(Plateau& plateau, int profondeur, Joueur& joueurCourant, bool isMaximizingPlayer, int alpha, int beta);

    bool estIA() const override { return true; }
    ~JoueurIA() override = default;
};
