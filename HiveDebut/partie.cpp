﻿#include "partie.h"

std::stack<Command*> Partie::historique;

Partie::~Partie() {
    delete joueur1;  // Libère la mémoire allouée pour joueur1
    delete joueur2;  // Libère la mémoire allouée pour joueur2
}

void Partie::setup() {
    // Créer le dossier "sauvegardes" s'il n'existe pas
    const std::string dossierSauvegarde = "sauvegardes";
    GestionnaireSauvegarde::creerDossierSiInexistant(dossierSauvegarde);
    int choix = -1;
    while (choix != 1 && choix != 2) {
        choix = choixChargementOuCreationPartie();
        if (choix == 1) {
            bool partie_chargee = GestionnaireSauvegarde::chargementSauvegardePartie(*this, dossierSauvegarde);
            if (partie_chargee) {
                return;
            }
        }
        else if (choix == 2) {
            creationPartie(dossierSauvegarde);
        }
        else {
            cout << "ERREUR : Chiffre invalide.\n\n";
        }
    }
}

int Partie::choixChargementOuCreationPartie() {
    // Demander à l'utilisateur s'il souhaite charger une partie ou en commencer une nouvelle
    int choix;
    std::cout << "Voulez-vous\n(1) Charger une partie\n(2) Commencer une nouvelle partie\n";
    std::cin >> choix;
    return choix;
}


void Partie::creationPartie(const std::string dossierSauvegarde) {

    // Création d'une nouvelle partie
    std::cout << "Entrez un nom pour la nouvelle partie : ";
    std::cin >> nomPartie;

    // Ajouter le nom dans le fichier de liste
    std::ofstream listeFichiers(dossierSauvegarde + "/liste_sauvegardes.txt", std::ios::app);
    if (listeFichiers) {
        listeFichiers << nomPartie << std::endl;
    }

    std::cout << "Nouvelle partie creee : " << nomPartie << std::endl;

    int nbUndoCin = -1;
    while (nbUndoCin < 0) {
        std::cout << "Entrez le nombre de retour en arriere : ";
        std::cin >> nbUndoCin;

        if (nbUndoCin < 0) {
            std::cout << "Erreur : Vous devez entrer un nombre positif." << std::endl;
        }
    }
    nbUndo = nbUndoCin;

    // Initialisation de la partie
    int nbJoueur = 0;
    while (nbJoueur != 1 && nbJoueur != 2) {
        std::cout << "Entrez le nombre de joueurs (1 ou 2) : ";
        std::cin >> nbJoueur;

        if (nbJoueur != 1 && nbJoueur != 2) {
            std::cout << "Erreur : Vous devez entrer 1 ou 2 joueurs." << std::endl;
        }
    }
    std::string nomJoueur1, nomJoueur2;
    std::cout << "Entrez le nom du joueur 1 : ";
    std::cin >> nomJoueur1;
    if (nbJoueur == 2) {
        std::cout << "Entrez le nom du joueur 2 : ";
        std::cin >> nomJoueur2;
    }
    else {
        nomJoueur2 = "IA"; // Nom par défaut pour l'IA
    }

    std::cout << "Nombre de joueurs selectionne : " << nbJoueur << std::endl;

    if (nbJoueur == 1) {
        joueur1 = new JoueurHumain(nomJoueur1, initialiserPions(RED), RED, *this);
        joueur2 = new JoueurIA(nomJoueur2, initialiserPions(WHITE), WHITE, *this);
    }
    else {
        joueur1 = new JoueurHumain(nomJoueur1, initialiserPions(RED), RED, *this);
        joueur2 = new JoueurHumain(nomJoueur2, initialiserPions(WHITE), WHITE, *this);
    }
}

