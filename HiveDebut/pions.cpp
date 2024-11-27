#include "pions.h"
#include "plateau.h"

std::map<int, Pion*> Pion::pions;
int Pion::prochainId = 1;

std::vector<Mouvement*> Reine::deplacementsPossibles(Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;

    // Si le plateau est vide, aucun mouvement possible
    if (plateau.isVide()) {
        return mouvementsPossibles;
    }

    // Récupérer les coordonnées des voisins du pion Reine
    std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

    // Récupérer les coordonnées des cases vides autour du pion Reine
    std::vector<std::tuple<int, int, int>> casesVidesCoords = GestionnaireVoisins::getCasesVidesAutour(*this, plateau);

    // Parcourir les voisins pour récupérer leurs cases vides
    for (const auto& voisinCoord : voisinsCoords) {
        int v_ligne = std::get<0>(voisinCoord);
        int v_colonne = std::get<1>(voisinCoord);
        int v_z = std::get<2>(voisinCoord);

        // Récupérer les coordonnées des cases vides autour des voisins
        std::vector<std::tuple<int, int, int>> casesVidesCoordsVoisin = GestionnaireVoisins::getCasesVidesAutour(v_ligne, v_colonne, v_z, plateau);

        // Trouver les cases communes entre les cases vides autour de la Reine et celles des voisins
        for (const auto& caseVide : casesVidesCoords) {
            if (std::find(casesVidesCoordsVoisin.begin(), casesVidesCoordsVoisin.end(), caseVide) != casesVidesCoordsVoisin.end()) { // std::find() Retourne casesVidesCoordsVoisin.end() si l'élément caseVide n'est pas dans casesVidesCoordsVoisin
                int c_ligne = std::get<0>(caseVide);
                int c_colonne = std::get<1>(caseVide);
                int c_z = std::get<2>(caseVide);

                // Vérifier si le déplacement est valide (ne casse pas la ruche)
                if (!GestionnaireMouvements::deplacementCasseRuche(this, c_ligne, c_colonne, c_z, plateau)) {
                    // Éviter les doublons
                    if (emplacementsVisites.find({ c_ligne, c_colonne, c_z }) == emplacementsVisites.end()) {
                        emplacementsVisites.insert({ c_ligne, c_colonne, c_z });
                        mouvementsPossibles.push_back(new Mouvement(id, c_ligne, c_colonne, c_z, ligne, colonne, z));
                    }
                }
            }
        }
    }

    return mouvementsPossibles;
}

std::vector<Mouvement*> Sauterelle::deplacementsPossibles(Plateau& plateau) {
    std::vector<Mouvement*> deplacements;

    // Directions si la ligne est paire
    const int directions[6][4] = {
        {-1, -1, -1, 0}, {-1, 0, -1, 1}, {0, 1, 0, 1}, {1, 0, 1, 1}, {1, -1, 1, 0}, {0, -1, 0, -1}
    };

    for (const auto& dir : directions) {
        int nligne, ncolonne;
        if (ligne % 2 == 0) {
            nligne = ligne + dir[0], ncolonne = colonne + dir[1];
        }
        else {
            nligne = ligne + dir[2], ncolonne = colonne + dir[3];
        }

        if ((plateau.estValide(nligne, ncolonne, 0)) && (GestionnairePions::getPion(nligne, ncolonne, plateau) != nullptr)) {
            while ((plateau.estValide(nligne, ncolonne, 0)) && (GestionnairePions::getPion(nligne, ncolonne, plateau) != nullptr)) {
                if (nligne % 2 == 0) {
                    nligne += dir[0];
                    ncolonne += dir[1];
                }
                else {
                    nligne += dir[2];
                    ncolonne += dir[3];
                }
            }
            deplacements.push_back(new Mouvement(id, nligne, ncolonne, 0, ligne, colonne, 0));
        }
    }

    return deplacements;
}

std::vector<Mouvement*> Araignee::deplacementsPossibles(Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons

    if (plateau.isVide()) {
        return mouvementsPossibles;
    }
    else {
        // Récupérer tous les pions présents sur le plateau
        std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = GestionnairePions::getPions(plateau);

        for (const auto& pionTuple : pionsSurPlateau) {
            Pion* pionActuel = std::get<0>(pionTuple);
            int ligne = std::get<1>(pionTuple);
            int colonne = std::get<2>(pionTuple);
            int z = std::get<3>(pionTuple);

            std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

            for (const auto& voisinCoord : voisinsCoords) {
                int v_ligne = std::get<0>(voisinCoord);
                int v_colonne = std::get<1>(voisinCoord);
                int v_z = std::get<2>(voisinCoord);

                int distCarree = (v_ligne - ligne) * (v_ligne - ligne) +
                    (v_colonne - colonne) * (v_colonne - colonne) +
                    (v_z - z) * (v_z - z);

                // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
                if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && distCarree == 9 && !GestionnaireMouvements::deplacementCasseRuche(this, v_ligne, v_colonne, v_z, plateau)) {
                    // Vérifier si cet emplacement a déjà été visité
                    if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                        // Ajouter l'emplacement au set pour éviter les doublons
                        emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                        // Ajouter le mouvement à la liste des mouvements possibles
                        mouvementsPossibles.push_back(new Mouvement(id, v_ligne, v_colonne, v_z, ligne, colonne, z));
                    }
                }
            }
        }
    }

    return mouvementsPossibles;
}


