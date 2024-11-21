#pragma once

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
    virtual void Jouer(Plateau& plateau, GestionnaireCommand& gC) = 0; // Methode virtuelle pure
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

    Mouvement* poserPionHumain(Plateau& plateau, GestionnaireCommand& gC);  // Poser un pion
    Mouvement* deplacerPionHumain(Plateau& plateau, GestionnaireCommand& gC);  // Deplacer un pion

public:
    JoueurHumain(std::vector<Pion*> pionsEnMain, string couleur) : Joueur(pionsEnMain, couleur) {}
    void Jouer(Plateau& plateau, GestionnaireCommand& gC) override;
    bool estIA() const override { return false; }
    ~JoueurHumain() override = default;
};


class JoueurIA : public Joueur {
public:
    JoueurIA(std::vector<Pion*> pionsEnMain, string couleur) : Joueur(pionsEnMain, couleur) {}
    void Jouer(Plateau& plateau, GestionnaireCommand& gC) override {
        // Implementation specifique pour un joueur IA
        std::cout << "L intelligence artificielle joue son tour." << std::endl;
        return;
    }
    bool estIA() const override { return true; }
    ~JoueurIA() override = default;
};
