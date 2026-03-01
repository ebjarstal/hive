#pragma once

#include "plateau.h"
#include "joueur.h"
#include "pions.h"
#include "mouvement.h"
#include "usine.h"
#include "gestionnaireCommand.h"
#include "gestionnaireSauvegarde.h"

#include <iostream>
#include <memory>
#include <stack>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <set>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Partie {
private:
    Plateau& plateau;
    std::unique_ptr<UsineDePions> usine;
    std::stack<std::unique_ptr<Command>> historique;
    std::unique_ptr<Joueur> joueur1;
    std::unique_ptr<Joueur> joueur2;
    std::vector<std::unique_ptr<Pion>> tousLesPions;
    unsigned int nombreTour;
public:

    string nomPartie;
    Partie(Plateau& p, UsineDePions* u) : joueur1(nullptr), joueur2(nullptr), nombreTour(1), plateau(p), usine(u) {}
    ~Partie() = default;
    Plateau& getPlateau() { return plateau; }
    unsigned int getNombreTour() const { return nombreTour; }
    Joueur* getJoueur1() const { return joueur1.get(); }
    Joueur* getJoueur2() const { return joueur2.get(); }
    std::stack<std::unique_ptr<Command>>& getHistorique() { return historique; }

    void setNomPartie(const std::string& nom) { nomPartie = nom; }
    void setPlateau(Plateau& p) { plateau = p; }
    void setNombreTour(unsigned int nb) { nombreTour = nb; }
    void setJoueur1(Joueur* j) { joueur1.reset(j); }
    void setJoueur2(Joueur* j) { joueur2.reset(j); }
    void setHistorique(std::stack<std::unique_ptr<Command>> h) { historique = std::move(h); }

    // Single owner of all Pion objects
    void addPion(std::unique_ptr<Pion> p) { tousLesPions.push_back(std::move(p)); }

    Joueur& joueurAdverse(Joueur& joueur) { return (&joueur == joueur1.get()) ? *joueur2 : *joueur1; }

    void setup(); // Met en place une partie

    int choixChargementOuCreationPartie(); // Donne le choix au joueur de charger ou créer une partie
    void creationPartie(const std::string dossierSauvegarde);  // Cree une partie
    void choixExtension();
    std::vector<Pion*> initialiserPions(const std::string& couleur);  // Initialise les pions des 2 joueurs
    UsineDePions* getUsine() const { return usine.get(); }

    void jouerUnTour(Joueur* j);

    bool partieTerminee() const;
    Joueur* determinerGagnant() const;
    void annulerMouvement(Joueur& j);
};
