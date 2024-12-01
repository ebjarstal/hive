#include "pions.h"
#include "plateau.h"
#include "joueur.h"

std::map<int, Pion*> Pion::pions;
int Pion::prochainId = 1;

std::vector<Mouvement*> Pion::emplacementsPossibles(Pion& p, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsUniques;

    // Vérifier si le plateau est vide
    if (plateau.isVide()) {
        // Si le plateau est vide, ajouter seulement le centre comme emplacement possible
        emplacementsUniques.insert({ (plateau.getNbLignes() - 1) / 2, (plateau.getNbColonnes() - 1) / 2, 0 });
    }
    else {
        // Récupérer tous les pions présents sur le plateau
        std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = GestionnairePions::getPions(plateau);

        // Vérifier si c'est le deuxième tour (un seul pion sur le plateau)
        if (pionsSurPlateau.size() == 1) {
            // Ajouter tous les voisins du pion existant comme emplacements possibles
            int ligne = std::get<1>(pionsSurPlateau[0]);
            int colonne = std::get<2>(pionsSurPlateau[0]);
            int z = std::get<3>(pionsSurPlateau[0]);

            std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);
            for (const auto& voisinCoord : voisinsCoords) {
                int v_ligne = std::get<0>(voisinCoord);
                int v_colonne = std::get<1>(voisinCoord);
                int v_z = std::get<2>(voisinCoord);

                if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr) {
                    emplacementsUniques.insert({ v_ligne, v_colonne, v_z });
                }
            }
        }
        else {
            // Parcourir tous les pions sur le plateau
            for (const auto& pionTuple : pionsSurPlateau) {
                Pion* pionActuel = std::get<0>(pionTuple);
                int ligne = std::get<1>(pionTuple);
                int colonne = std::get<2>(pionTuple);
                int z = std::get<3>(pionTuple);

                // Vérifier que le pion est de la même couleur que le pion courant
                if (pionActuel->getCouleur() == p.getCouleur()) {
                    // Obtenir les coordonnées des cases voisines de ce pion
                    std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

                    for (const auto& voisinCoord : voisinsCoords) {
                        int v_ligne = std::get<0>(voisinCoord);
                        int v_colonne = std::get<1>(voisinCoord);
                        int v_z = std::get<2>(voisinCoord);

                        // Vérifier si la case voisine est vide
                        if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr) {
                            // Vérifier que cette case n'est pas voisine d'un pion adverse
                            std::vector<std::tuple<int, int, int>> voisinsAdversaires =
                                GestionnaireVoisins::getVoisinsCoords(v_ligne, v_colonne, plateau, v_z);

                            bool voisinAdverse = false;
                            for (const auto& voisinAdverseCoord : voisinsAdversaires) {
                                int adv_ligne = std::get<0>(voisinAdverseCoord);
                                int adv_colonne = std::get<1>(voisinAdverseCoord);
                                int adv_z = std::get<2>(voisinAdverseCoord);

                                Pion* pionVoisin = GestionnairePions::getPion(adv_ligne, adv_colonne, plateau, adv_z);
                                if (pionVoisin != nullptr && pionVoisin->getCouleur() != p.getCouleur()) {
                                    voisinAdverse = true;
                                    break;
                                }
                            }

                            // Si ce n'est pas un voisin d'un pion adverse, ajouter l'emplacement
                            if (!voisinAdverse) {
                                emplacementsUniques.insert({ v_ligne, v_colonne, v_z });
                            }
                        }
                    }
                }
            }
        }
    }
    // Convertir les emplacements uniques en objets Mouvement et les ajouter à la liste des mouvements possibles
    for (const auto& emplacement : emplacementsUniques) {
        int ligne = std::get<0>(emplacement);
        int colonne = std::get<1>(emplacement);
        int z = std::get<2>(emplacement);
        mouvementsPossibles.push_back(new Mouvement(p.getId(), ligne, colonne, z, -1, -1, -1));
    }

    return mouvementsPossibles;
}

std::vector<Mouvement*> Reine::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;

    // Si le plateau est vide, aucun mouvement possible
    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
        return mouvementsPossibles;
    }
    
    // Récupérer les coordonnées des cases vides autour du pion Reine
    std::vector<std::tuple<int, int, int>> casesVidesCoords = GestionnaireVoisins::getCasesVidesAutour(p, plateau);

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

std::vector<Mouvement*> Sauterelle::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> deplacements;

    if (j.getCouleur() != p.getCouleur()) {
        return deplacements;
    }

    int l = p.getLigne();
    int c = p.getColonne();
    int z = p.getZ();

    // Directions si la ligne est paire
    const int directions[6][4] = {
        {-1, -1, -1, 0}, {-1, 0, -1, 1}, {0, 1, 0, 1}, {1, 0, 1, 1}, {1, -1, 1, 0}, {0, -1, 0, -1}
    };

    for (const auto& dir : directions) {
        int nligne, ncolonne;
        if (ligne % 2 == 0) {
            nligne = l + dir[0], ncolonne = c + dir[1];
        }
        else {
            nligne = l + dir[2], ncolonne = c + dir[3];
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
            deplacements.push_back(new Mouvement(id, nligne, ncolonne, 0, l, c, 0));
        }
    }

    return deplacements;
}

