﻿#include "partie.h"

Partie::~Partie() {
    delete joueur1;  // Libère la mémoire allouée pour joueur1
    delete joueur2;  // Libère la mémoire allouée pour joueur2
}

void Partie::jouerUnTour(Joueur* j) {
    // Le joueur joue son tour
    if (historique.size() >= 2 && getNbUndo() >= 1 && !j->estIA()) {
        int choixUndo;
        std::cout << "Voulez-vous annuler (1) ou non (2) un mouvement ? (" << getNbUndo() << " restants) ";
        std::cin >> choixUndo;
        if (choixUndo == 1) {
            annulerMouvement();
            plateau.afficher();
        }
    }

    Mouvement* mouvement = j->Jouer(getPlateau());  // Joue le mouvement

    if (mouvement) {
        ajouterMouvement(mouvement);  // Executer et ajouter à l'historique
        if (j == getJoueur2()) {
            nombreTour++;
        }
    }

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
            std::cout << "Egalit  !! La partie s'est finie en " << Partie::getNombreTour() << " tours." << std::endl;
        }
    }
}

std::vector<std::string> Partie::listerSauvegardes() {
    std::vector<std::string> sauvegardes;
    std::ifstream fichierListe("liste_sauvegardes.txt");
    if (fichierListe) {
        std::string ligne;
        while (std::getline(fichierListe, ligne)) {
            if (!ligne.empty()) {
                sauvegardes.push_back(ligne);
            }
        }
    }
    return sauvegardes;
}

void creerDossierSiInexistant(const std::string& cheminDossier) {
    if (_mkdir(cheminDossier.c_str()) == 0) {
        std::cout << "Dossier cree avec succes : " << cheminDossier << std::endl;
    }
    else if (errno == EEXIST) {
        std::cout << "Le dossier existe deja : " << cheminDossier << std::endl;
    }
    else {
        std::cerr << "Erreur : Impossible de creer le dossier " << cheminDossier << std::endl;
    }
}

void Partie::setup() {
    // Créer le dossier "sauvegardes" s'il n'existe pas
    const std::string dossierSauvegarde = "sauvegardes";
    creerDossierSiInexistant(dossierSauvegarde);

    int choix = 0;

    // Demander à l'utilisateur s'il souhaite charger une partie ou en commencer une nouvelle
    std::cout << "Voulez-vous\n(1) Charger une partie\n(2) Commencer une nouvelle partie\n";
    std::cin >> choix;

    if (choix == 1) {
        // Charger la liste des fichiers de sauvegarde
        std::vector<std::string> fichiersSauvegarde;
        std::ifstream listeFichiers(dossierSauvegarde + "/liste_sauvegardes.txt");

        if (!listeFichiers) {
            std::cerr << "Aucune liste de sauvegarde disponible." << std::endl;
        }
        else {
            std::string nomFichier;
            while (std::getline(listeFichiers, nomFichier)) {
                fichiersSauvegarde.push_back(nomFichier);
            }
            listeFichiers.close();
        }

        if (fichiersSauvegarde.empty()) {
            std::cout << "Aucune sauvegarde trouvee. Creation d'une nouvelle partie." << std::endl;
        }
        else {
            std::cout << "Sauvegardes disponibles : " << std::endl;
            for (size_t i = 0; i < fichiersSauvegarde.size(); ++i) {
                std::cout << i + 1 << ". " << fichiersSauvegarde[i] << std::endl;
            }

            int choixSauvegarde = 0;
            while (choixSauvegarde < 1 || choixSauvegarde > fichiersSauvegarde.size()) {
                std::cout << "Choisissez une sauvegarde a charger (1-" << fichiersSauvegarde.size() << ") : ";
                std::cin >> choixSauvegarde;
            }

            nomPartie = fichiersSauvegarde[choixSauvegarde - 1];
            if (chargerPartie()) {
                std::cout << "Partie chargee avec succes !" << std::endl;
                return;
            }
            else {
                std::cout << "Erreur lors du chargement de la partie. Creation d'une nouvelle partie." << std::endl;
            }
        }
    }

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

    std::cout << "Nombre de joueurs selectionne : " << nbJoueur << std::endl;

    if (nbJoueur == 1) {
        joueur1 = new JoueurHumain(initialiserPions(RED), RED);
        joueur2 = new JoueurIA(initialiserPions(WHITE), WHITE);
    }
    else {
        joueur1 = new JoueurHumain(initialiserPions(RED), RED);
        joueur2 = new JoueurHumain(initialiserPions(WHITE), WHITE);
    }
}

