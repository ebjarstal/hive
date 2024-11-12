#include "joueur.h"
#include "plateau.h"

Joueur::~Joueur() {
    // Lib�rer chaque pion point� dans le vecteur pionsEnMain
    for (Pion* pion : pionsEnMain) {
        delete pion;
    }
    pionsEnMain.clear(); // Vide le vecteur apr�s la suppression des pointeurs
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

Joueur::Joueur(vector<Pion*> pEm, string c) : pionsEnMain(pEm), couleur(c) {

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
        std::cout << index++ << ": (" << m->getColonne() << ", " << m->getLigne() << ", " << m->getZ() << ")" << std::endl;
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
    if (isMainVide()) {
        choix = 2; // Si la main est vide, le joueur ne peut que d placer un pion
    }
    else if (peutBougerPions()) {
        std::cout << "Voulez-vous poser (1) ou deplacer (2) un pion ? ";
        std::cin >> choix;
    }
    else {
        choix = 1; // Si le joueur ne peut pas bouger les pions, il doit poser un pion
    }

    if (choix == 1) {
        return poserPionHumain(plateau);  // Appel de la m thode pour poser un pion
    }
    else if (choix == 2 && !plateau.isVide()) {
        return deplacerPionHumain(plateau);  // Appel de la m thode pour d placer un pion
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

    Pion* pionAPoser = new Pion(pionChoisi->getType(), pionChoisi->getCouleur());
    plateau.gestionnairePions.setPion(emplacementChoisi->getLigne(), emplacementChoisi->getColonne(), emplacementChoisi->getZ(), pionAPoser, plateau);

    pionsEnMain.erase(pionsEnMain.begin() + choixPion);

    std::cout << "Pion pose en (" << emplacementChoisi->getColonne() << ", " << emplacementChoisi->getLigne() << ", " << emplacementChoisi->getZ() << ")." << std::endl;

    for (Mouvement* m : emplacements) {
        if (m != emplacementChoisi) {
            delete m;
        }
    }

    return emplacementChoisi;
}

void JoueurHumain::afficherPionsSurPlateau(const std::vector<std::tuple<Pion*, int, int, int>>& pionsSurPlateau) {
    std::cout << "Pions disponibles sur le plateau : " << std::endl;
    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]);
        int ligne = std::get<1>(pionsSurPlateau[i]);
        int colonne = std::get<2>(pionsSurPlateau[i]);
        int z = std::get<3>(pionsSurPlateau[i]);

        std::cout << i << ": " << pion->getType() << " en (" << colonne << ", " << ligne << ", " << z << ")" << std::endl;
    }
}

bool JoueurHumain::deplacementCasseRuche(Pion* pion, int newLigne, int newColonne, int newZ, Plateau& plateau) {
    std::vector<Pion*> rucheAvant = plateau.gestionnaireVoisins.getRuche(pion, plateau);

    // D placer temporairement le pion
    int oldLigne = pion->getLigne();
    int oldColonne = pion->getColonne();
    int oldZ = pion->getZ();
    plateau.gestionnairePions.deletePion(*pion, plateau);
    plateau.gestionnairePions.setPion(newLigne, newColonne, newZ, pion, plateau);

    std::vector<Pion*> rucheApres = plateau.gestionnaireVoisins.getRuche(pion, plateau);

    // Remettre le pion   sa position initiale
    plateau.gestionnairePions.deletePion(*pion, plateau);
    plateau.gestionnairePions.setPion(oldLigne, oldColonne, oldZ, pion, plateau);

    return rucheAvant.size() != rucheApres.size();
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

std::list<Mouvement*> JoueurHumain::filtrerEmplacementsValides(const std::list<Mouvement*>& emplacements, Pion* pion, Plateau& plateau) {
    std::list<Mouvement*> emplacementsValides;
    for (Mouvement* m : emplacements) {
        if (!deplacementCasseRuche(pion, m->getLigne(), m->getColonne(), m->getZ(), plateau)) {
            emplacementsValides.push_back(m);
        }
    }
    return emplacementsValides;
}

Mouvement* JoueurHumain::deplacerPionHumain(Plateau& plateau) {
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = plateau.gestionnairePions.getPions(plateau);
    afficherPionsSurPlateau(pionsSurPlateau);

    int choixPion = choisirPionSurPlateau(pionsSurPlateau);
    Pion* pionChoisi = std::get<0>(pionsSurPlateau[choixPion]);

    std::list<Mouvement*> emplacements = plateau.gestionnaireMouvements.emplacementsPossibles(*pionChoisi, plateau);
    std::list<Mouvement*> emplacementsValides = filtrerEmplacementsValides(emplacements, pionChoisi, plateau);

    if (emplacementsValides.empty()) {
        std::cout << "Il n'existe aucun emplacement possible pour ce pion. Veuillez reessayer." << std::endl;
        return nullptr;
    }

    afficherEmplacements(emplacementsValides);
    int choixEmplacement = choisirEmplacement(emplacementsValides);

    auto it = emplacementsValides.begin();
    std::advance(it, choixEmplacement);
    Mouvement* emplacementChoisi = *it;

    plateau.gestionnairePions.movePion(emplacementChoisi->getLigne(), emplacementChoisi->getColonne(), emplacementChoisi->getZ(), pionChoisi, plateau);

    for (Mouvement* m : emplacements) {
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