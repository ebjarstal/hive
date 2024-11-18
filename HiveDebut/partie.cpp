#include "partie.h"

Partie::~Partie() {
    delete joueur1;  // Lib�re la m�moire allou�e pour joueur1
    delete joueur2;  // Lib�re la m�moire allou�e pour joueur2

    // Lib�re chaque mouvement dans l'historique
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
        std::cout << "Dossier cr�� avec succ�s : " << cheminDossier << std::endl;
    }
    else if (errno == EEXIST) {
        std::cout << "Le dossier existe d�j� : " << cheminDossier << std::endl;
    }
    else {
        std::cerr << "Erreur : Impossible de cr�er le dossier " << cheminDossier << std::endl;
    }
}

void Partie::setup() {
    // Cr�er le dossier "sauvegarde" s'il n'existe pas
    const std::string dossierSauvegarde = "sauvegardes";
    creerDossierSiInexistant(dossierSauvegarde);

    int choix = 0;

    // Demander � l'utilisateur s'il souhaite charger une partie ou en commencer une nouvelle
    std::cout << "Voulez-vous (1) Charger une partie ou (2) Commencer une nouvelle partie ? ";
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
                std::cout << "Choisissez une sauvegarde � charger (1-" << fichiersSauvegarde.size() << ") : ";
                std::cin >> choixSauvegarde;
            }

            nomPartie = fichiersSauvegarde[choixSauvegarde - 1];
            if (chargerPartie()) {
                std::cout << "Partie charg�e avec succ�s !" << std::endl;
                return;
            }
            else {
                std::cout << "Erreur lors du chargement de la partie. Cr�ation d'une nouvelle partie." << std::endl;
            }
        }
    }

    // Cr�ation d'une nouvelle partie
    std::cout << "Entrez un nom pour la nouvelle partie : ";
    std::cin >> nomPartie;

    // Ajouter le nom dans le fichier de liste
    std::ofstream listeFichiers(dossierSauvegarde + "/liste_sauvegardes.txt", std::ios::app);
    if (listeFichiers) {
        listeFichiers << nomPartie << std::endl;
    }

    std::cout << "Nouvelle partie creee : " << nomPartie << std::endl;

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
    pions.push_back(new Pion("R", couleur));
    pions.push_back(new Pion("S", couleur));
    pions.push_back(new Pion("S", couleur));
    pions.push_back(new Pion("S", couleur));
    pions.push_back(new Pion("F", couleur));
    pions.push_back(new Pion("F", couleur));
    pions.push_back(new Pion("F", couleur));
    pions.push_back(new Pion("C", couleur));
    pions.push_back(new Pion("C", couleur));
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
    std::getline(fichier, ligne); // Joueur 1
    std::getline(fichier, ligne); // Type de joueur
    std::string typeJ1 = ligne.substr(ligne.find(":") + 2);

    std::getline(fichier, ligne); // Couleur joueur 1
    std::string couleurJ1 = ligne.substr(ligne.find(":") + 2);

    std::vector<Pion*> pionsJ1;
    std::getline(fichier, ligne); // Pions en main joueur 1
    while (std::getline(fichier, ligne) && ligne.find("Type") != std::string::npos) {
        // Extraire le type du pion
        std::string typePart = ligne.substr(ligne.find(":") + 2);
        size_t posVirgule = typePart.find(",");
        std::string type = typePart.substr(0, posVirgule);

        // Extraire la couleur du pion
        std::string couleur = typePart.substr(posVirgule + 11); // Sauter ", Couleur: "
        pionsJ1.push_back(new Pion(type, couleur));
    }

    if (typeJ1 == "Humain")
        joueur1 = new JoueurHumain(pionsJ1, couleurJ1);
    else
        joueur1 = new JoueurIA(pionsJ1, couleurJ1);

    // Charger les informations du joueur 2
    std::getline(fichier, ligne); // Type de joueur
    std::string typeJ2 = ligne.substr(ligne.find(":") + 2);

    std::getline(fichier, ligne); // Couleur joueur 2
    std::string couleurJ2 = ligne.substr(ligne.find(":") + 2);

    std::vector<Pion*> pionsJ2;
    std::getline(fichier, ligne); // Pions en main joueur 2
    while (std::getline(fichier, ligne) && ligne.find("Type") != std::string::npos) {
        // Extraire le type du pion
        std::string typePart = ligne.substr(ligne.find(":") + 2);
        size_t posVirgule = typePart.find(",");
        std::string type = typePart.substr(0, posVirgule);

        // Extraire la couleur du pion
        std::string couleur = typePart.substr(posVirgule + 11); // Sauter ", Couleur: "
        pionsJ2.push_back(new Pion(type, couleur));
    }

    if (typeJ2 == "Humain")
        joueur2 = new JoueurHumain(pionsJ2, couleurJ2);
    else
        joueur2 = new JoueurIA(pionsJ2, couleurJ2);

    // Charger l'�tat du plateau
    while (std::getline(fichier, ligne) && ligne.find("Position") != std::string::npos) {
        // Extraire les coordonn�es
        size_t pos1 = ligne.find("(") + 1;
        size_t pos2 = ligne.find(",");
        int colonne = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(",", pos1);
        int lignePlateau = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(")", pos1);
        int couche = std::stoi(ligne.substr(pos1, pos2 - pos1));

        // Extraire le type et la couleur
        size_t posType = ligne.find("Type: ") + 6;
        size_t posCouleur = ligne.find(", Couleur: ");
        std::string type = ligne.substr(posType, posCouleur - posType);
        std::string couleur = ligne.substr(posCouleur + 11);
        Pion* pion = new Pion(type, couleur);
        plateau.gestionnairePions.setPion(lignePlateau, colonne, couche, pion, plateau);
    }

    // Charger l'historique des mouvements
    while (std::getline(fichier, ligne) && ligne.find("De") != std::string::npos) {
        // Extraire les anciennes coordonn�es
        size_t pos1 = ligne.find("(") + 1;
        size_t pos2 = ligne.find(",");
        int oldColonne = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(",", pos1);
        int oldLigne = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(")", pos1);
        int oldZ = std::stoi(ligne.substr(pos1, pos2 - pos1));

        // Extraire les nouvelles coordonn�es
        pos1 = ligne.find("(", pos2) + 1;
        pos2 = ligne.find(",", pos1);
        int newColonne = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(",", pos1);
        int newLigne = std::stoi(ligne.substr(pos1, pos2 - pos1));

        pos1 = pos2 + 2;
        pos2 = ligne.find(")", pos1);
        int newZ = std::stoi(ligne.substr(pos1, pos2 - pos1));

        Mouvement* mvt = new Mouvement(oldColonne, oldLigne, oldZ, newColonne, newLigne, newZ);
        historique.push(mvt);
    }

    fichier.close();
    return true;
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
    std::string nomFichier = "sauvegardes/" + nomPartie + ".txt"; // Utiliser nomPartie pour le fichier
    std::ofstream fichier(nomFichier);
    if (!fichier) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier de sauvegarde." << std::endl;
        return;
    }

    // Sauvegarder les donn�es comme avant...
    fichier << "Nombre de tour: " << nombreTour << std::endl;
    fichier << "Nombre d'undo: " << nbUndo << std::endl;

    fichier << "Joueur 1:" << std::endl;
    fichier << "Type: " << (dynamic_cast<JoueurHumain*>(joueur1) ? "Humain" : "IA") << std::endl;
    fichier << "Couleur: " << joueur1->getCouleur() << std::endl;
    fichier << "Pions en main:" << std::endl;
    for (Pion* pion : joueur1->pionsEnMain) {
        fichier << "  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
    }

    fichier << "Joueur 2:" << std::endl;
    fichier << "Type: " << (dynamic_cast<JoueurHumain*>(joueur2) ? "Humain" : "IA") << std::endl;
    fichier << "Couleur: " << joueur2->getCouleur() << std::endl;
    fichier << "Pions en main:" << std::endl;
    for (Pion* pion : joueur2->pionsEnMain) {
        fichier << "  Type: " << pion->getType() << ", Couleur: " << pion->getCouleur() << std::endl;
    }

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

    fichier << "Historique des mouvements:" << std::endl;
    std::stack<Mouvement*> tempHistorique = historique; // Copie pour ne pas d�truire l'historique
    while (!tempHistorique.empty()) {
        Mouvement* mvt = tempHistorique.top();
        tempHistorique.pop();
        fichier << "  De (" << mvt->getOldColonne() << ", " << mvt->getOldLigne() << ", " << mvt->getOldZ() << ") ";
        fichier << "  (" << mvt->getColonne() << ", " << mvt->getLigne() << ", " << mvt->getZ() << ")" << std::endl;
    }

    fichier.close();
    std::cout << "Sauvegarde de la partie r�ussie dans '" << nomFichier << "'." << std::endl;
}


/*
void Partie::ajouterExtension(unsigned int id)
{
    Extension::getNom();
    Extention.id.active = !Extention.active;
    Extension::compteur += 1;
}
*/