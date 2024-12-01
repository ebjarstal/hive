#include "joueur.h"
#include "plateau.h"
#include "partie.h"



Joueur::~Joueur() {
    // Libérer chaque pion pointé dans le vecteur pionsEnMain
    for (Pion* pion : pionsEnMain) {
        delete pion;
    }
    pionsEnMain.clear(); // Vide le vecteur après la suppression des pointeurs
}

bool Joueur::peutBougerPions() {
    for (Pion* pion : pionsEnMain) {
        // Si un pion de type "Reine" est trouve, on retourne false
        if (pion->getType() == "R") {
            return false;
        }
    }
    // Si aucun pion de type "Reine" n'a pas ete trouve, on retourne true
    return true;
}

bool Joueur::isMainVide() {
    if (pionsEnMain.size() == 0) return true;
    else return false;
}

Joueur::Joueur(vector<Pion*> pEm, string c, Partie& p) : pionsEnMain(pEm), couleur(c), partie(p) {

}

// -------------------------------- JOUEUR HUMAIN -------------------------------------

Pion* JoueurHumain::choisirPion(const std::vector<Pion*>& pions) {
    int choixPion;
    while (true) {
        std::cout << "Choisissez un pion a poser (numero) : ";
        std::cin >> choixPion;

        if (choixPion >= 0 && static_cast<size_t>(choixPion) < pions.size()) {
            return pions[choixPion];
        }
        else {
            std::cout << "Choix de pion invalide. Veuillez reessayer." << std::endl;
        }
    }
}

Pion* JoueurHumain::choisirPionEnMain() {
    int choixPion;
    while (true) {
        std::cout << "Choisissez un pion a poser (numero) : ";
        std::cin >> choixPion;

        if (choixPion >= 0 && static_cast<size_t>(choixPion) < pionsEnMain.size()) {
            return pionsEnMain[choixPion];
        }
        else {
            std::cout << "Choix de pion invalide. Veuillez reessayer." << std::endl;
        }
    }
}

Mouvement* JoueurHumain::choisirEmplacement(const std::vector<Mouvement*>& emplacements) {
    int choixEmplacement;
    while (true) {
        std::cout << "Choisissez un emplacement pour poser le pion (numero) : ";
        std::cin >> choixEmplacement;

        if (choixEmplacement >= 0 && static_cast<size_t>(choixEmplacement) < emplacements.size()) {
            return emplacements[choixEmplacement];
        }
        else {
            std::cout << "Choix d'emplacement invalide. Veuillez reessayer." << std::endl;
        }
    }
}

Pion* JoueurHumain::choisirPionSurPlateau(Plateau& plateau) {
    int choixPion;
    while (true) {
        std::cout << "Choisissez un pion a deplacer (numero) : ";
        std::cin >> choixPion;

        if (choixPion >= 0 && static_cast<size_t>(choixPion) < GestionnaireMouvements::getPionsBougeables(plateau, *this).size()) {
            return std::get<0>(GestionnaireMouvements::getPionsBougeables(plateau, *this)[choixPion]);
        }
        else {
            std::cout << "Choix de pion invalide. Veuillez reessayer." << std::endl;
        }
    }
}

void JoueurHumain::afficherPions(const std::vector<Pion*>& pions) {
    std::cout << "Pions disponibles en main : " << std::endl;
    for (size_t i = 0; i < pions.size(); ++i) {
        std::cout << i << ": " << pions[i]->getType() << std::endl;
    }
}

void JoueurHumain::afficherPionsEnMain() {
    std::cout << "Pions disponibles en main : " << std::endl;
    for (size_t i = 0; i < pionsEnMain.size(); ++i) {
        std::cout << i << ": " << pionsEnMain[i]->getType() << std::endl;
    }
}


void JoueurHumain::afficherEmplacements(const std::vector<Mouvement*>& emplacements) {
    std::cout << "Emplacements possibles : " << std::endl;
    int index = 0;
    for (Mouvement* m : emplacements) {
        std::cout << index++ << ": (" << m->getLigne() << ", " << m->getColonne() << ", " << m->getZ() << ")" << std::endl;
    }
}


void JoueurHumain::afficherPionsSurPlateau(Plateau& plateau) {
    std::cout << "Pions pouvant etre deplaces sur le plateau : " << std::endl;

    std::vector<std::tuple<Pion*,int,int,int>> pionsSurPlateau = GestionnaireMouvements::getPionsBougeables(plateau, *this);

    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]);
        int ligne = std::get<1>(pionsSurPlateau[i]);
        int colonne = std::get<2>(pionsSurPlateau[i]);
        int z = std::get<3>(pionsSurPlateau[i]);

        std::cout << i << ": " << pion->getType() << " en (" << ligne << ", " << colonne << ", " << z << ")" << std::endl;
    }
}

void JoueurIA::Jouer(Plateau& plateau) {
    Mouvement* mvt = trouverMeilleurMouvement(plateau, *this, 2);
    Command* commande = new MouvementCommand(partie, mvt);
    GestionnaireCommand::executeCommand(partie, commande);
}