std::vector<Mouvement*> Araignee::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;

    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
        return mouvementsPossibles;
    }
    else {

        // Fonction interne pour explorer récursivement 3 déplacements
        std::function<void(int, int, int, int, int, int, int)> explorerDeplacements =
            [&](int ligne, int colonne, int z, int profondeur, int   ligneInitiale, int colonneInitiale, int zInitial) {
            if (profondeur == 3) {
                // Si on a atteint 3 déplacements, on enregistre ce mouvement
                mouvementsPossibles.push_back(new Mouvement(getId(), ligne, colonne, z, ligneInitiale, colonneInitiale, zInitial));
                return;
            }

            // Récupérer les voisins accessibles
            std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

            for (const auto& voisinCoord : voisinsCoords) {
                int v_ligne = std::get<0>(voisinCoord);
                int v_colonne = std::get<1>(voisinCoord);
                int v_z = std::get<2>(voisinCoord);

                // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
                if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr &&
                    !GestionnaireMouvements::deplacementCasseRuche(this, v_ligne, v_colonne, v_z, plateau)) {
                    // Vérifier si cet emplacement n’a pas déjà été visité dans cette exploration
                    if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                        // Ajouter l'emplacement au set des emplacements visités
                        emplacementsVisites.insert({ v_ligne, v_colonne, v_z });

                        // Explorer récursivement pour le prochain déplacement
                        explorerDeplacements(v_ligne, v_colonne, v_z, profondeur + 1, ligneInitiale, colonneInitiale, zInitial);

                        // Nettoyer cet emplacement du set pour les autres chemins possibles
                        emplacementsVisites.erase({ v_ligne, v_colonne, v_z });
                    }
                }
            }
            };

        // Initialiser l'exploration depuis la position de l'araignée
        int ligne = p.getLigne();
        int colonne = p.getColonne();
        int z = p.getZ();

        // Récupérer les voisins initiaux
        std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

        for (const auto& voisinCoord : voisinsCoords) {
            int v_ligne = std::get<0>(voisinCoord);
            int v_colonne = std::get<1>(voisinCoord);
            int v_z = std::get<2>(voisinCoord);

            // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
            if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr &&
                !GestionnaireMouvements::deplacementCasseRuche(this, v_ligne, v_colonne, v_z, plateau)) {
                // Ajouter au set des emplacements visités
                emplacementsVisites.insert({ v_ligne, v_colonne, v_z });

                // Explorer récursivement pour 3 déplacements
                explorerDeplacements(v_ligne, v_colonne, v_z, 1, ligne, colonne, z);

                // Nettoyer cet emplacement du set après exploration
                emplacementsVisites.erase({ v_ligne, v_colonne, v_z });
            }
        }
    }

    return mouvementsPossibles;
}


std::vector<Mouvement*> Fourmi::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons

    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
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

                // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
                if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && !GestionnaireMouvements::deplacementCasseRuche(&p, v_ligne, v_colonne, v_z, plateau)) {
                    // Vérifier si cet emplacement a déjà été visité
                    if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                        // Ajouter l'emplacement au set pour éviter les doublons
                        emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                        // Ajouter le mouvement à la liste des mouvements possibles
                        mouvementsPossibles.push_back(new Mouvement(p.getId(), v_ligne, v_colonne, v_z, p.getLigne(), p.getColonne(), p.getZ()));
                    }
                }
            }
        }
        return mouvementsPossibles;
    }
}

std::vector<Mouvement*> Scarabee::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons

    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
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

                // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
                if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && !GestionnaireMouvements::deplacementCasseRuche(&p, v_ligne, v_colonne, v_z, plateau)) {
                    // Vérifier si cet emplacement a déjà été visité
                    if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                        // Ajouter l'emplacement au set pour éviter les doublons
                        emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                        // Ajouter le mouvement à la liste des mouvements possibles
                        mouvementsPossibles.push_back(new Mouvement(p.getId(), v_ligne, v_colonne, v_z, p.getLigne(), p.getColonne(), p.getZ()));
                    }
                }
            }
        }
        return mouvementsPossibles;
    }
}

std::vector<Mouvement*> Coccinelle::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
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

std::vector<Mouvement*> Cloporte::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons

    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
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

                // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
                if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && !GestionnaireMouvements::deplacementCasseRuche(&p, v_ligne, v_colonne, v_z, plateau)) {
                    // Vérifier si cet emplacement a déjà été visité
                    if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                        // Ajouter l'emplacement au set pour éviter les doublons
                        emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                        // Ajouter le mouvement à la liste des mouvements possibles
                        mouvementsPossibles.push_back(new Mouvement(p.getId(), v_ligne, v_colonne, v_z, p.getLigne(), p.getColonne(), p.getZ()));
                    }
                }
            }
        }
        return mouvementsPossibles;
    }
}

std::vector<Mouvement*> Moustique::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons

    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
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

                // Vérifier si la case voisine est vide et que le déplacement ne casse pas la ruche
                if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && !GestionnaireMouvements::deplacementCasseRuche(&p, v_ligne, v_colonne, v_z, plateau)) {
                    // Vérifier si cet emplacement a déjà été visité
                    if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                        // Ajouter l'emplacement au set pour éviter les doublons
                        emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                        // Ajouter le mouvement à la liste des mouvements possibles
                        mouvementsPossibles.push_back(new Mouvement(p.getId(), v_ligne, v_colonne, v_z, p.getLigne(), p.getColonne(), p.getZ()));
                    }
                }
            }
        }
        return mouvementsPossibles;
    }
}
