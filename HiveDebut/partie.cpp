#include "partie.h"

Partie::~Partie() {
    delete joueur1;  // Libère la mémoire allouée pour joueur1
    delete joueur2;  // Libère la mémoire allouée pour joueur2

    // Libère chaque mouvement dans l'historique
    while (!historique.empty()) {
        delete historique.top();
        historique.pop();
    }
}

void Partie::jouerUnTour(Joueur* j) {
    // Le joueur joue son tour
    Mouvement* mouvement = j->Jouer(plateau);  // Joue le mouvement

    // Ajouter ce mouvement   l'historique
    ajouterMouvement(mouvement);

    if (j == getJoueur2()) {
        nombreTour += 1;
    }

    // V rification si la partie est termin e
    if (Partie::partieTerminee() == true) {
        if (getJoueur1() == Partie::determinerGagnant()) {
            std::cout << "Joueur 1 gagne !! La partie s'est finie en " << Partie::getNombreTour() << " tours." << std::endl;
        }
        else if (getJoueur2() == Partie::determinerGagnant()) {
            std::cout << "Joueur 2 gagne !! La partie s'est finie en " << Partie::getNombreTour() << " tours." << std::endl;
        }
        else {
            std::cout << "Egalit  !! La partie s'est finie en " << Partie::getNombreTour() << " tours." << std::endl;
        }
    }
}

void Partie::setup()
{

    //r nitialise le plateau
    int nbJoueur = 0;

    // Boucle jusqu'  ce que l'utilisateur entre 1 ou 2
    while (nbJoueur != 1 && nbJoueur != 2) {
        std::cout << "Entrez le nombre de joueurs (1 ou 2) : ";
        std::cin >> nbJoueur;

        if (nbJoueur != 1 && nbJoueur != 2) {
            std::cout << "Erreur : Vous devez entrer 1 ou 2 joueurs." << std::endl;
        }
    }

    std::cout << "Nombre de joueurs selectionn  : " << nbJoueur << std::endl;
    if (nbJoueur == 1) {
        std::vector<Pion*> pionsEnMainH;
        std::vector<Pion*> pionsEnMainIA;

        pionsEnMainH.push_back(new Pion("R", RED));
        pionsEnMainH.push_back(new Pion("S", RED));
        pionsEnMainH.push_back(new Pion("S", RED));
        pionsEnMainH.push_back(new Pion("S", RED));
        pionsEnMainH.push_back(new Pion("F", RED));
        pionsEnMainH.push_back(new Pion("F", RED));
        pionsEnMainH.push_back(new Pion("F", RED));
        pionsEnMainH.push_back(new Pion("C", RED));
        pionsEnMainH.push_back(new Pion("C", RED));

        pionsEnMainIA.push_back(new Pion("R", WHITE));
        pionsEnMainIA.push_back(new Pion("S", WHITE));
        pionsEnMainIA.push_back(new Pion("S", WHITE));
        pionsEnMainIA.push_back(new Pion("S", WHITE));
        pionsEnMainIA.push_back(new Pion("F", WHITE));
        pionsEnMainIA.push_back(new Pion("F", WHITE));
        pionsEnMainIA.push_back(new Pion("F", WHITE));
        pionsEnMainIA.push_back(new Pion("C", WHITE));
        pionsEnMainIA.push_back(new Pion("C", WHITE));

        joueur1 = new JoueurHumain(pionsEnMainH, RED);
        joueur2 = new JoueurIA(pionsEnMainIA, WHITE);

    }
    else {
        std::vector<Pion*> pionsEnMainH1;
        std::vector<Pion*> pionsEnMainH2;

        pionsEnMainH1.push_back(new Pion("R", RED));
        pionsEnMainH1.push_back(new Pion("S", RED));
        pionsEnMainH1.push_back(new Pion("S", RED));
        pionsEnMainH1.push_back(new Pion("S", RED));
        pionsEnMainH1.push_back(new Pion("F", RED));
        pionsEnMainH1.push_back(new Pion("F", RED));
        pionsEnMainH1.push_back(new Pion("F", RED));
        pionsEnMainH1.push_back(new Pion("C", RED));
        pionsEnMainH1.push_back(new Pion("C", RED));

        pionsEnMainH2.push_back(new Pion("R", WHITE));
        pionsEnMainH2.push_back(new Pion("S", WHITE));
        pionsEnMainH2.push_back(new Pion("S", WHITE));
        pionsEnMainH2.push_back(new Pion("S", WHITE));
        pionsEnMainH2.push_back(new Pion("F", WHITE));
        pionsEnMainH2.push_back(new Pion("F", WHITE));
        pionsEnMainH2.push_back(new Pion("F", WHITE));
        pionsEnMainH2.push_back(new Pion("C", WHITE));
        pionsEnMainH2.push_back(new Pion("C", WHITE));

        joueur1 = new JoueurHumain(pionsEnMainH1, RED);
        joueur2 = new JoueurHumain(pionsEnMainH2, WHITE);

    }

    //Extension Cocc("Coccinelle", 1);
    //Extension Mous("Moustique", 2);
    //Extension Clop("Cloporte", 3);
    //extensions.push_back(Cocc);
    //extensions.push_back(Mous);
    //extensions.push_back(Clop);

}

