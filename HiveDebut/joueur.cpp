#include "joueur.h"
#include "plateau.h"

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

Joueur::Joueur(vector<Pion*> pEm, string c) : pionsEnMain(pEm), couleur(c) {

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

        if (choixPion >= 0 && static_cast<size_t>(choixPion) < GestionnaireMouvements::getPionsBougeables(plateau).size()) {
            return std::get<0>(GestionnaireMouvements::getPionsBougeables(plateau)[choixPion]);
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

    std::vector<std::tuple<Pion*,int,int,int>> pionsSurPlateau = GestionnaireMouvements::getPionsBougeables(plateau);

    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]);
        int ligne = std::get<1>(pionsSurPlateau[i]);
        int colonne = std::get<2>(pionsSurPlateau[i]);
        int z = std::get<3>(pionsSurPlateau[i]);

        std::cout << i << ": " << pion->getType() << " en (" << colonne << ", " << ligne << ", " << z << ")" << std::endl;
    }
}

void JoueurHumain::Jouer(Plateau& plateau, GestionnaireCommand& gC) {

    int choix;
    if (isMainVide()) {
        choix = 2; // Si la main est vide, le joueur ne peut que d placer un pion
    }
    else if (peutBougerPions()) {
        std::cout << "Voulez-vous poser (1) ou deplacer (2) ?";
        std::cin >> choix;
    }
    else {
        choix = 1; // Si le joueur ne peut pas bouger les pions, il doit poser un pion
    }

    if (choix == 1) {
        poserPionHumain(plateau, gC);  // Appel de la m thode pour poser un pion
    }
    else if (choix == 2 && !plateau.isVide()) {
        deplacerPionHumain(plateau, gC);  // Appel de la m thode pour d placer un pion
    }
    else {
        std::cout << "Choix invalide." << std::endl;
        return; // Ajout d'un retour par d faut
    }
}

Mouvement* JoueurHumain::poserPionHumain(Plateau& plateau, GestionnaireCommand& gC) {

    afficherPions(pionsEnMain);
    Pion* pionChoisi = choisirPion(pionsEnMain);

    std::vector<Mouvement*> emplacements = GestionnaireMouvements::emplacementsPossibles(*pionChoisi, plateau);
    if (emplacements.empty()) {
        std::cout << "Il n'existe aucun emplacement possible pour ce pion. Veuillez reessayer." << std::endl;
        return nullptr;
    }

    afficherEmplacements(emplacements);
    Mouvement* emplacementChoisi = choisirEmplacement(emplacements);

    Pion* pionAPoser = new Pion(pionChoisi->getId(), pionChoisi->getType(), pionChoisi->getCouleur());
    auto poserPionCommand = new PoserPionCommand(*this, plateau, pionAPoser, emplacementChoisi);
    gC.executeCommand(std::move(poserPionCommand));

    for (Mouvement* m : emplacements) {
        if (m != emplacementChoisi) {
            delete m;
        }
    }
    return emplacementChoisi;
}

Mouvement* JoueurHumain::deplacerPionHumain(Plateau& plateau, GestionnaireCommand& gC) {
    std::vector<std::tuple<Pion*, int, int, int>> pionsBougeable = GestionnaireMouvements::getPionsBougeables(plateau);
    if (pionsBougeable.empty()) {
        std::cout << "Aucun pion peut etre bouge\n";
        return nullptr;
    }
    afficherPionsSurPlateau(plateau);

    Pion* pionChoisi = choisirPionSurPlateau(plateau);

    std::vector<Mouvement*> emplacements = GestionnaireMouvements::emplacementsPossibles(*pionChoisi, plateau);
    std::vector<Mouvement*> deplacementsValides = GestionnaireMouvements::filtrerDeplacementsValides(emplacements, pionChoisi, plateau);

    if (deplacementsValides.empty()) {
        std::cout << "Il n'existe aucun emplacement possible pour ce pion. Veuillez reessayer." << std::endl;
        return nullptr;
    }

    afficherEmplacements(deplacementsValides);
    Mouvement* emplacementChoisi = choisirEmplacement(deplacementsValides);

    auto deplacerPionCommand = new DeplacerPionCommand(*this, plateau, pionChoisi, emplacementChoisi);
    gC.executeCommand(std::move(deplacerPionCommand));

    for (Mouvement* m : emplacements) {
        if (m != emplacementChoisi) {
            delete m;
        }
    }

    return emplacementChoisi;
}
