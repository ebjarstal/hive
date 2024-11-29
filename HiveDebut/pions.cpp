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

    // Récupérer les coordonnées des cases vides autour du pion Reine
    std::vector<std::tuple<int, int, int>> casesVidesCoords = GestionnaireVoisins::getCasesVidesAutour(*this, plateau);

    // Vérifier si le déplacement est valide (ne casse pas la ruche)
    for (std::tuple<int, int, int> caseVide : casesVidesCoords) {
        int ligne = std::get<0>(caseVide);
        int colonne = std::get<1>(caseVide);
        int z = std::get<2>(caseVide);

        if (!GestionnaireMouvements::deplacementCasseRuche(this, ligne, colonne, z, plateau)) {
            // Éviter les doublons
            if (emplacementsVisites.find({ ligne, colonne, z }) == emplacementsVisites.end()) {
                emplacementsVisites.insert({ ligne, colonne, z });
                mouvementsPossibles.push_back(new Mouvement(id, ligne, colonne, z, ligne, colonne, z));
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
    std::set<std::tuple<int, int, int>> emplacementsVisites;

    // Si le plateau est vide, aucun mouvement n'est possible
    if (plateau.isVide()) {
        return mouvementsPossibles;
    }

    // Déplacements comme une Reine 
    // Récupérer les voisins directs du Scarabée
    std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau);

    // Récupérer les cases vides accessibles autour de la position actuelle
    std::vector<std::tuple<int, int, int>> casesVidesCoords = GestionnaireVoisins::getCasesVidesAutour(*this, plateau);

    // Explorer les voisins pour identifier les cases vides valides
    for (const auto& voisin : voisinsCoords) {
        int voisinLigne = std::get<0>(voisin);
        int voisinColonne = std::get<1>(voisin);
        int voisinZ = std::get<2>(voisin);
        std::cout << voisinLigne << "," << voisinColonne << "," << voisinZ << "\n" << std::endl;
        // Récupérer les cases vides autour de ce voisin
        std::vector<std::tuple<int, int, int>> casesVidesAutourVoisin =
            GestionnaireVoisins::getCasesVidesAutour(voisinLigne, voisinColonne, voisinZ, plateau);

        // Filtrer les cases vides communes entre celles du Scarabée et les voisins
        for (const auto& caseVide : casesVidesCoords) {
            if (std::find(casesVidesAutourVoisin.begin(), casesVidesAutourVoisin.end(), caseVide) != casesVidesAutourVoisin.end()) {
                int caseLigne = std::get<0>(caseVide);
                int caseColonne = std::get<1>(caseVide);
                int caseZ = std::get<2>(caseVide);

                // Vérifier que le déplacement ne casse pas la ruche
                if (!GestionnaireMouvements::deplacementCasseRuche(this, caseLigne, caseColonne, caseZ, plateau)) {
                    // Ajouter le mouvement si ce n'est pas un doublon
                    if (emplacementsVisites.find({ caseLigne, caseColonne, caseZ }) == emplacementsVisites.end()) {
                        emplacementsVisites.insert({ caseLigne, caseColonne, caseZ });
                        mouvementsPossibles.push_back(
                            new Mouvement(this->getId(), caseLigne, caseColonne, caseZ, ligne, colonne, z)
                        );
                    }
                }
            }
        }
    }

    // Superposition sur les voisins 
    for (const auto& voisin : voisinsCoords) {
        int voisinLigne = std::get<0>(voisin);
        int voisinColonne = std::get<1>(voisin);
        int voisinZ = std::get<2>(voisin) + 1;  // Monter au-dessus du voisin, ajoute donc une unité à z

        // Vérifier que le déplacement ne casse pas la ruche
        //if (!GestionnaireMouvements::deplacementCasseRuche(this, voisinLigne, voisinColonne, voisinZ, plateau)) {
            // Ajouter le mouvement pour se superposer au voisin
            if (emplacementsVisites.find({ voisinLigne, voisinColonne, voisinZ }) == emplacementsVisites.end()) {
                emplacementsVisites.insert({ voisinLigne, voisinColonne, voisinZ });
                mouvementsPossibles.push_back(
                    new Mouvement(this->getId(), voisinLigne, voisinColonne, voisinZ, ligne, colonne, z)
                );
            
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
