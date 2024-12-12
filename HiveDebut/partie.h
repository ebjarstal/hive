#pragma once

#include "plateau.h"
#include "joueur.h"
#include "pions.h"
#include "mouvement.h"
#include "usine.h"
#include "gestionnaireCommand.h"
#include "gestionnaireSauvegarde.h"

#include <iostream>
#include <stack>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <map>
#include <algorithm>
#include <set>

#define NOMINMAX

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Partie {
private:
    Plateau& plateau;
    std::stack<Command*> historique;
    Joueur* joueur1;
    Joueur* joueur2;
    unsigned int nombreTour;
    unsigned int nbUndo;

public:

    string nomPartie;

    Partie(Plateau& p) : joueur1(nullptr), joueur2(nullptr), nombreTour(1), nbUndo(0), plateau(p) {}
    ~Partie();
    Plateau& getPlateau() { return plateau; }
    unsigned int getNombreTour() const { return nombreTour; }
    Joueur* getJoueur1() const { return joueur1; }
    Joueur* getJoueur2() const { return joueur2; }
    unsigned int getNbUndo() const { return nbUndo; }
    std::stack<Command*>& getHistorique() { return historique; }

    void setNomPartie(const std::string& nom) { nomPartie = nom; }
    void setPlateau(Plateau& p) { plateau = p; }
    void setNombreTour(unsigned int nb) { nombreTour = nb; }
    void setJoueur1(Joueur* j) { joueur1 = j; }
    void setJoueur2(Joueur* j) { joueur2 = j; }
    void setNbUndo(unsigned int nb) { nbUndo = nb; }
    void setHistorique(std::stack<Command*>& h) { historique = h; }

    Joueur& joueurAdverse(Joueur& joueur) { return (&joueur == joueur1) ? *joueur2 : *joueur1; }

    bool canUndo() { return !historique.empty() && historique.size() >= 2 && getNbUndo() >= 1; }

    void setup(); // Met en place une partie

    int choixChargementOuCreationPartie(); // Donne le choix au joueur de charger ou créer une partie
    void creationPartie(const std::string dossierSauvegarde);  // Cree une partie
    std::vector<Pion*> initialiserPions(const std::string& couleur);  // Initialise les pions des 2 joueurs

    void jouerUnTour(Joueur* j);

    bool partieTerminee() const;
    Joueur* determinerGagnant() const;
    void annulerMouvement();
};