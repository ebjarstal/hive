#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <vector>
#include "pions.h"
#include "tableau.h"
using namespace std;

class Pion{
private:
    Pion* pieceAuDessus;
    Pion* pieceEnDessous;
    Joueur& proprietaire;
    int x;
    int y;
    string type;
public:
    bool peutBouger(); //s'occupe des règles de base (ex: casser ruche ou espace suffisant pour passer)
    list<Mouvement*> emplacementsPossibles(); //quand pose le pion sur le plateau
    list<Mouvement*> deplacementsPossibles(); //quand joue le pion déjà posé sur le plateau
    bool estVoisin(const Pion& p) const; //renvoie TRUE si pion passer paramètre est voisin de this
    list<Pion> getVoisins() const;//liste des voisins du pion
    bool estPose() const;//TRUE si pion sur le plateau
    string getType() const{return type;}
    Joueur& getProprietaire() const{return proprietaire;}
    pair<int,int> getCoordonnees() const{return make_pair(x,y);}
    Pion* getPieceDessus() const {return pieceAuDessus;}
    Pion* getPieceDessous() const {return pieceEnDessous;}
};

class Mouvement{
public:
    Pion& pion;
    pair<int,int> coordDest;
    pair<int,int> coordSrc;
};

class Joueur{
private:
    bool couleur;
    void poserPion();
    void deplacerPion();
public:
    list<Pion> pionsEnMain;
    bool peutBougerPions; //en fonction de si la reine est posée
    void jouer();
    bool getCouleur() const {return couleur;}
    void addPion();
};

class Extension{
private:
    string nom;
    unsigned int indice;
    static int compteur;
    bool active;
public:
    unsigned int getId() const{return indice;}
    string getNom() const {return nom;}
    bool isActive() const {return active;};
};

class Partie{
private:
    Joueur* joueur1;
    Joueur* joueur2;
    unsigned int nombreTour;
    stack<Mouvement> historique;
    unsigned int nbUndo;
    void jouerUnTour();
    void setup();
    bool partieTerminee() const;
    Joueur* determinerGagnant() const;
    void annulerMouvement(Joueur* j);
    void sauvegarde();
    void ajouterExtension(unsigned int id);
public:
    vector<vector<Pion*>> plateau;
    unsigned int getNombreTour() const{return nombreTour;}
    Joueur* getJoueur1() const {return joueur1;}
    Joueur* getJoueur2() const {return joueur2;}
    unsigned int getNbUndo() const {return nbUndo;}
};

class JoueurHumain{
    public:
    void jouer();
};

class JoueurIA{
    public:
    void jouer();
};