std::vector<Pion*> Partie::initialiserPions(const std::string& couleur) {
    std::vector<Pion*> pions;
    UsineDePions usine;

    // Créer les pions et les ajouter via la méthode Pion::ajouterPion
    // Reine
    Pion* pionR = usine.creerPion("R", couleur);
    Pion::ajouterPion(pionR);  // Appel de Pion::ajouterPion pour ajouter le pion au gestionnaire
    pions.push_back(pionR);  // Si vous avez besoin de garder une copie locale du pion

    // Sauterelle
    Pion* pionS1 = usine.creerPion("S", couleur);
    Pion::ajouterPion(pionS1);
    pions.push_back(pionS1);
    Pion* pionS2 = usine.creerPion("S", couleur);
    Pion::ajouterPion(pionS2);
    pions.push_back(pionS2);
    Pion* pionS3 = usine.creerPion("S", couleur);
    Pion::ajouterPion(pionS3);
    pions.push_back(pionS3);

    // Fourmi
    Pion* pionF1 = usine.creerPion("F", couleur);
    Pion::ajouterPion(pionF1);
    pions.push_back(pionF1);
    Pion* pionF2 = usine.creerPion("F", couleur);
    Pion::ajouterPion(pionF2);
    pions.push_back(pionF2);
    Pion* pionF3 = usine.creerPion("F", couleur);
    Pion::ajouterPion(pionF3);
    pions.push_back(pionF3);

    // Scarabee
    Pion* pionK1 = usine.creerPion("K", couleur);
    Pion::ajouterPion(pionK1);
    pions.push_back(pionK1);
    Pion* pionK2 = usine.creerPion("K", couleur);
    Pion::ajouterPion(pionK2);
    pions.push_back(pionK2);
    
    // Araignee
    Pion* pionA1 = usine.creerPion("A", couleur);
    Pion::ajouterPion(pionA1);
    pions.push_back(pionA1);
    Pion* pionA2 = usine.creerPion("A", couleur);
    Pion::ajouterPion(pionA2);
    pions.push_back(pionA2);

    return pions;
}

void Partie::jouerUnTour(Joueur* j) {
    // Le joueur joue son tour
    j->Jouer(getPlateau());  // Joue le mouvement

    if (j == getJoueur2()) {
        nombreTour += 1;
    }

    // Verification si la partie est terminee
    if (Partie::partieTerminee() == true) {
        if (getJoueur1() == Partie::determinerGagnant()) {
            std::cout << "Joueur 1 gagne !! La partie s'est finie en " << Partie::getNombreTour() << " tours." << std::endl;
        }
        else if (getJoueur2() == Partie::determinerGagnant()) {
            std::cout << "Joueur 2 gagne !! La partie s'est finie en " << Partie::getNombreTour() << " tours." << std::endl;
        }
        else {
            std::cout << "Egalite !! La partie s'est finie en " << Partie::getNombreTour() << " tours." << std::endl;
        }
    }
}

void Partie::annulerMouvement() {
    if (historique.size() >= 2) {
        GestionnaireCommand::undoCommand(*this);
        GestionnaireCommand::undoCommand(*this);
        nbUndo--;
    }
    else {
        std::cout << "Aucun mouvement à annuler." << std::endl;
    }
}

bool Partie::partieTerminee() const {
    // Obtenir tous les pions présents sur le plateau
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = GestionnairePions::getPions(plateau);

    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]);
        if (pion->getType() == "R") {
            // Vérifie si la reine a exactement 6 voisins
            if (GestionnaireVoisins::nombreVoisins(*pion, plateau) == 6) {
                return true; // La partie est terminée
            }
        }
    }
    return false;
}

Joueur* Partie::determinerGagnant() const
{
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = GestionnairePions::getPions(plateau);

    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]);
        if (pion->getType() == "R") {
            // Vérifie si la reine a exactement 6 voisins
            if (GestionnaireVoisins::nombreVoisins(*pion, plateau) == 6) {
                string couleurGagnante = pion->getCouleur();

                // Déterminer le joueur ayant cette couleur
                if (joueur1 && joueur1->getCouleur() == couleurGagnante) {
                    return joueur1;
                }
                else if (joueur2 && joueur2->getCouleur() == couleurGagnante) {
                    return joueur2;
                }
            }
        }
    }
    return nullptr;
}