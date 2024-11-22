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

Joueur::Joueur(vector<Pion*> pEm, string c, Partie& p) : pionsEnMain(pEm), couleur(c), partie(&p){

}

void JoueurHumain::afficherPions(const std::vector<Pion*>& pions) {
    std::cout << "Pions disponibles en main : " << std::endl;
    for (size_t i = 0; i < pions.size(); ++i) {
        std::cout << i << ": " << pions[i]->getType() << std::endl;
    }
}

void JoueurHumain::afficherEmplacements(const std::list<Mouvement*>& emplacements) {
    std::cout << "Emplacements possibles : " << std::endl;
    int index = 0;
    for (Mouvement* m : emplacements) {
        std::cout << index++ << ": (" << m->getLigne() << ", " << m->getColonne() << ", " << m->getZ() << ")" << std::endl;
    }
}

int JoueurHumain::choisirPion(const std::vector<Pion*>& pions) {
    int choixPion;
    while (true) {
        std::cout << "Choisissez un pion a poser (numero) : ";
        std::cin >> choixPion;

        if (choixPion >= 0 && static_cast<size_t>(choixPion) < pions.size()) {
            return choixPion;
        }
        else {
            std::cout << "Choix de pion invalide. Veuillez reessayer." << std::endl;
        }
    }
}

int JoueurHumain::choisirEmplacement(const std::list<Mouvement*>& emplacements) {
    int choixEmplacement;
    while (true) {
        std::cout << "Choisissez un emplacement pour poser le pion (numero) : ";
        std::cin >> choixEmplacement;

        if (choixEmplacement >= 0 && static_cast<size_t>(choixEmplacement) < emplacements.size()) {
            return choixEmplacement;
        }
        else {
            std::cout << "Choix d'emplacement invalide. Veuillez reessayer." << std::endl;
        }
    }
}


Mouvement* JoueurHumain::Jouer(Plateau& plateau) {
    int choix;
    bool annulerMouvement = false;
    if (isMainVide()) {
        choix = 2; // Si la main est vide, le joueur ne peut que d placer un pion
    }
    else if (partie->canUndo() && peutBougerPions()) {
        std::cout << "Voulez-vous poser (1), deplacer (2) ou annuler un mouvement (" << partie->getNbUndo() << " restants) (3) ? ";
        std::cin >> choix;
    }
    else if (partie->canUndo()) {
        annulerMouvement = true;
        std::cout << "Voulez-vous poser (1) ou annuler un mouvement (" << partie->getNbUndo() << " restants) (2) ? ";
        std::cin >> choix;
    }
    else if (peutBougerPions()) {
        std::cout << "Voulez-vous poser (1) ou deplacer (2) ?";
        std::cin >> choix;
    }
    else {
        choix = 1; // Si le joueur ne peut pas bouger les pions, il doit poser un pion
    }

    if (choix == 1) {
        return poserPionHumain(plateau);  // Appel de la m thode pour poser un pion
    }
    else if (choix == 2 && !plateau.isVide() && !annulerMouvement) {
        return deplacerPionHumain(plateau);  // Appel de la m thode pour d placer un pion
    }
    else if (choix == 2 && !plateau.isVide() && partie->canUndo()) {
        partie->annulerMouvement();
        Jouer(plateau);
    }
    else if (choix == 3 && partie->canUndo()) {
        partie->annulerMouvement();
        Jouer(plateau);
    }
    else {
        std::cout << "Choix invalide." << std::endl;
        return nullptr; // Ajout d'un retour par d faut
    }
}

Mouvement* JoueurHumain::poserPionHumain(Plateau& plateau) {
    afficherPions(pionsEnMain);
    int choixPion = choisirPion(pionsEnMain);
    Pion* pionChoisi = pionsEnMain[choixPion];

    std::list<Mouvement*> emplacements = plateau.gestionnaireMouvements.emplacementsPossibles(*pionChoisi, plateau);
    if (emplacements.empty()) {
        std::cout << "Il n'existe aucun emplacement possible pour ce pion. Veuillez reessayer." << std::endl;
        return nullptr;
    }

    afficherEmplacements(emplacements);
    int choixEmplacement = choisirEmplacement(emplacements);

    auto it = emplacements.begin();
    std::advance(it, choixEmplacement);
    Mouvement* emplacementChoisi = *it;

    for (Mouvement* m : emplacements) {
        if (m != emplacementChoisi) {
            delete m;
        }
    }
    return emplacementChoisi;
}

void JoueurHumain::afficherPionsSurPlateau(const std::vector<std::tuple<Pion*, int, int, int>>& pionsSurPlateau){
    std::cout << "Pions pouvant etre deplaces sur le plateau : " << std::endl;

    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]);
        int ligne = std::get<1>(pionsSurPlateau[i]);
        int colonne = std::get<2>(pionsSurPlateau[i]);
        int z = std::get<3>(pionsSurPlateau[i]);
        
        std::cout << i << ": " << pion->getType() << " en (" << ligne << ", " << colonne << ", " << z << ")" << std::endl;
    }
}

int JoueurHumain::choisirPionSurPlateau(const std::vector<std::tuple<Pion*, int, int, int>>& pionsSurPlateau) {
    int choixPion;
    while (true) {
        std::cout << "Choisissez un pion a deplacer (numero) : ";
        std::cin >> choixPion;

        if (choixPion >= 0 && static_cast<size_t>(choixPion) < pionsSurPlateau.size()) {
            return choixPion;
        }
        else {
            std::cout << "Choix de pion invalide. Veuillez reessayer." << std::endl;
        }
    }
}

