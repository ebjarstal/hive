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
    UsineDePions usine;
    std::stack<Command*> historique;
    Joueur* joueur1;
    Joueur* joueur2;
    unsigned int nombreTour;
public:

    string nomPartie;
    Partie(Plateau& p) : joueur1(nullptr), joueur2(nullptr), nombreTour(1), plateau(p), usine() {}
    ~Partie();
    Plateau& getPlateau() { return plateau; }
    unsigned int getNombreTour() const { return nombreTour; }
    Joueur* getJoueur1() const { return joueur1; }
    Joueur* getJoueur2() const { return joueur2; }
    std::stack<Command*>& getHistorique() { return historique; }

    void setPlateau(Plateau& p) { plateau = p; }
    void setNombreTour(unsigned int nb) { nombreTour = nb; }
    void setJoueur1(Joueur* j) { joueur1 = j; }
    void setJoueur2(Joueur* j) { joueur2 = j; }
    void setHistorique(std::stack<Command*>& h) { historique = h; }

    Joueur& joueurAdverse(Joueur& joueur) { return (&joueur == joueur1) ? *joueur2 : *joueur1; }

    void setup(); // Met en place une partie

    int choixChargementOuCreationPartie(); // Donne le choix au joueur de charger ou créer une partie
    void creationPartie(const std::string dossierSauvegarde);  // Cree une partie
    void choixExtension();
    std::vector<Pion*> initialiserPions(const std::string& couleur);  // Initialise les pions des 2 joueurs

    void jouerUnTour(Joueur* j);

    bool partieTerminee() const;
    Joueur* determinerGagnant() const;
    void annulerMouvement(Joueur& j);
};