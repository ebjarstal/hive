#pragma once

#include "plateau.h"
#include "joueur.h"
#include "pions.h"
#include "mouvement.h"
#include "usine.h"
#include "gestionnaireCommand.h"

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
    GestionnaireCommand& gC;
    Joueur* joueur1;
    Joueur* joueur2;
    unsigned int nombreTour;
    unsigned int nbUndo;

    friend class JoueurIA;
public:

    string nomPartie;

    Partie(Plateau& p, GestionnaireCommand& gC) : joueur1(nullptr), joueur2(nullptr), nombreTour(1), nbUndo(0), plateau(p), gC(gC) {}
    ~Partie();
    Plateau& getPlateau() { return plateau; }
    unsigned int getNombreTour() const { return nombreTour; }
    Joueur* getJoueur1() const { return joueur1; }
    Joueur* getJoueur2() const { return joueur2; }
    unsigned int getNbUndo() const { return nbUndo; }
    Joueur& joueurAdverse(Joueur& joueur) { return (&joueur == joueur1) ? *joueur2 : *joueur1; }

    bool canUndo() { return !gC.historique.empty() && gC.historique.size() >= 2 && getNbUndo() >= 1; }

    void setup(); // Met en place une partie
    static void creerDossierSiInexistant(const std::string& cheminDossier);

    int choixChargementOuCreationPartie(); // Donne le choix au joueur de charger ou créer une partie
    bool chargementSauvegardePartie(const std::string dossierSauvegarde);  // Charge une partie depuis une liste de fichier de sauvegarde
    bool chargerPartie();  // Charge une partie à partir du fichier texte de sauvegarde
    void creationPartie(const std::string dossierSauvegarde);  // Cree une partie
    std::vector<Pion*> initialiserPions(const std::string& couleur);  // Initialise les pions des 2 joueurs

    void jouerUnTour(Joueur* j);

    bool partieTerminee() const;
    Joueur* determinerGagnant() const;
    void sauvegarde();

    void annulerMouvement();

    std::vector<std::string> listerSauvegardes();  // Liste l'entierete des sauvegardes de liste_sauvegardes.txt
};