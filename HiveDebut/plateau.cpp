#include "plateau.h"

// Constructeur pour initialiser la grille avec les lignes et colonnes donn es
Plateau::Plateau(unsigned int l, unsigned int c, unsigned int z, GestionnairePions& gP, GestionnaireMouvements& gM, GestionnaireVoisins& gV) : nb_lignes(l), nb_colonnes(c), nb_couches(z), gestionnairePions(gP), gestionnaireMouvements(gM), gestionnaireVoisins(gV) {
    grille.resize(l, std::vector<std::vector<Pion*>>(c, std::vector<Pion*>(z, nullptr))); // Initialiser la grille avec des nullptr
    std::cout << "Plateau créé avec " << l << " lignes, " << c << " colonnes et " << z << " couches." << std::endl;
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

void Plateau::afficher() const {
    // Calculer la largeur maximale n cessaire pour le num ro de ligne
    size_t largeurLigne = std::to_string(nb_lignes - 1).size();

    for (unsigned int ligne = 0; ligne < nb_lignes; ++ligne) {
        // Afficher le num ro de ligne avec un espace r serv 
        std::cout << std::setw(largeurLigne) << ligne << " ";

        // Imprimer les espaces seulement pour une ligne sur deux
        if (ligne % 2 == 1) {
            std::cout << " ";
        }

        for (unsigned int colonne = 0; colonne < nb_colonnes; ++colonne) {
            if (grille[ligne][colonne][0] == nullptr) {
                std::cout << RESET << ". ";
            }
            else {
                std::cout << gestionnairePions.getPion(ligne, colonne, *this)->getCouleur()
                    << grille[ligne][colonne][0]->getType() << " ";
            }
        }
        std::cout << std::endl;
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
