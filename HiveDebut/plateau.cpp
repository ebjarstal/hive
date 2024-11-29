#include "plateau.h"

// Constructeur pour initialiser la grille avec les lignes et colonnes donn es
Plateau::Plateau(unsigned int l, unsigned int c, unsigned int z) : nb_lignes(l), nb_colonnes(c), nb_couches(z) {
    grille.resize(l, std::vector<std::vector<Pion*>>(c, std::vector<Pion*>(z, nullptr))); // Initialiser la grille avec des nullptr
    std::cout << "Plateau cree avec " << l << " lignes, " << c << " colonnes et " << z << " couches." << std::endl;
}

// Destructeur pour supprimer les objets Pion alloues dynamiquement
Plateau::~Plateau() {
    for (auto& couche : grille) {
        for (auto& ligne : couche) {
            for (Pion* pion : ligne) {
                delete pion;
            }
        }
    }
    std::cout << "Pions du plateau supprimes" << std::endl;
}

void Plateau::afficher() {
    // Calculer la largeur maximale nécessaire pour le numéro de ligne
    size_t largeurLigne = std::to_string(nb_lignes - 1).size();

    // Stocker les informations des cases avec plusieurs pions (pile)
    std::vector<std::string> casesEmpilees;

    for (unsigned int ligne = 0; ligne < nb_lignes; ++ligne) {
        // Afficher le numéro de ligne avec un espace réservé
        std::cout << std::setw(largeurLigne) << ligne << " ";

        // Imprimer les espaces seulement pour une ligne sur deux
        if (ligne % 2 == 1) {
            std::cout << " ";
        }

        for (unsigned int colonne = 0; colonne < nb_colonnes; ++colonne) {
            if (grille[ligne][colonne][0] == nullptr) {
                // Si la case est vide (aucun pion), afficher un point
                std::cout << RESET << ". ";
            }
            else {
                int z = 0;
                std::vector<std::string> pionsEmpiles;

                // Parcourir tous les niveaux z de cette case
                while (grille[ligne][colonne][z] != nullptr) {
                    Pion* pion = grille[ligne][colonne][z];
                    if (pion != nullptr) {
                        // Ajouter le type et la couleur du pion dans la pile
                        pionsEmpiles.push_back(
                            pion->getCouleur() + pion->getType()
                        );
                    }
                    ++z;
                }

                // Afficher uniquement le pion situé au niveau z le plus élevé (dernier dans le vecteur)
                if (!pionsEmpiles.empty()) {
                    std::cout << pionsEmpiles.back() << " "; // Pion au sommet
                }

                // Ajouter les autres pions (recouverts) dans la légende
                if (pionsEmpiles.size() > 1) {
                    std::ostringstream details;
                    details << "Case (" << ligne << ", " << colonne << "): ";
                    for (size_t i = 0; i < pionsEmpiles.size() - 1; ++i) {
                        details << pionsEmpiles[i] << " ";
                    }
                    casesEmpilees.push_back(details.str());
                }
            }
        }
        std::cout << std::endl;
    }

    // Afficher la légende des cases avec des pions empilés
    if (!casesEmpilees.empty()) {
        std::cout << "\nPions empiles :\n";
        for (const auto& ligne : casesEmpilees) {
            std::cout << ligne << RESET << std::endl;
        }
    }
}



bool Plateau::isVide() const {
    for (const auto& ligne : grille) {
        for (const auto& colonne : ligne) {
            for (const auto& pion : colonne) {
                if (pion != nullptr) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Plateau::redimensionnerPlateau() {
    // D finir les nouvelles dimensions en doublant la taille actuelle
    unsigned int newNbLignes = nb_lignes * 2;
    unsigned int newNbColonnes = nb_colonnes * 2;

    // Cr er une nouvelle grille de dimensions doubl es, remplie de nullptr
    std::vector<std::vector<std::vector<Pion*>>> nouvelleGrille(newNbLignes, std::vector<std::vector<Pion*>>(newNbColonnes, std::vector<Pion*>(nb_couches, nullptr)));

    // Calculer l'offset pour recentrer l'ancien plateau dans le nouveau
    int offsetLigne = (newNbLignes - nb_lignes) / 2;
    int offsetColonne = (newNbColonnes - nb_colonnes) / 2;

    // Copier les pions existants dans la nouvelle grille, recentr s
    for (unsigned int i = 0; i < nb_lignes; ++i) {
        for (unsigned int j = 0; j < nb_colonnes; ++j) {
            for (unsigned int k = 0; k < nb_couches; ++k) {
                if (grille[i][j][k] != nullptr) {
                    int newI = i + offsetLigne;
                    int newJ = j + offsetColonne;
                    nouvelleGrille[newI][newJ][k] = grille[i][j][k];

                    // Mettre   jour les coordonn es du pion pour qu'elles soient correctes dans la nouvelle grille
                    grille[i][j][k]->setLigne(newI);
                    grille[i][j][k]->setColonne(newJ);
                    grille[i][j][k]->setZ(k);
                }
            }
        }
    }

    // Remplacer l'ancienne grille par la nouvelle
    grille = std::move(nouvelleGrille);

    // Mettre   jour les dimensions du plateau
    nb_lignes = newNbLignes;
    nb_colonnes = newNbColonnes;
}

bool Plateau::estValide(int ligne, int colonne, int z) const {
    return ligne >= 0 && static_cast<unsigned int>(ligne) < nb_lignes && colonne >= 0 && static_cast<unsigned int>(colonne) < nb_colonnes && z >= 0;
}