std::vector<Pion*> Partie::initialiserPions(const std::string& couleur) {
    std::vector<Pion*> pions;

    // Créer les pions et les ajouter via la méthode Pion::ajouterPion
    Pion* pionR = new Pion("R", couleur);
    Pion::ajouterPion(pionR);  // Appel de Pion::ajouterPion pour ajouter le pion au gestionnaire
    pions.push_back(pionR);  // Si vous avez besoin de garder une copie locale du pion

    Pion* pionS1 = new Pion("S", couleur);
    Pion::ajouterPion(pionS1);
    pions.push_back(pionS1);

    Pion* pionS2 = new Pion("S", couleur);
    Pion::ajouterPion(pionS2);
    pions.push_back(pionS2);

    Pion* pionS3 = new Pion("S", couleur);
    Pion::ajouterPion(pionS3);
    pions.push_back(pionS3);

    Pion* pionF1 = new Pion("F", couleur);
    Pion::ajouterPion(pionF1);
    pions.push_back(pionF1);

    Pion* pionF2 = new Pion("F", couleur);
    Pion::ajouterPion(pionF2);
    pions.push_back(pionF2);

    Pion* pionF3 = new Pion("F", couleur);
    Pion::ajouterPion(pionF3);
    pions.push_back(pionF3);

    Pion* pionC1 = new Pion("C", couleur);
    Pion::ajouterPion(pionC1);
    pions.push_back(pionC1);

    Pion* pionC2 = new Pion("C", couleur);
    Pion::ajouterPion(pionC2);
    pions.push_back(pionC2);

    return pions;
}

