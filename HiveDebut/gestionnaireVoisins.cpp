#include "gestionnaireVoisins.h"
#include "plateau.h"

// Obtenir un vecteur de Pion* contenant les voisins du pion aux coordonn es sp cifi es. Sens horaire en commen ant par en haut   droite.
std::vector<Pion*> GestionnaireVoisins::getVoisins(Pion& p, Plateau& plateau){
    int ligne = p.getLigne();
    int colonne = p.getColonne();
    int z = 0;
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
        z = 0;
        int newLigne = ligne + direction.first;
        int newColonne = colonne + direction.second;
        if (plateau.estValide(newLigne, newColonne, z)) {
            while (plateau.getGrille()[newLigne][newColonne][z + 1] != nullptr) {
                z++;
            }
            voisins.push_back(plateau.getGrille()[newLigne][newColonne][z]);
        }
        else {
            voisins.push_back(nullptr);
        }
    }

    return voisins;
}

// Surcharge de la m thode ci-dessus sans la structure pion
std::vector<Pion*> GestionnaireVoisins::getVoisins(int ligne, int colonne, Plateau& plateau, int z){
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
        z = 0;
        int newLigne = ligne + direction.first;
        int newColonne = colonne + direction.second;
        if(plateau.estValide(newLigne, newColonne, z)) {
            while (plateau.getGrille()[newLigne][newColonne][z + 1] != nullptr) {
                z++;
            }
            voisins.push_back(plateau.getGrille()[newLigne][newColonne][z]);
        }
        else {
            voisins.push_back(nullptr);
        }
    }

    return voisins;
}

std::vector<std::tuple<int, int, int>> GestionnaireVoisins::getVoisinsCoords(int ligne, int colonne, Plateau& plateau, int z){
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
        z = 0;
        int newLigne = ligne + direction.first;
        int newColonne = colonne + direction.second;
        if (plateau.estValide(newLigne, newColonne, z)) {
            while (plateau.getGrille()[newLigne][newColonne][z + 1] != nullptr) {
                z++;
            }
            voisinsCoords.push_back({ newLigne, newColonne, z });
        }
    }
    return voisinsCoords;
}

int GestionnaireVoisins::nombreVoisins(Pion& p, Plateau& plateau){
    std::vector<Pion*> voisins = getVoisins(p, plateau);
    int nbrVoisin = 0;
    for (auto voisin : voisins) {
        if (voisin) nbrVoisin++;
    }
    return nbrVoisin;
}

bool GestionnaireVoisins::hasVoisin(Pion& p, Plateau& plateau){
    return nombreVoisins(p, plateau) == 0;
}

bool GestionnaireVoisins::sontVoisin(Pion& p1, Pion& p2, Plateau& plateau){
    if (!GestionnairePions::estPose(p1) || !GestionnairePions::estPose(p2)) {
        std::cout << "Au moins l'un des deux pions n'est pas pose sur le plateau\n";
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
std::vector<Pion*> GestionnaireVoisins::getRuche(Pion* p, Plateau& plateau){
    std::deque<Pion*> tests; // Queue contenant les pions a tester
    Pion* test; // Pion a tester
    std::vector<Pion*> voisins = getVoisins(*p, plateau); // Voisins du pion a tester
    std::vector<Pion*> ruche; // Ruche finale

    // Initialisation de tests
    ruche.push_back(p);
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
    // Tri des ruche pour pouvoir ensuite les comparer
    std::sort(ruche.begin(), ruche.end());
    return ruche;
}

// Obtenir les coordon�es des cases vides autour d'un pion d'apr�s une r�f�rence
std::vector<std::tuple<int, int, int>> GestionnaireVoisins::getCasesVidesAutour(Pion& p, Plateau& plateau) {
    std::vector<std::tuple<int, int, int>> casesVides;

    int ligne = p.getLigne();
    int colonne = p.getColonne();

    // Liste des d�placements possibles pour un hexagone, d�pendant de la parit� de la ligne
    std::vector<std::tuple<int, int, int>> directions;

    if (ligne % 2 == 0) { // Ligne paire
        directions = {
            {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {1, -1, 0}, {-1, -1, 0}
        };
    }
    else { // Ligne impaire
        directions = {
            {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {1, 1, 0}, {-1, 1, 0}
        };
    }

    // Parcours des voisins autour du pion
    for (const auto& direction : directions) {
        int voisinLigne = ligne + std::get<0>(direction);
        int voisinColonne = colonne + std::get<1>(direction);

        // V�rifie si la case voisine est vide
        if (GestionnairePions::getPion(voisinLigne, voisinColonne, plateau) == nullptr) {
            casesVides.emplace_back(voisinLigne, voisinColonne, 0);
        }
    }
    return casesVides;
}

// Obtenir les coordon�es des cases vides autour d'un pion d'apr�s des coordon�es
std::vector<std::tuple<int, int, int>> GestionnaireVoisins::getCasesVidesAutour(int ligne, int colonne, int z, Plateau& plateau) {
    std::vector<std::tuple<int, int, int>> casesVides;

    // Liste des d�placements possibles pour un hexagone, d�pendant de la parit� de la ligne
    std::vector<std::tuple<int, int, int>> directions;

    if (ligne % 2 == 0) { // Ligne paire
        directions = {
            {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {1, -1, 0}, {-1, -1, 0}
        };
    }
    else { // Ligne impaire
        directions = {
            {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {1, 1, 0}, {-1, 1, 0}
        };
    }

    // Parcours des voisins autour du pion
    for (const auto& direction : directions) {
        int voisinLigne = ligne + std::get<0>(direction);
        int voisinColonne = colonne + std::get<1>(direction);

        // V�rifie si la case voisine est vide
        if (GestionnairePions::getPion(voisinLigne, voisinColonne, plateau, 0) == nullptr) {
            casesVides.emplace_back(voisinLigne, voisinColonne, 0);
        }
    }

    return casesVides;
}


