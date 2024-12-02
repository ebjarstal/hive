﻿#include "partie.h"

std::stack<Command*> Partie::historique;

Partie::~Partie() {
    delete joueur1;  // Libère la mémoire allouée pour joueur1
    delete joueur2;  // Libère la mémoire allouée pour joueur2
}

void Partie::setup() {
    // Créer le dossier "sauvegardes" s'il n'existe pas
    const std::string dossierSauvegarde = "sauvegardes";
    Partie::creerDossierSiInexistant(dossierSauvegarde);
    int choix = -1;
    while (choix != 1 && choix != 2) {
        choix = choixChargementOuCreationPartie();
        if (choix == 1) {
            bool partie_chargee = chargementSauvegardePartie(dossierSauvegarde);
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

void Partie::creerDossierSiInexistant(const std::string& cheminDossier) {
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

int Partie::choixChargementOuCreationPartie() {
    // Demander à l'utilisateur s'il souhaite charger une partie ou en commencer une nouvelle
    int choix;
    std::cout << "Voulez-vous\n(1) Charger une partie\n(2) Commencer une nouvelle partie\n";
    std::cin >> choix;
    return choix;
}

bool Partie::chargementSauvegardePartie(const std::string dossierSauvegarde) {

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
        return false;
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
            return true;
        }
        else {
            std::cout << "Erreur lors du chargement de la partie. Creation d'une nouvelle partie." << std::endl;
            return false;
        }
    }
}

bool Partie::chargerPartie() {
    UsineDePions usine_j1;
    UsineDePions usine_j2;

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

        Pion* pion = usine_j1.creerPion(id, type, couleur); // Création avec ID
        Pion::ajouterPion(pion);
        pionsJ1.push_back(pion); // Ajouter aux pions du joueur
    }
    // Créer le joueur 1
    if (typeJ1 == "Humain")
        joueur1 = new JoueurHumain(pionsJ1, couleurJ1, *this);
    else
        joueur1 = new JoueurIA(pionsJ1, couleurJ1, *this);

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
        Pion* pion = usine_j2.creerPion(id, type, couleur); // Création avec ID
        Pion::ajouterPion(pion);
        pionsJ2.push_back(pion);                  // Ajouter aux pions du joueur
    }
    // Créer le joueur 2
    if (typeJ2 == "Humain")
        joueur2 = new JoueurHumain(pionsJ2, couleurJ2, *this);
    else
        joueur2 = new JoueurIA(pionsJ2, couleurJ2, *this);

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
        if (couleur == joueur1->getCouleur()) {
            Pion* pion = usine_j1.creerPion(id, type, couleur); // Avec ID
            Pion::ajouterPion(pion);
            GestionnairePions::setPion(lignePlateau, colonne, couche, pion, plateau);
        }
        else {
            Pion* pion = usine_j2.creerPion(id, type, couleur); // Avec ID
            Pion::ajouterPion(pion);
            GestionnairePions::setPion(lignePlateau, colonne, couche, pion, plateau);
        }
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

        // Extraire le joueur
        pos1 = pos2 + 2;
        pos2 = ligne.find("|", pos1);
        int newJoueur = std::stoi(ligne.substr(pos1, pos2 - pos1));

        // Créer un mouvement en utilisant l'ID du pion
        Joueur& j = *joueur2;
        Mouvement* mvt = new Mouvement(pionId, newLigne, newColonne, newZ, oldLigne, oldColonne, oldZ);
        if (newJoueur == 1) {
            Joueur& j = *joueur1;
        }

        auto mouvementCommand = new MouvementCommand(*this, mvt);
        historique.push(mouvementCommand);
    }
    fichier.close();
    return true;
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

    std::cout << "Nombre de joueurs selectionne : " << nbJoueur << std::endl;

    if (nbJoueur == 1) {
        joueur1 = new JoueurHumain(initialiserPions(RED), RED, *this);
        joueur2 = new JoueurIA(initialiserPions(WHITE), WHITE, *this);
    }
    else {
        joueur1 = new JoueurHumain(initialiserPions(RED), RED, *this);
        joueur2 = new JoueurHumain(initialiserPions(WHITE), WHITE, *this);
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

    // Coccinelle
    Pion* pionM = usine.creerPion("M", couleur);
    Pion::ajouterPion(pionM);
    pions.push_back(pionM);

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

void Partie::sauvegarde() {
    std::string nomFichier = "sauvegardes/" + nomPartie + ".txt"; // Utiliser nomPartie pour le fichier
    std::ofstream fichier(nomFichier);
    if (!fichier) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier de sauvegarde." << std::endl;
        return;
    }

    // Sauvegarder les données de base
    fichier << "Nombre de tour: " << nombreTour << std::endl;
    fichier << "Nombre d'undo: " << nbUndo << std::endl;

    // Sauvegarde des joueurs
    fichier << "Joueur 1:" << std::endl;
    fichier << "Type: " << (dynamic_cast<JoueurHumain*>(joueur1) ? "Humain" : "IA") << std::endl;
    fichier << "Couleur: " << joueur1->getCouleur() << std::endl;
    fichier << "Pions en main:" << std::endl;
    for (Pion* pion : joueur1->getPionsEnMain()) {
        fichier << "  ID: " << pion->getId() << "  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
    }

    fichier << "Joueur 2:" << std::endl;
    fichier << "Type: " << (dynamic_cast<JoueurHumain*>(joueur2) ? "Humain" : "IA") << std::endl;
    fichier << "Couleur: " << joueur2->getCouleur() << std::endl;
    fichier << "Pions en main:" << std::endl;
    for (Pion* pion : joueur2->getPionsEnMain()) {
        fichier << "  ID: " << pion->getId() << "  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
    }

    // Sauvegarde du plateau
    fichier << "Plateau:" << std::endl;
    for (unsigned int l = 0; l < plateau.getNbLignes(); ++l) {
        for (unsigned int c = 0; c < plateau.getNbColonnes(); ++c) {
            for (unsigned int z = 0; z < plateau.getNbCouches(); ++z) {
                Pion* pion = GestionnairePions::getPion(l, c, plateau, z);
                if (pion != nullptr) {
                    fichier << "  Position: (" << c << ", " << l << ", " << z << "), ";
                    fichier << "  ID: " << pion->getId() << ",  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
                }
            }
        }
    }

    // Vérification de la présence de callback dans l'historique
    fichier << "Historique des mouvements:" << std::endl;
    bool callbackDetected = false;
    std::stack<Command*> historiqueInversee;

    while (!historique.empty()) {
        Command* cmd = historique.top();
        MouvementCommand* mouvCmd = dynamic_cast<MouvementCommand*>(cmd);

        if (mouvCmd && mouvCmd->getMouvement()->hasCallback()) {
            callbackDetected = true;
        }

        historiqueInversee.push(cmd);
        historique.pop();
    }

    // Si une callback est détectée, ne pas sauvegarder les détails des mouvements
    if (!callbackDetected) {
        while (!historiqueInversee.empty()) {
            Command* cmd = historiqueInversee.top();
            fichier << cmd->getDescription() << std::endl;
            historique.push(historiqueInversee.top());
            historiqueInversee.pop();
        }
    }

    // Restaurer l'historique original
    while (!historiqueInversee.empty()) {
        historique.push(historiqueInversee.top());
        historiqueInversee.pop();
    }

    fichier.close();
    std::cout << "Sauvegarde de la partie réussie dans '" << nomFichier << "'." << std::endl;
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