bool Partie::chargerPartie() {
    std::string nomFichier = "sauvegardes/" + nomPartie + ".txt"; // Utiliser nomPartie pour le fichier
    std::ifstream fichier(nomFichier);
    if (!fichier) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier de sauvegarde." << std::endl;
        return false;
    }

    std::string ligne;

    // Lire le nombre de tours et d'undo
    std::getline(fichier, ligne);
    nombreTour = std::stoi(ligne.substr(ligne.find(":") + 2));

    std::getline(fichier, ligne);
    nbUndo = std::stoi(ligne.substr(ligne.find(":") + 2));

    // Charger les informations du joueur 1
    std::getline(fichier, ligne); // "Joueur 1"
    std::getline(fichier, ligne); // "Type: Humain/IA"
    std::string typeJ1 = ligne.substr(ligne.find(":") + 2);

    std::getline(fichier, ligne); // "Couleur: Couleur_Joueur"
    std::string couleurJ1 = ligne.substr(ligne.find(":") + 2);

    std::vector<Pion*> pionsJ1;
    std::getline(fichier, ligne); // "Pions en main:"
    while (std::getline(fichier, ligne) && ligne.find("ID:") != std::string::npos) {
        // Extraire l'ID du pion
        size_t posId = ligne.find("ID: ") + 4;
        size_t posType = ligne.find("Type: ");
        int id = std::stoi(ligne.substr(posId, posType - posId - 2));

        // Extraire le type
        posType += 6;
        size_t posCouleur = ligne.find(", Couleur: ");
        std::string type = ligne.substr(posType, posCouleur - posType);

        // Extraire la couleur
        std::string couleur = ligne.substr(posCouleur + 11);

        Pion* pion = new Pion(id, type, couleur); // Création avec ID
        Pion::ajouterPion(pion);
        pionsJ1.push_back(pion);                  // Ajouter aux pions du joueur
    }
    // Créer le joueur 1
    if (typeJ1 == "Humain")
        joueur1 = new JoueurHumain(pionsJ1, couleurJ1);
    else
        joueur1 = new JoueurIA(pionsJ1, couleurJ1);

    // Charger les informations du joueur 2
    std::getline(fichier, ligne); // "Type: Humain/IA"
    std::string typeJ2 = ligne.substr(ligne.find(":") + 2);
    
    std::getline(fichier, ligne); // "Couleur: Couleur_Joueur"
    std::string couleurJ2 = ligne.substr(ligne.find(":") + 2);

    std::vector<Pion*> pionsJ2;
    std::getline(fichier, ligne); // "Pions en main:"
    
    while (std::getline(fichier, ligne) && ligne.find("ID:") != std::string::npos) {
        // Extraire l'ID du pion
        size_t posId = ligne.find("ID: ") + 4;
        size_t posType = ligne.find("Type: ");
        int id = std::stoi(ligne.substr(posId, posType - posId - 2));

        // Extraire le type
        posType += 6;
        size_t posCouleur = ligne.find(", Couleur: ");
        std::string type = ligne.substr(posType, posCouleur - posType);

        // Extraire la couleur
        std::string couleur = ligne.substr(posCouleur + 11);

        Pion* pion = new Pion(id, type, couleur); // Création avec ID
        Pion::ajouterPion(pion);
        pionsJ2.push_back(pion);                  // Ajouter aux pions du joueur
    }
    // Créer le joueur 2
    if (typeJ2 == "Humain")
        joueur2 = new JoueurHumain(pionsJ2, couleurJ2);
    else
        joueur2 = new JoueurIA(pionsJ2, couleurJ2);

    // Charger l'état du plateau
    while (std::getline(fichier, ligne) && ligne.find("Position") != std::string::npos) {
        // Extraire les coordonnées
        size_t pos1 = ligne.find("(") + 1;
        size_t pos2 = ligne.find(",");
        int colonne = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(",", pos1);
        int lignePlateau = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(")", pos1);
        int couche = std::stoi(ligne.substr(pos1, pos2 - pos1));

        // Extraire l'ID, le type et la couleur du pion
        size_t posId = ligne.find("ID: ") + 4;
        size_t posType = ligne.find("Type: ");
        int id = std::stoi(ligne.substr(posId, posType - posId - 2));

        posType += 6;
        size_t posCouleur = ligne.find(", Couleur: ");
        std::string type = ligne.substr(posType, posCouleur - posType);

        std::string couleur = ligne.substr(posCouleur + 11);
        // Créer ou récupérer le pion et le placer sur le plateau
        Pion* pion = new Pion(id, type, couleur); // Avec ID
        Pion::ajouterPion(pion);
        plateau.gestionnairePions.setPion(lignePlateau, colonne, couche, pion, plateau);
    }

    // Charger l'historique des mouvements
    while (std::getline(fichier, ligne) && ligne.find("ID") != std::string::npos) {
        // Extraire l'ID du pion
        size_t pos1 = ligne.find(":") + 2;
        size_t pos2 = ligne.find(" ", pos1);
        int pionId = std::stoi(ligne.substr(pos1, pos2 - pos1));

        // Extraire les anciennes coordonnées
        pos1 = ligne.find("(") + 1;
        pos2 = ligne.find(",");
        int oldLigne = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(",", pos1);
        int oldColonne = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(")", pos1);
        int oldZ = std::stoi(ligne.substr(pos1, pos2 - pos1));

        // Extraire les nouvelles coordonnées
        pos1 = ligne.find("(", pos2) + 1;
        pos2 = ligne.find(",", pos1);
        int newLigne = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(",", pos1);
        int newColonne = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(")", pos1);
        int newZ = std::stoi(ligne.substr(pos1, pos2 - pos1));
        
        // Créer un mouvement en utilisant l'ID du pion
        Mouvement* mvt = new Mouvement(pionId, newLigne, newColonne, newZ, oldLigne, oldColonne, oldZ);
        historique.push(mvt);
    }
    fichier.close();
    return true;
}



