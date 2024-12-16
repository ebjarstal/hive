#include "partie.h"

Partie::~Partie() {
    // Si les joueurs ont été créés dynamiquement ailleurs, vérifiez et libérez la mémoire.
    if (joueur1) {
        delete joueur1;
        joueur1 = nullptr;
    }
    if (joueur2) {
        delete joueur2;
        joueur2 = nullptr;
    }

    // Nettoyage de l'historique des commandes (pile de Commandes).
    while (!historique.empty()) {
        delete historique.top();  // Libère chaque commande.
        historique.pop();         // Retire l'élément de la pile.
    }
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


    int nbUndoCin = -1;
    while (nbUndoCin < 0) {
        std::cout << "Entrez le nombre de retour en arriere : ";
        std::cin >> nbUndoCin;

        if (nbUndoCin < 0) {
            std::cout << "Erreur : Vous devez entrer un nombre positif." << std::endl;
        }
    }

    UsineDePions usine;
    usine = choixExtension(usine);


    std::cout << "Nombre de joueurs selectionne : " << nbJoueur << std::endl;

    if (nbJoueur == 1) {
        joueur1 = new JoueurHumain(nomJoueur1, initialiserPions(RED, usine), RED, nbUndoCin, *this);
        joueur2 = new JoueurIA(nomJoueur2, initialiserPions(WHITE, usine), WHITE, *this, nbUndoCin);
    }
    else {
        joueur1 = new JoueurHumain(nomJoueur1, initialiserPions(RED, usine), RED, nbUndoCin, *this);
        joueur2 = new JoueurHumain(nomJoueur2, initialiserPions(WHITE, usine), WHITE, nbUndoCin, *this);
    }
}

UsineDePions Partie::choixExtension(UsineDePions usine) {
    std::cout << "Voici les extensions disponible, veuillez choisir si vous voulez les activer ou non:\n";
    std::cout << "(Pour plus d'info, bah dommage)\n";
    for (const auto& pair : usine.getNombreDePions()) {
        const std::string& type = pair.first;              // Clé (type)
        const std::pair<unsigned int, bool>& data = pair.second; // Valeur (nombre et état)
        if (type != "R" && type != "K" && type != "F" && type != "S" && type != "A") {
            int choix;
            std::cout << "Type : " << type << "\nVoulez-vous l'activer ? (1 pour oui, 0 pour non)\n";
            std::cin >> choix;
            if (choix == 1) {
                usine.setExtensionActive(type);
            }
        }
    }
    return usine;
}

std::vector<Pion*> Partie::initialiserPions(const std::string& couleur, UsineDePions usine) {
    std::vector<Pion*> pions;

    for (const auto& pair : usine.getNombreDePions()) {
        const std::string& type = pair.first;              // Clé (type)
        const std::pair<unsigned int, bool>& data = pair.second; // Valeur (nombre et état)

        int nbpion = data.first;
        while (nbpion > 0 && data.second) {
            Pion* pionQuelconque = usine.creerPion(type, couleur);
            Pion::ajouterPion(pionQuelconque);
            pions.push_back(pionQuelconque);
            nbpion--;
        }
    }

    return pions;
}

void Partie::jouerUnTour(Joueur* j) {
    // Le joueur joue son tour
    j->Jouer(getPlateau(),*this);  // Joue le mouvement

    if (j == getJoueur2()) {
        nombreTour += 1;
    }

    // Verification si la partie est terminee
    if (Partie::partieTerminee() == true) {
        if (getJoueur1() == Partie::determinerGagnant()) {
            std::cout << getJoueur1()->getNom() << " gagne !! La partie s'est finie en " << Partie::getNombreTour() << " tours." << std::endl;
        }
        else if (getJoueur2() == Partie::determinerGagnant()) {
            std::cout << getJoueur2()->getNom() << " gagne !! La partie s'est finie en " << Partie::getNombreTour() << " tours." << std::endl;
        }
        else {
            std::cout << "Egalite !! La partie s'est finie en " << Partie::getNombreTour() << " tours." << std::endl;
        }
    }
}

void Partie::annulerMouvement(Joueur& j) {
    if (historique.size() >= 2) {
        GestionnaireCommand::undoCommand(*this);
        GestionnaireCommand::undoCommand(*this);
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

Joueur* Partie::determinerGagnant() const {
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = GestionnairePions::getPions(plateau);
    std::vector<std::string> couleursReines; // Ensemble des couleurs des reines avec 6 voisins.

    // Collecte des couleurs de toutes les reines avec exactement 6 voisins.
    for (const auto& tuple : pionsSurPlateau) {
        Pion* pion = std::get<0>(tuple);
        if (pion->getType() == "R" && GestionnaireVoisins::nombreVoisins(*pion, plateau) == 6) {
            couleursReines.push_back(pion->getCouleur());
        }
    }

    // Analyse des couleurs trouvées.
    if (couleursReines.size() == 1) {
        // Si une seule couleur de reine a été trouvée.
        std::string couleurReine = *couleursReines.begin();

        if (joueur1 && joueur1->getCouleur() != couleurReine) {
            return joueur1;
        }
        if (joueur2 && joueur2->getCouleur() != couleurReine) {
            return joueur2;
        }
    }
    else if (couleursReines.size() > 1) {
        // Si au moins deux couleurs différentes de reines sont trouvées, égalité.
        return nullptr;
    }

    // Aucun gagnant déterminé (pas de reine avec 6 voisins ou aucune couleur différente trouvée).
    return nullptr;
}