Mouvement* JoueurHumain::deplacerPionHumain(Plateau& plateau) {
    std::vector<std::tuple<Pion*, int, int, int>> pionsBougeable = plateau.gestionnaireMouvements.getPionsBougeables(plateau, *this);
    if (pionsBougeable.empty()) {
        std::cout << "Aucun pion peut etre bouge\n";
        return nullptr;
    }
    afficherPionsSurPlateau(pionsBougeable);

    int choixPion = choisirPionSurPlateau(pionsBougeable);
    Pion* pionChoisi = std::get<0>(pionsBougeable[choixPion]);
    std::list<Mouvement*> deplacementsValides = plateau.gestionnaireMouvements.deplacementPossibles(*pionChoisi, plateau);
    if (deplacementsValides.empty()) {
        std::cout << "Il n'existe aucun emplacement possible pour ce pion. Veuillez reessayer." << std::endl;
        return nullptr;
    }

    afficherEmplacements(deplacementsValides);
    int choixEmplacement = choisirEmplacement(deplacementsValides);

    auto it = deplacementsValides.begin();
    std::advance(it, choixEmplacement);
    Mouvement* emplacementChoisi = *it;

    for (Mouvement* m : deplacementsValides) {
        if (m != emplacementChoisi) {
            delete m;
        }
    }

    return emplacementChoisi;
}

bool Joueur::isMainVide() {
    if (pionsEnMain.size() == 0) return true;
    else return false;
}

Mouvement* JoueurIA::trouverMeilleurMouvement(Plateau& plateau, Joueur& joueurCourant, int profondeurMax) {
    int meilleurScore = -10000; // Valeur initiale très basse pour maximiser
    Mouvement* meilleurMouvement = nullptr;
    std::vector<Mouvement*> mouvementsPossibles = plateau.gestionnaireMouvements.genererTousLesMouvements(plateau, joueurCourant);
    for (Mouvement* mouvement : mouvementsPossibles) {

        // Appliquer le mouvement
        partie->appliquerMouvement(mouvement);

        // Évaluer avec minimax
        int score = minimax(plateau, profondeurMax - 1, partie->joueurAdverse(joueurCourant), false, -10000, 10000);

        // Annuler le mouvement
        partie->annulerUniqueMouvement(mouvement);

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
int JoueurIA::calculerBlocageAbeille(Plateau& plateau, Joueur& joueur, bool isMaximizingPlayer) {
    Pion* reineAdverse = nullptr;
    Pion* reineJoueur = nullptr;
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = plateau.gestionnairePions.getPions(plateau);

    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]);
        if (pion->getType() == "R") { // Si le pion est une reine
            if (pion->getCouleur() == joueur.getCouleur()) {
                reineJoueur = pion;
            }
            else {
                reineAdverse = pion;
            }

            // Sortie anticipée si les deux reines sont trouvées
            if (reineJoueur && reineAdverse) break;
        }
    }

    // Si une des reines manque, retourne un score neutre
    if (!reineJoueur || !reineAdverse) return 0;

    int score = 10 * plateau.gestionnaireVoisins.nombreVoisins(*reineAdverse, plateau) -
        10 * plateau.gestionnaireVoisins.nombreVoisins(*reineJoueur, plateau);

    return isMaximizingPlayer ? score : -score;
}

int JoueurIA::evaluerPartie(Plateau& plateau, Joueur& j, bool isMaximizingPlayer) {
    // Évalue l'état du plateau pour l'IA ou l'adversaire
    int score = 0;

    // Exemple : favorise l'encerclement de l'abeille ennemie
    if (partie->partieTerminee()) {
        Joueur* gagnant = partie->determinerGagnant();
        if (gagnant == &j) {
            return isMaximizingPlayer ? 1000 : -1000;
        }
        else {
            return isMaximizingPlayer ? -1000 : 1000;
        }
    }

    // Ajoute une heuristique basique :
    // Par exemple : favorise un plus grand nombre de pions bloquant
    score += calculerBlocageAbeille(plateau, j, isMaximizingPlayer);
    score += calculerScoreBlocage(plateau, j, isMaximizingPlayer);
    return score;
}

int JoueurIA::calculerScoreBlocage(Plateau& plateau, Joueur& joueur, bool isMaximizingPlayer) {
    int score = 0;

    // Récupère tous les pions sur le plateau
    std::vector<std::tuple<Pion*, int, int, int>> pionsBougeables = plateau.gestionnaireMouvements.getPionsBougeables(plateau, joueur);

    for (std::tuple<Pion*, int, int, int> pionBougeable : pionsBougeables) {
        Pion* pion = std::get<0>(pionBougeable);
        if (pion->getCouleur() == joueur.getCouleur()) {
            score += 3; // Pion du joueur bloqué
        }
        else {
            score -= 3; // Pion adverse bloqué
        }
    }

    return isMaximizingPlayer ? score : -score;
}

int JoueurIA::minimax(Plateau& plateau, int profondeur, Joueur& joueurCourant, bool isMaximizingPlayer, int alpha, int beta) {
    // Condition de base : profondeur atteinte ou fin de partie
    if (profondeur == 0 || partie->partieTerminee()) {
        return evaluerPartie(plateau, joueurCourant, isMaximizingPlayer);
    }
    int meilleurScore = isMaximizingPlayer ? -10000 : 10000;

    // Parcourir tous les mouvements possibles pour le joueur courant
    std::vector<Mouvement*> mouvementsPossibles = plateau.gestionnaireMouvements.genererTousLesMouvements(plateau, joueurCourant);

    for (Mouvement* mouvement : mouvementsPossibles) {

        // Appliquer le mouvement
        partie->appliquerMouvement(mouvement);

        // Appeler récursivement minimax
        int score = minimax(plateau, profondeur - 1, partie->joueurAdverse(joueurCourant), !isMaximizingPlayer, alpha, beta);

        // Annuler le mouvement
        partie->annulerUniqueMouvement(mouvement);

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