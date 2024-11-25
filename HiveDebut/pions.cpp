#include "pions.h"
#include "plateau.h"

std::map<int, Pion*> Pion::pions;
int Pion::prochainId = 1;

std::vector<Mouvement*> Sauterelle::deplacementsPossibles(Plateau& plateau) {
    std::vector<Mouvement*> deplacements;
    const int directions[6][2] = {
            {0, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, 0}, {-1, 1} // Hexagone
    };
    for (const auto& dir : directions) {
        int nligne = ligne, ncolonne = colonne;

        if ((GestionnairePions::getPion(nligne, ncolonne, plateau) != nullptr) && (plateau.estValide(nligne, ncolonne, 0))) {
            while ((GestionnairePions::getPion(nligne, ncolonne, plateau) != nullptr) && (plateau.estValide(nligne, ncolonne, 0))) {
                nligne += dir[0];
                ncolonne += dir[1];
            }
            deplacements.push_back(new Mouvement(this->id, nligne, ncolonne, 0, ligne, colonne, 0));
        }
    }

    return deplacements;
}