bool Partie::partieTerminee() const {
    // Obtenir tous les pions pr sents sur le plateau
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = plateau.gestionnairePions.getPions(plateau);

    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]);
        if (pion->getType() == "R") {
            // V rifie si la reine a exactement 6 voisins
            if (plateau.gestionnaireVoisins.nombreVoisins(*pion, plateau) == 6) {
                return true; // La partie est termin e
            }
        }
    }
    return false;
}
Joueur* Partie::determinerGagnant() const
{
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = plateau.gestionnairePions.getPions(plateau);

    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]);
        if (pion->getType() == "R") {
            // V rifie si la reine a exactement 6 voisins
            if (plateau.gestionnaireVoisins.nombreVoisins(*pion, plateau) == 6) {
                string couleurGagnante = pion->getCouleur();

                // D terminer le joueur ayant cette couleur
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

void Partie::annulerMouvement() {
    if (dynamic_cast<JoueurIA*>(getJoueur2())) {
        // Si le joueur 2 est une IA, on augmente le nombre d'undo de 2
        nbUndo += 2;
    }
    else {
        // Si c'est un joueur humain, on augmente de 1
        nbUndo += 1;
    }

    // Nombre d'actions   annuler
    int nbaction = static_cast<int>(historique.size());

    // Annuler les mouvements dans l'historique
    for (unsigned int i = 0; i < getNbUndo(); i++) {
        // V rifier si la pile n'est pas vide
        if (!historique.empty()) {
            // R cup rer le dernier mouvement (pointeur)
            Mouvement* ancienMouvement = historique.top();

            // D placer le pion dans l'autre sens (annuler le mouvement)
            Pion* pionABouger = plateau.gestionnairePions.getPion(ancienMouvement->getLigne(), ancienMouvement->getColonne(), plateau, ancienMouvement->getZ());
            plateau.gestionnairePions.movePion(ancienMouvement->getLigne(), ancienMouvement->getColonne(), ancienMouvement->getZ(), pionABouger, plateau);

            // Supprimer le mouvement de l'historique
            historique.pop();

            // Lib rer la m moire de l'objet Mouvement si n cessaire
            delete ancienMouvement;  // Suppression du pointeur pour  viter la fuite m moire
        }
    }

    // R initialiser le nombre d'undos
    nbUndo = 0;
}

void Partie::sauvegarde() {
    // Ouvrir un fichier pour la sauvegarde
    std::ofstream fichier("sauvegarde.txt");
    if (!fichier) {
        std::cerr << "Erreur : Impossible d ouvrir le fichier de sauvegarde." << std::endl;
        return;
    }

    // Sauvegarder le nombre de tour et le nombre d'undo
    fichier << "Nombre de tour: " << nombreTour << std::endl;
    fichier << "Nombre d'undo: " << nbUndo << std::endl;

    // Sauvegarder les informations des joueurs
    fichier << "Joueur 1:" << std::endl;
    fichier << "Couleur: " << joueur1->getCouleur() << std::endl;
    fichier << "Pions en main:" << std::endl;
    for (Pion* pion : joueur1->pionsEnMain) {
        fichier << "  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
    }

    fichier << "Joueur 2:" << std::endl;
    fichier << "Couleur: " << joueur2->getCouleur() << std::endl;
    fichier << "Pions en main:" << std::endl;
    for (Pion* pion : joueur2->pionsEnMain) {
        fichier << "  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
    }

    // Sauvegarder l' tat de la grille du plateau
    fichier << "Plateau:" << std::endl;
    for (unsigned int l = 0; l < plateau.nb_lignes; ++l) {
        for (unsigned int c = 0; c < plateau.nb_colonnes; ++c) {
            for (unsigned int z = 0; z < plateau.nb_couches; ++z) {
                Pion* pion = plateau.gestionnairePions.getPion(l, c, plateau, z);
                if (pion != nullptr) {
                    fichier << "  Position: (" << c << ", " << l << ", " << z << "), ";
                    fichier << "Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
                }
            }
        }
    }

    // Sauvegarder l'historique des mouvements
    fichier << "Historique des mouvements:" << std::endl;
    std::stack<Mouvement*> tempHistorique = historique; // Copie pour ne pas d truire l'historique
    while (!tempHistorique.empty()) {
        Mouvement* mvt = tempHistorique.top();
        tempHistorique.pop();
        fichier << "  De (" << mvt->getOldColonne() << ", " << mvt->getOldLigne() << ", " << mvt->getOldZ() << ") ";
        fichier << "  (" << mvt->getColonne() << ", " << mvt->getLigne() << ", " << mvt->getZ() << ")" << std::endl;
    }

    /*
    // Sauvegarder la liste des extensions actives
    fichier << "Extensions actives:" << std::endl;
    for (const Extension& extension : extensions) {
        if (extension.isActive()) {
            fichier << "  Nom: " << extension.getNom() << ", ID: " << extension.getId() << std::endl;
        }
    }
    */

    // Fermer le fichier
    fichier.close();
    std::cout << "Sauvegarde de la partie reussie dans 'sauvegarde.txt'." << std::endl;
}

/*
void Partie::ajouterExtension(unsigned int id)
{
    Extension::getNom();
    Extention.id.active = !Extention.active;
    Extension::compteur += 1;
}
*/