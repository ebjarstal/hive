#include "gestionnaireVoisins.h"
#include "plateau.h"

// Obtenir un vecteur de Pion* contenant les voisins du pion aux coordonn es sp cifi es. Sens horaire en commen ant par en haut   droite.
std::vector<Pion*> GestionnaireVoisins::getVoisins(Pion& p, Plateau& plateau) const {
    int ligne = p.getLigne();
    int colonne = p.getColonne();
    int z = p.getZ();
    std::vector<Pion*> voisins;
    // Directions si la ligne est paire
    static const std::vector<std::pair<int, int>> directions_pair = {
        {-1, -1}, {-1, 0}, {0, 1}, {1, 0}, {1, -1}, {0, -1}
    };
    // Directions si la ligne est impaire
    static const std::vector<std::pair<int, int>> directions_impair = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {0, -1}
    };

    const auto& directions = (ligne % 2 == 0) ? directions_pair : directions_impair;

    for (const auto& direction : directions) {
        int newLigne = ligne + direction.first;
        int newColonne = colonne + direction.second;
        if (plateau.estValide(newLigne, newColonne, z)) {
            voisins.push_back(plateau.grille[newLigne][newColonne][z]);
        }
        else {
            voisins.push_back(nullptr);
        }
    }

    return voisins;
}

// Surcharge de la m thode ci-dessus sans la structure pion
std::vector<Pion*> GestionnaireVoisins::getVoisins(int ligne, int colonne, Plateau& plateau, int z) const {
    std::vector<Pion*> voisins;
    // Directions si la ligne est paire
    static const std::vector<std::pair<int, int>> directions_pair = {
        {-1, -1}, {-1, 0}, {0, 1}, {1, 0}, {1, -1}, {0, -1}
    };
    // Directions si la ligne est impaire
    static const std::vector<std::pair<int, int>> directions_impair = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {0, -1}
    };

    const auto& directions = (ligne % 2 == 0) ? directions_pair : directions_impair;

    for (const auto& direction : directions) {
        int newLigne = ligne + direction.first;
        int newColonne = colonne + direction.second;
        if (plateau.estValide(newLigne, newColonne, z)) {
            voisins.push_back(plateau.grille[newLigne][newColonne][z]);
        }
        else {
            voisins.push_back(nullptr);
        }
    }

    return voisins;
}

std::vector<std::tuple<int, int, int>> GestionnaireVoisins::getVoisinsCoords(int ligne, int colonne, Plateau& plateau, int z) const {
    std::vector<std::tuple<int, int, int>> voisinsCoords;

    // Directions pour les lignes paires
    static const std::vector<std::pair<int, int>> directions_pair = {
        {-1, -1}, {-1, 0}, {0, 1}, {1, 0}, {1, -1}, {0, -1}
    };
    // Directions pour les lignes impaires
    static const std::vector<std::pair<int, int>> directions_impair = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {0, -1}
    };

    // Choisir les directions en fonction de la parit  de la ligne
    const auto& directions = (ligne % 2 == 0) ? directions_pair : directions_impair;

    // Parcourir les directions pour obtenir les coordonn es des voisins
    for (const auto& direction : directions) {
        int newLigne = ligne + direction.first;
        int newColonne = colonne + direction.second;

        // V rifier si la position voisine est dans les limites
        if (plateau.estValide(newLigne, newColonne, z)) {
            voisinsCoords.push_back({ newLigne, newColonne, z });
        }
    }

    return voisinsCoords;
}

int GestionnaireVoisins::nombreVoisins(Pion& p, Plateau& plateau) const {
    std::vector<Pion*> voisins = getVoisins(p, plateau);
    int nbrVoisin = 0;
    for (auto voisin : voisins) {
        if (voisin) nbrVoisin++;
    }
    return nbrVoisin;
}

bool GestionnaireVoisins::hasVoisin(Pion& p, Plateau& plateau) const {
    return nombreVoisins(p, plateau) == 0;
}

bool GestionnaireVoisins::sontVoisin(Pion& p1, Pion& p2, Plateau& plateau) const {
    if (!plateau.gestionnairePions.estPose(p1) || !plateau.gestionnairePions.estPose(p2)) {
        std::cout << "Au moins l un des deux pions n est pas pose sur le plateau\n";
        return false;
    }
    std::vector<Pion*> voisins = getVoisins(p1, plateau); // On prend tous les voisins de P1
    for (Pion* voisin : voisins) {
        // compare chaque voisin de p1 avec p2
        if (voisin &&
            voisin->getLigne() == p2.getLigne() &&
            voisin->getColonne() == p2.getColonne() &&
            voisin->getZ() == p2.getZ()) {
            return true;
        }
    }
    return false;
}

// Obtenir l'enti ret  de la ruche (voisins des voisins des voisins ... de (ligne, colonne))
std::vector<Pion*> GestionnaireVoisins::getRuche(Pion* p, Plateau& plateau) const {
    std::deque<Pion*> tests; // Queue contenant les pion   tester
    Pion* test; // Pion   tester
    std::vector<Pion*> voisins = getVoisins(*p, plateau); // Voisins du pion   tester
    std::vector<Pion*> ruche; // Ruche finale

    // Initialisation de tests
    for (Pion* voisin : voisins) {
        if (voisin) {
            tests.push_back(voisin);
            ruche.push_back(voisin);
        }
    }

    // Test de tous les pions
    while (!(tests.empty())) {  // Tant que la queue n'est pas vide
        test = tests.front();
        tests.pop_front();
        voisins = getVoisins(*test, plateau); // On r cup re les voisins de test

        // Pour chaque voisin, on le met dans la ruche (et dans tests) s'il ne l'est pas d j 
        for (Pion* voisin : voisins) {
            if (voisin) {
                if ((count(ruche.begin(), ruche.end(), voisin) == 0) && (count(tests.begin(), tests.end(), voisin) == 0)) {
                    ruche.push_back(voisin);
                    tests.push_back(voisin);
                }
            }
        }
    }
    return ruche;
}