bool Partie::partieTerminee() const {
    // Obtenir tous les pions présents sur le plateau
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = plateau.gestionnairePions.getPions(plateau);

    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]);
        if (pion->getType() == "R") {
            // Vérifie si la reine a exactement 6 voisins
            if (plateau.gestionnaireVoisins.nombreVoisins(*pion, plateau) == 6) {
                return true; // La partie est terminée
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
            // Vérifie si la reine a exactement 6 voisins
            if (plateau.gestionnaireVoisins.nombreVoisins(*pion, plateau) == 6) {
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

void Partie::annulerMouvement() {
    if (historique.size() >= 2) {
        // Récupérer le dernier mouvement
        //Problème avec les pions qui ont été posé
        Mouvement* mouvement = historique.top();
        historique.pop();
        // Inverser le mouvement : ramener le pion à son origine
        Pion* pion = Pion::getPionById(mouvement->getPionId());
        Pion::getPion();
        Joueur* j = getJoueur1();
        if (j->getCouleur() != pion->getCouleur()) {
            j = getJoueur2();
        }
        tuple<int, int, int> coordSrc = mouvement->getCoordSrc();
        if (mouvement->getOldLigne() == -1 && mouvement->getOldColonne() == -1 && mouvement->getOldZ() == -1) {
            plateau.gestionnairePions.deletePion(*pion, plateau);
            j->pionsEnMain.push_back(pion);
        }
        else {
            plateau.gestionnairePions.movePion(
                mouvement->getOldLigne(),
                mouvement->getOldColonne(),
                mouvement->getOldZ(),
                pion,
                plateau
            );
        }
        
        // Libérer la mémoire
        delete mouvement;

        Mouvement* mouvement1 = historique.top();
        historique.pop();
        // Inverser le mouvement : ramener le pion à son origine
        Pion* pion1 = Pion::getPionById(mouvement1->getPionId());
        if (j->getCouleur() != pion1->getCouleur()) {
            j = getJoueur2();
        }
        if (mouvement1->getOldLigne() == -1 && mouvement1->getOldColonne() == -1 && mouvement1->getOldZ() == -1) {
            plateau.gestionnairePions.deletePion(*pion1, plateau);
            j->pionsEnMain.push_back(pion1);
        }
        else {
            plateau.gestionnairePions.movePion(
                mouvement1->getOldLigne(),
                mouvement1->getOldColonne(),
                mouvement1->getOldZ(),
                pion1,
                plateau
            );
        }
        // Libérer la mémoire
        delete mouvement1;

        nbUndo--;
    }
    else {
        std::cout << "Aucun mouvement à annuler." << std::endl;
    }
}


void Partie::sauvegarde() {
    std::string nomFichier = "sauvegardes/" + nomPartie + ".txt"; // Utiliser nomPartie pour le fichier
    std::ofstream fichier(nomFichier);
    if (!fichier) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier de sauvegarde." << std::endl;
        return;
    }

    // Sauvegarder les données comme avant...
    fichier << "Nombre de tour: " << nombreTour << std::endl;
    fichier << "Nombre d'undo: " << nbUndo << std::endl;

    fichier << "Joueur 1:" << std::endl;
    fichier << "Type: " << (dynamic_cast<JoueurHumain*>(joueur1) ? "Humain" : "IA") << std::endl;
    fichier << "Couleur: " << joueur1->getCouleur() << std::endl;
    fichier << "Pions en main:" << std::endl;
    for (Pion* pion : joueur1->pionsEnMain) {
        fichier << "  ID: " << pion->getId() << "  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
    }

    fichier << "Joueur 2:" << std::endl;
    fichier << "Type: " << (dynamic_cast<JoueurHumain*>(joueur2) ? "Humain" : "IA") << std::endl;
    fichier << "Couleur: " << joueur2->getCouleur() << std::endl;
    fichier << "Pions en main:" << std::endl;
    for (Pion* pion : joueur2->pionsEnMain) {
        fichier << "  ID: " << pion->getId() << "  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
    }

    fichier << "Plateau:" << std::endl;
    for (unsigned int l = 0; l < plateau.nb_lignes; ++l) {
        for (unsigned int c = 0; c < plateau.nb_colonnes; ++c) {
            for (unsigned int z = 0; z < plateau.nb_couches; ++z) {
                Pion* pion = plateau.gestionnairePions.getPion(l, c, plateau, z);
                if (pion != nullptr) {
                    fichier << "  Position: (" << c << ", " << l << ", " << z << "), ";
                    fichier << "  ID: " << pion->getId() << ",  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
                }
            }
        }
    }

    fichier << "Historique des mouvements:" << std::endl;
    std::stack<Mouvement*> tempPile;  // Pile temporaire pour inverser l'ordre

    // Copier les éléments dans une pile temporaire (ce qui inverse l'ordre)
    while (!historique.empty()) {
        tempPile.push(historique.top());
        historique.pop();
    }

    // Écrire les éléments depuis la pile temporaire
    while (!tempPile.empty()) {
        Mouvement* mvt = tempPile.top();
        tempPile.pop();

        fichier << "ID: " << mvt->getPionId() << " ";
        fichier << "  De (" << mvt->getOldLigne() << ", " << mvt->getOldColonne() << ", " << mvt->getOldZ() << ") ";
        fichier << "  A (" << mvt->getLigne() << ", " << mvt->getColonne() << ", " << mvt->getZ() << ")" << std::endl;

        // Restaurer l'historique original
        historique.push(mvt);
    }

    fichier.close();
    std::cout << "Sauvegarde de la partie reussie dans '" << nomFichier << "'." << std::endl;
}


/*
void Partie::ajouterExtension(unsigned int id)
{
    Extension::getNom();
    Extention.id.active = !Extention.active;
    Extension::compteur += 1;
}
*/