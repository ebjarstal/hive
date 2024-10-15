#include "../include/plateau.h"

 // Constructeur pour initialiser la grille avec les lignes et colonnes données
Plateau::Plateau(int lignes, int colonnes) : lignes(lignes), colonnes(colonnes) {
    grille.resize(lignes, std::vector<Pion*>(colonnes, nullptr)); // Initialiser la grille avec des nullptr
}

// Destructeur pour supprimer les objets Pion alloués dynamiquement
Plateau::~Plateau() {
    for (auto& ligne : grille) {
        for (auto& pion : ligne) {
            delete pion;
        }
    }
    std::cout << "Pions du plateau supprimes" << std::endl;
}

// Obtenir le Pion à la position spécifiée dans la grille
Pion* Plateau::getPion(int ligne, int colonne) const {
    if (estValide(ligne, colonne)) {
        return grille[ligne][colonne];
    }
    throw std::out_of_range("Position de grille invalide"); // Lancer une exception si la position est invalide
}

// Définir un Pion à la position spécifiée dans la grille
void Plateau::setPion(int ligne, int colonne, Pion* pion) {
    if (estValide(ligne, colonne)) {
        delete grille[ligne][colonne]; // Supprimer le Pion existant pour éviter une fuite de mémoire
        grille[ligne][colonne] = pion;
    } else {
        throw std::out_of_range("Position de grille invalide"); // Lancer une exception si la position est invalide
    }
}

// Supprimer le pion aux coordonnées spécifiées pour éviter une fuite de mémoire
void Plateau::deletePion(int ligne, int colonne) {
    if (estValide(ligne, colonne)) {
        delete grille[ligne][colonne];
        grille[ligne][colonne] = nullptr; // Remettre la case à nullptr
    } else {
        throw std::out_of_range("Position de grille invalide");
    }
}

// Obtenir un vecteur de Pion* contenant les voisins du pion aux coordonnées spécifiées. Sens horaire en commençant par en haut à droite.
std::vector<Pion*> Plateau::getVoisins(int ligne, int colonne) const {
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
        if (estValide(newLigne, newColonne)) {
            voisins.push_back(grille[newLigne][newColonne]);
        } else {
            voisins.push_back(nullptr);
        }
    }

    return voisins;
}

void Plateau::afficher() const {
    for (int ligne = 0; ligne < lignes; ++ligne) {
        // Afficher le numéro de ligne
        std::cout << ligne << " ";

        // Imprimer les espaces seulement pour une ligne sur deux
        if (ligne % 2 == 1) {
            std::cout << " ";
        }

        for (int colonne = 0; colonne < colonnes; ++colonne) {
            if (grille[ligne][colonne] == nullptr) {
                std::cout << ". ";
            } else {
                std::cout << grille[ligne][colonne]->getNom() << " ";
            }
        }
        std::cout << std::endl;
    }
}

bool Plateau::isVide() const {
    for (const auto& ligne : grille) {
        for (const auto& pion : ligne) {
            if (pion != nullptr) {
                return false;
            }
        }
    }
    return true;
}

bool Plateau::hasVoisin(int ligne, int colonne) const {
    auto voisins = getVoisins(ligne, colonne);
    for (const auto& voisin : voisins) {
        if (voisin != nullptr) {
            return true;
        }
    }
    return false;
}

bool Plateau::estValide(int ligne, int colonne) const {
    return ligne >= 0 && ligne < lignes && colonne >= 0 && colonne < colonnes;
}
