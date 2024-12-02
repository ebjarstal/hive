#include "gestionnaireSauvegarde.h"
#include "partie.h"

void GestionnaireSauvegarde::creerDossierSiInexistant(const std::string& cheminDossier) {
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

bool GestionnaireSauvegarde::chargementSauvegardePartie(Partie& p, const std::string dossierSauvegarde) {
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
        p.nomPartie = fichiersSauvegarde[choixSauvegarde - 1];
        if (chargerPartie(p)) {
            std::cout << "Partie chargee avec succes !" << std::endl;
            return true;
        }
        else {
            std::cout << "Erreur lors du chargement de la partie. Creation d'une nouvelle partie." << std::endl;
            return false;
        }
    }
}

bool GestionnaireSauvegarde::chargerPartie(Partie& p) {
    UsineDePions usine_j1;
    UsineDePions usine_j2;

    std::string nomFichier = "sauvegardes/" + p.nomPartie + ".txt"; // Utiliser nomPartie pour le fichier
    std::ifstream fichier(nomFichier);
    if (!fichier) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier de sauvegarde." << std::endl;
        return false;
    }

    std::string ligne;

    // Lire le nombre de tours et d'undo
    std::getline(fichier, ligne);
    p.setNombreTour(std::stoi(ligne.substr(ligne.find(":") + 2)));

    std::getline(fichier, ligne);
    p.setNbUndo(std::stoi(ligne.substr(ligne.find(":") + 2)));

    // Charger les informations du joueur 1
    std::getline(fichier, ligne); // "Joueur 1"
    std::getline(fichier, ligne); // "Type: Humain/IA"
    std::string typeJ1 = ligne.substr(ligne.find(":") + 2);

    std::getline(fichier, ligne); // "Couleur: Nom_Joueur"
    std::string nomJ1 = ligne.substr(ligne.find(":") + 2);

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
        p.setJoueur1(new JoueurHumain(nomJ1, pionsJ1, couleurJ1, p));
    else
        p.setJoueur1(new JoueurIA(nomJ1, pionsJ1, couleurJ1, p));

    // Charger les informations du joueur 2
    std::getline(fichier, ligne); // "Type: Humain/IA"
    std::string typeJ2 = ligne.substr(ligne.find(":") + 2);

    std::getline(fichier, ligne); // "Couleur: Nom_Joueur"
    std::string nomJ2 = ligne.substr(ligne.find(":") + 2);

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
        p.setJoueur2(new JoueurHumain(nomJ2, pionsJ2, couleurJ2, p));
    else
        p.setJoueur2(new JoueurIA(nomJ2, pionsJ2, couleurJ2, p));

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
        if (couleur == p.getJoueur1()->getCouleur()) {
            Pion* pion = usine_j1.creerPion(id, type, couleur); // Avec ID
            Pion::ajouterPion(pion);
            GestionnairePions::setPion(lignePlateau, colonne, couche, pion, p.getPlateau());
        }
        else {
            Pion* pion = usine_j2.creerPion(id, type, couleur); // Avec ID
            Pion::ajouterPion(pion);
            GestionnairePions::setPion(lignePlateau, colonne, couche, pion, p.getPlateau());
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
        Joueur& j = *p.getJoueur2();
        Mouvement* mvt = new Mouvement(pionId, newLigne, newColonne, newZ, oldLigne, oldColonne, oldZ);
        if (newJoueur == 1) {
            Joueur& j = *p.getJoueur1();
        }

        auto mouvementCommand = new MouvementCommand(p, mvt);
        p.getHistorique().push(mouvementCommand);
    }
    fichier.close();
    return true;
}

void GestionnaireSauvegarde::sauvegarde(Partie& p) {

    string nomPartie = p.nomPartie;
    int nombreTour = p.getNombreTour();
    int nbUndo = p.getNbUndo();
    Joueur* joueur1 = p.getJoueur1();
    Joueur* joueur2 = p.getJoueur2();
    Plateau& plateau = p.getPlateau();
    stack<Command*>& historique = p.getHistorique();

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
    fichier << "Nom: " << joueur1->getNom() << std::endl;
    fichier << "Couleur: " << joueur1->getCouleur() << std::endl;
    fichier << "Pions en main:" << std::endl;
    for (Pion* pion : joueur1->getPionsEnMain()) {
        fichier << "  ID: " << pion->getId() << "  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
    }

    fichier << "Joueur 2:" << std::endl;
    fichier << "Type: " << (dynamic_cast<JoueurHumain*>(joueur2) ? "Humain" : "IA") << std::endl;
    fichier << "Nom: " << joueur2->getNom() << std::endl;
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

std::vector<std::string> GestionnaireSauvegarde::listerSauvegardes() {
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