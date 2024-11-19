#pragma once

#include "plateau.h"
#include "joueur.h"
#include "pions.h"
#include "mouvement.h"

#include <iostream>
#include <stack>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <map>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

using namespace std;

class Partie {
private:
    Plateau& plateau;
    Joueur* joueur1;
    Joueur* joueur2;
    unsigned int nombreTour;
    std::stack<Mouvement*> historique;
    unsigned int nbUndo;

    //std::vector<Extension> extensions;
    //void ajouterExtension(unsigned int id);

public:

    Partie(Plateau& p) : joueur1(nullptr), joueur2(nullptr), nombreTour(1), nbUndo(0), plateau(p) {}
    ~Partie();
    string nomPartie;

    std::vector<std::string> listerSauvegardes();
    Plateau& getPlateau() { return plateau; }
    void setup();
    std::vector<Pion*> initialiserPions(const std::string& couleur);
    bool chargerPartie();
    bool partieTerminee() const;
    Joueur* determinerGagnant() const;
    void jouerUnTour(Joueur* j);
    void sauvegarde();
    unsigned int getNombreTour() const { return nombreTour; }
    Joueur* getJoueur1() const { return joueur1; }
    Joueur* getJoueur2() const { return joueur2; }
    unsigned int getNbUndo() const { return nbUndo; }
    void annulerMouvement();
    void ajouterMouvement(Mouvement* m) { if (m != nullptr) historique.push(m); }
};