void JoueurHumain::Jouer(Plateau& plateau) {

    int choix;
    if (isMainVide()) {
        choix = 2; // Si la main est vide, le joueur ne peut que d placer un pion
    }
    else if (partie.canUndo() && peutBougerPions()) {
        std::cout << "Voulez-vous poser (1), deplacer (2) ou annuler un mouvement (" << partie.getNbUndo() << " restants) (3) ? ";
        std::cin >> choix;
    }
    else if (partie.canUndo()) {
        std::cout << "Voulez-vous poser (1) ou annuler un mouvement (" << partie.getNbUndo() << " restants) (3) ? ";
        std::cin >> choix;
    }
    else if (peutBougerPions()) {
        std::cout << "Voulez-vous poser (1) ou deplacer (2) ? ";
        std::cin >> choix;
    }
    else {
        choix = 1; // Si le joueur ne peut pas bouger les pions, il doit poser un pion
    }

    if (choix == 1) {
        poserPionHumain(plateau);  // Appel de la m thode pour poser un pion
    }
    else if (choix == 2 && !plateau.isVide()) {
        deplacerPionHumain(plateau);  // Appel de la m thode pour d placer un pion
    }
    else if (choix == 3 && partie.canUndo()){
        partie.annulerMouvement();
        plateau.afficher();
        Jouer(plateau);
    }
    else {
        std::cout << "Choix invalide." << std::endl;
        return; // Ajout d'un retour par d faut
    }
}

Mouvement* JoueurHumain::poserPionHumain(Plateau& plateau) {

    afficherPions(pionsEnMain);
    Pion* pionChoisi = choisirPion(pionsEnMain);

    std::vector<Mouvement*> emplacements = pionChoisi->emplacementsPossibles(*pionChoisi, plateau);

    if (emplacements.empty()) {
        std::cout << "Il n'existe aucun emplacement possible pour ce pion. Veuillez reessayer." << std::endl;
        Jouer(plateau);
        return nullptr;
    }

    afficherEmplacements(emplacements);
    Mouvement* emplacementChoisi = choisirEmplacement(emplacements);

    auto poserPionCommand = new MouvementCommand(partie, emplacementChoisi);
    GestionnaireCommand::executeCommand(partie, poserPionCommand);


    for (Mouvement* m : emplacements) {
        if (m != emplacementChoisi) {
            delete m;
        }
    }
    return emplacementChoisi;
}

Mouvement* JoueurHumain::deplacerPionHumain(Plateau& plateau) {
    std::vector<std::tuple<Pion*, int, int, int>> pionsBougeable = GestionnaireMouvements::getPionsBougeables(plateau, *this);
    if (pionsBougeable.empty()) {
        std::cout << "Aucun pion peut etre bouge\n";
        return nullptr;
    }
    std::vector<Mouvement*> deplacementsValides;
    while (deplacementsValides.empty()) {

        afficherPionsSurPlateau(plateau);

        Pion* pionChoisi = choisirPionSurPlateau(plateau);

        deplacementsValides = GestionnaireMouvements::deplacementsPossibles(*pionChoisi, plateau);

        if (deplacementsValides.empty()) {
            std::cout << "Il n'existe aucun emplacement possible pour ce pion. Veuillez reessayer.\n" << std::endl;
        }
    }
    afficherEmplacements(deplacementsValides);
    Mouvement* deplacementChoisi = choisirEmplacement(deplacementsValides);

    auto deplacerPionCommand = new MouvementCommand(partie, deplacementChoisi);
    GestionnaireCommand::executeCommand(partie, deplacerPionCommand);

    for (Mouvement* m : deplacementsValides) {
        if (m != deplacementChoisi) {
            delete m;
        }
    }

    return deplacementChoisi;
}

Mouvement* JoueurIA::trouverMeilleurMouvement(Plateau& plateau, Joueur& joueurCourant, int profondeurMax) {
    int meilleurScore = -10000; // Valeur initiale très basse pour maximiser
    Mouvement* meilleurMouvement = nullptr;
    std::vector<Mouvement*> mouvementsPossibles = GestionnaireMouvements::genererTousLesMouvements(plateau, joueurCourant);
    for (Mouvement* mouvement : mouvementsPossibles) {

        Command* command = new MouvementCommand(partie, mouvement);
        GestionnaireCommand::executeCommand(partie, command);

        // Évaluer avec minimax
        int score = minimax(plateau, profondeurMax - 1, partie.joueurAdverse(joueurCourant), true, -10000, 10000);

        // Annuler le mouvement
        GestionnaireCommand::undoCommand(partie);

        // Vérifier si ce mouvement est le meilleur
        if (score > meilleurScore) {
            meilleurScore = score;
            if (meilleurMouvement) {
                delete meilleurMouvement; // Libérer l'ancien mouvement
            }
            meilleurMouvement = mouvement; // Sauvegarder le nouveau meilleur
        }
        else {
            delete mouvement; // Libérer les mouvements non retenus
        }
    }

    return meilleurMouvement;
}