std::vector<Mouvement*> Fourmi::deplacementsPossibles(Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = GestionnairePions::getPions(plateau);

    for (const auto& pionTuple : pionsSurPlateau) {
        Pion* pionActuel = std::get<0>(pionTuple);
        int ligne = std::get<1>(pionTuple);
        int colonne = std::get<2>(pionTuple);
        int z = std::get<3>(pionTuple);

        std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

        for (const auto& voisinCoord : voisinsCoords) {
            int v_ligne = std::get<0>(voisinCoord);
            int v_colonne = std::get<1>(voisinCoord);
            int v_z = std::get<2>(voisinCoord);

            // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
            if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && !GestionnaireMouvements::deplacementCasseRuche(this, v_ligne, v_colonne, v_z, plateau)) {
                // Vérifier si cet emplacement a déjà été visité
                if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                    // Ajouter l'emplacement au set pour éviter les doublons
                    emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                    // Ajouter le mouvement à la liste des mouvements possibles
                    mouvementsPossibles.push_back(new Mouvement(id, v_ligne, v_colonne, v_z, ligne, colonne, z));
                }
            }
        }
    }
    return mouvementsPossibles;
}

std::vector<Mouvement*> Scarabee::deplacementsPossibles(Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = GestionnairePions::getPions(plateau);

    for (const auto& pionTuple : pionsSurPlateau) {
        Pion* pionActuel = std::get<0>(pionTuple);
        int ligne = std::get<1>(pionTuple);
        int colonne = std::get<2>(pionTuple);
        int z = std::get<3>(pionTuple);

        std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

        for (const auto& voisinCoord : voisinsCoords) {
            int v_ligne = std::get<0>(voisinCoord);
            int v_colonne = std::get<1>(voisinCoord);
            int v_z = std::get<2>(voisinCoord);

            // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
            if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && !GestionnaireMouvements::deplacementCasseRuche(this, v_ligne, v_colonne, v_z, plateau)) {
                // Vérifier si cet emplacement a déjà été visité
                if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                    // Ajouter l'emplacement au set pour éviter les doublons
                    emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                    // Ajouter le mouvement à la liste des mouvements possibles
                    mouvementsPossibles.push_back(new Mouvement(id, v_ligne, v_colonne, v_z, ligne, colonne, z));
                }
            }
        }
    }
    return mouvementsPossibles;
}

std::vector<Mouvement*> Coccinelle::deplacementsPossibles(Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons
    std::vector<Pion*> voisins = GestionnaireVoisins::getVoisins(*this, plateau);
    

    for (Pion* pion : voisins) {
        if (pion) {
            for (Pion* pion_voisin : GestionnaireVoisins::getVoisins(*pion, plateau)) {
                if (pion_voisin && (pion_voisin != this)) {
                    for (std::tuple<int, int, int> case_vide : GestionnaireVoisins::getCasesVidesAutour(*pion_voisin, plateau)) {
                        if (emplacementsVisites.find(case_vide) == emplacementsVisites.end()) {
                            emplacementsVisites.insert(case_vide);
                            int new_l = std::get<0>(case_vide);
                            int new_c = std::get<1>(case_vide);
                            int new_z = std::get<2>(case_vide);
                            Mouvement* mouv = new Mouvement(id, new_l, new_c, new_z, ligne, colonne, z);
                            mouvementsPossibles.push_back(mouv);
                        }
                    }
                }
            }
        }
    }
    return mouvementsPossibles;
}

std::vector<Mouvement*> Cloporte::deplacementsPossibles(Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = GestionnairePions::getPions(plateau);

    for (const auto& pionTuple : pionsSurPlateau) {
        Pion* pionActuel = std::get<0>(pionTuple);
        int ligne = std::get<1>(pionTuple);
        int colonne = std::get<2>(pionTuple);
        int z = std::get<3>(pionTuple);

        std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

        for (const auto& voisinCoord : voisinsCoords) {
            int v_ligne = std::get<0>(voisinCoord);
            int v_colonne = std::get<1>(voisinCoord);
            int v_z = std::get<2>(voisinCoord);

            // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
            if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && !GestionnaireMouvements::deplacementCasseRuche(this, v_ligne, v_colonne, v_z, plateau)) {
                // Vérifier si cet emplacement a déjà été visité
                if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                    // Ajouter l'emplacement au set pour éviter les doublons
                    emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                    // Ajouter le mouvement à la liste des mouvements possibles
                    mouvementsPossibles.push_back(new Mouvement(id, v_ligne, v_colonne, v_z, ligne, colonne, z));
                }
            }
        }
    }
    return mouvementsPossibles;
}

std::vector<Mouvement*> Moustique::deplacementsPossibles(Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = GestionnairePions::getPions(plateau);

    for (const auto& pionTuple : pionsSurPlateau) {
        Pion* pionActuel = std::get<0>(pionTuple);
        int ligne = std::get<1>(pionTuple);
        int colonne = std::get<2>(pionTuple);
        int z = std::get<3>(pionTuple);

        std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

        for (const auto& voisinCoord : voisinsCoords) {
            int v_ligne = std::get<0>(voisinCoord);
            int v_colonne = std::get<1>(voisinCoord);
            int v_z = std::get<2>(voisinCoord);

            // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
            if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && !GestionnaireMouvements::deplacementCasseRuche(this, v_ligne, v_colonne, v_z, plateau)) {
                // Vérifier si cet emplacement a déjà été visité
                if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                    // Ajouter l'emplacement au set pour éviter les doublons
                    emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                    // Ajouter le mouvement à la liste des mouvements possibles
                    mouvementsPossibles.push_back(new Mouvement(id, v_ligne, v_colonne, v_z, ligne, colonne, z));
                }
            }
        }
    }
    return mouvementsPossibles;
}