int JoueurIA::evaluerPartie(Plateau& plateau, Joueur& joueur, bool isMaximizingPlayer) {
    int score = 0;

    // 1. Vérifier si la partie est terminée
    if (partie.partieTerminee()) {
        Joueur* gagnant = partie.determinerGagnant();
        if (gagnant == &joueur) {
            return isMaximizingPlayer ? 1000 : -1000; // Victoire ou défaite
        }
        else {
            return isMaximizingPlayer ? -1000 : 1000;
        }
    }

    // 2. Obtenir tous les pions sur le plateau
    const std::vector<std::tuple<Pion*, int, int, int>>& pionsSurPlateau = GestionnairePions::getPions(plateau);

    int optionsJoueur = 0, optionsAdversaire = 0;
    int voisinsReineJoueur = 0, voisinsReineAdverse = 0;
    Pion* reineJoueur = nullptr;
    Pion* reineAdverse = nullptr;

    for (const auto& pionTuple : pionsSurPlateau) {
        Pion* pion = std::get<0>(pionTuple);
        int ligne = std::get<1>(pionTuple);
        int colonne = std::get<2>(pionTuple);
        int z = std::get<3>(pionTuple);

        bool estJoueur = (pion->getCouleur() == joueur.getCouleur());

        // Identifier les reines
        if (pion->getType() == "R") {
            if (estJoueur) reineJoueur = pion;
            else reineAdverse = pion;
        }

        // Évaluer la mobilité et les types de pions
        if (!GestionnaireMouvements::cassageRuche(*pion, plateau)) {
            auto mouvements = pion->deplacementsPossibles(*pion, joueur, plateau);
            int options = mouvements.size();

            if (estJoueur) {
                optionsJoueur += options;

                // Bonus basé sur le type de pion
                score += (isMaximizingPlayer ? 1 : -1) * (pion->getType() == "Fourmi" ? 5
                    : pion->getType() == "Scarabée" ? 10
                    : pion->getType() == "Araignée" ? 3
                    : 0);
            }
            else {
                optionsAdversaire += options;

                // Pénalité basée sur le type de pion adverse
                score -= (isMaximizingPlayer ? 1 : -1) * (pion->getType() == "Fourmi" ? 5
                    : pion->getType() == "Scarabée" ? 10
                    : pion->getType() == "Araignée" ? 3
                    : 0);
            }
        }
    }

    // 3. Blocage des reines
    if (reineJoueur) {
        voisinsReineJoueur = GestionnaireVoisins::nombreVoisins(*reineJoueur, plateau);
    }

    if (reineAdverse) {
        voisinsReineAdverse = GestionnaireVoisins::nombreVoisins(*reineAdverse, plateau);
    }

    // Bonus ou pénalité pour le blocage des reines
    score += (isMaximizingPlayer ? 1 : -1) * ((10 - voisinsReineAdverse) * 10); // Bonus pour bloquer la reine adverse
    score -= (isMaximizingPlayer ? 1 : -1) * (voisinsReineJoueur * 10);         // Pénalité si la reine du joueur est bloquée

    // 4. Ajouter les scores de mobilité
    score += (isMaximizingPlayer ? 1 : -1) * (optionsJoueur * 2);
    score -= (isMaximizingPlayer ? 1 : -1) * (optionsAdversaire * 2);

    return score;
}

int JoueurIA::minimax(Plateau& plateau, int profondeur, Joueur& joueurCourant, bool isMaximizingPlayer, int alpha, int beta) {
    // Condition de base : profondeur atteinte ou fin de partie
    if (profondeur == 0 || partie.partieTerminee()) {
        return evaluerPartie(plateau, joueurCourant, isMaximizingPlayer);
    }
    int meilleurScore = isMaximizingPlayer ? -10000 : 10000;

    // Parcourir tous les mouvements possibles pour le joueur courant
    std::vector<Mouvement*> mouvementsPossibles = GestionnaireMouvements::genererTousLesMouvements(plateau, joueurCourant);

    for (Mouvement* mouvement : mouvementsPossibles) {

        // Appliquer le mouvement
        Command* command = new MouvementCommand(partie, mouvement);
        GestionnaireCommand::executeCommand(partie, command);

        // Appeler récursivement minimax
        int score = minimax(plateau, profondeur - 1, partie.joueurAdverse(joueurCourant), !isMaximizingPlayer, alpha, beta);

        // Annuler le mouvement
        GestionnaireCommand::undoCommand(partie);

        if (isMaximizingPlayer) {
            meilleurScore = std::max(meilleurScore, score);
            alpha = std::max(alpha, score);
        }
        else {
            meilleurScore = std::min(meilleurScore, score);
            beta = std::min(beta, score);
        }

        // Coupure alpha-beta
        if (beta <= alpha) {
            break;
        }
    }

    // Libérer la mémoire des mouvements générés
    for (Mouvement* mouvement : mouvementsPossibles) {
        delete mouvement;
    }

    return meilleurScore;
}