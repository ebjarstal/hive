#include "pions.h"
#include "plateau.h"
#include "joueur.h"
#include <unordered_set>

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
            // Parcourir tous les pions sur le plateau qui sont pas empilés
            for (const auto& pionTuple : pionsSurPlateau) {
                Pion* pionActuel = std::get<0>(pionTuple);
                int ligne = std::get<1>(pionTuple);
                int colonne = std::get<2>(pionTuple);
                int z = std::get<3>(pionTuple);
                if (plateau.getGrille()[ligne][colonne][z+1]==nullptr) {
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

std::vector<Mouvement*> Pion::deplacementsPossibles(Joueur& j, Plateau& plateau) {
    return this->deplacementsPossibles(*this, j, plateau);
}

std::vector<Mouvement*> Reine::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;

    // Si le plateau est vide, aucun mouvement n'est possible
    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
        return mouvementsPossibles;
    }

    // Récupérer les cases vides autour du pion communes à ses voisins directs
    std::vector<std::tuple<int, int, int>> casesVidesCoords = GestionnaireVoisins::getCasesVidesCommunes(p, plateau);

    for (const auto& caseVide : casesVidesCoords) {
        int new_ligne = std::get<0>(caseVide);
        int new_colonne = std::get<1>(caseVide);
        int new_z = std::get<2>(caseVide);
        // Traiter chaque case vide commune

    // Vérifier que le passage est ouvert ET que le déplacement est valide (sans survoler un trou)
        if (!GestionnaireMouvements::deplacementCasseRuche(&p, new_ligne, new_colonne, new_z, plateau)) {

            // Éviter les doublons
            if (emplacementsVisites.find({ new_ligne, new_colonne, new_z }) == emplacementsVisites.end()) {
                emplacementsVisites.insert({ new_ligne, new_colonne, new_z });
                mouvementsPossibles.push_back(new Mouvement(p.getId(), new_ligne, new_colonne, new_z, p.getLigne(), p.getColonne(), p.getZ()));
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
            deplacements.push_back(new Mouvement(p.getId(), nligne, ncolonne, 0, p.getLigne(), p.getColonne(), 0));
        }
    }

    return deplacements;
}

std::vector<Mouvement*> Araignee::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites; // Set pour éviter les doublons

    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
        return mouvementsPossibles;
    }

    // Fonction interne pour explorer récursivement jusqu'à 3 déplacements
    std::function<void(int, int, int, int)> explorerDeplacements = [&](int ligne, int colonne, int z, int profondeur) {
        if (profondeur == 3) { // Si on a atteint 3 déplacements, ajouter le mouvement et retourner
            mouvementsPossibles.push_back(new Mouvement(p.getId(), ligne, colonne, z, p.getLigne(), p.getColonne(), p.getZ()));
            return;
        }

        std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

        for (const auto& voisinCoord : voisinsCoords) {
            int v_ligne = std::get<0>(voisinCoord);
            int v_colonne = std::get<1>(voisinCoord);
            int v_z = std::get<2>(voisinCoord);

            if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && // Case vide
                !GestionnaireMouvements::deplacementCasseRuche(&p, v_ligne, v_colonne, v_z, plateau) && // Ne casse pas la ruche
                emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end() && // Non visité
                GestionnaireMouvements::estPassageOuvert(ligne, colonne, v_ligne, v_colonne, plateau)) { // Passage entre les deux cases ouvert

                // Ajouter au set des emplacements visités
                emplacementsVisites.insert({ v_ligne, v_colonne, v_z });

                // Explorer récursivement cette nouvelle position avec profondeur + 1
                explorerDeplacements(v_ligne, v_colonne, v_z, profondeur + 1);

                // Nettoyer l'emplacement visité après retour de la récursion
                emplacementsVisites.erase({ v_ligne, v_colonne, v_z });
            }
        }
        };

    // Démarrer l'exploration à partir de la position actuelle de l'araignée
    int ligne = p.getLigne();
    int colonne = p.getColonne();
    int z = p.getZ();

    emplacementsVisites.insert({ ligne, colonne, z }); // Ajouter la position initiale pour éviter de revenir dessus
    std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

    for (const auto& voisinCoord : voisinsCoords) {
        int v_ligne = std::get<0>(voisinCoord);
        int v_colonne = std::get<1>(voisinCoord);
        int v_z = std::get<2>(voisinCoord);

        if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && // Case vide
            !GestionnaireMouvements::deplacementCasseRuche(&p, v_ligne, v_colonne, v_z, plateau) && // Ne casse pas la ruche
            GestionnaireMouvements::estPassageOuvert(ligne, colonne, v_ligne, v_colonne, plateau)) { // Passage entre les deux cases ouvert

            // Ajouter au set des emplacements visités
            emplacementsVisites.insert({ v_ligne, v_colonne, v_z });

            // Explorer récursivement pour 3 déplacements
            explorerDeplacements(v_ligne, v_colonne, v_z, 1);

            // Nettoyer l'emplacement visité après exploration
            emplacementsVisites.erase({ v_ligne, v_colonne, v_z });
        }
    }

    return mouvementsPossibles;
}

std::vector<Mouvement*> Fourmi::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites; // Set pour éviter les doublons

    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
        return mouvementsPossibles;
    }

    // Fonction interne pour explorer récursivement les déplacements
    std::function<void(int, int, int)> explorerDeplacements = [&](int ligne, int colonne, int z) {
        std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

        for (const auto& voisinCoord : voisinsCoords) {
            int v_ligne = std::get<0>(voisinCoord);
            int v_colonne = std::get<1>(voisinCoord);
            int v_z = std::get<2>(voisinCoord);

            if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && // Case vide
                !GestionnaireMouvements::deplacementCasseRuche(&p, v_ligne, v_colonne, v_z, plateau) && // Ne casse pas la ruche
                emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end() && // Non visité
                GestionnaireMouvements::estPassageOuvert(ligne, colonne, v_ligne, v_colonne, plateau)) { // Passage entre les deux cases ouvert


                // Ajouter au set des emplacements visités
                emplacementsVisites.insert({ v_ligne, v_colonne, v_z });

                // Ajouter le mouvement à la liste des mouvements possibles
                mouvementsPossibles.push_back(new Mouvement(p.getId(), v_ligne, v_colonne, v_z, p.getLigne(), p.getColonne(), p.getZ()));

                // Explorer récursivement cette nouvelle position
                explorerDeplacements(v_ligne, v_colonne, v_z);
            }
        }
        };

    // Démarrer l'exploration à partir de la position actuelle de la fourmi
    int ligne = p.getLigne();
    int colonne = p.getColonne();
    int z = p.getZ();

    emplacementsVisites.insert({ ligne, colonne, z }); // Ajouter la position initiale pour éviter de revenir dessus
    explorerDeplacements(ligne, colonne, z);

    return mouvementsPossibles;
}

std::vector<Mouvement*> Scarabee::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;

    // Si le plateau est vide, aucun mouvement n'est possible
    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
        return mouvementsPossibles;
    }

    // Récupérer les cases vides autour du pion communes à ses voisins directs
    std::vector<std::tuple<int, int, int>> casesVidesCoords = GestionnaireVoisins::getCasesVidesCommunes(p, plateau);

    for (const auto& caseVide : casesVidesCoords) {
        int new_ligne = std::get<0>(caseVide);
        int new_colonne = std::get<1>(caseVide);
        int new_z = std::get<2>(caseVide);
        // Traiter chaque case vide commune

    // Vérifier que le passage est ouvert ET que le déplacement est valide (sans survoler un trou)
        if (z > 0 || !GestionnaireMouvements::deplacementCasseRuche(&p, new_ligne, new_colonne, new_z, plateau)) {

            // Éviter les doublons
            if (emplacementsVisites.find({ new_ligne, new_colonne, new_z }) == emplacementsVisites.end()) {
                emplacementsVisites.insert({ new_ligne, new_colonne, new_z });
                mouvementsPossibles.push_back(new Mouvement(p.getId(), new_ligne, new_colonne, new_z, p.getLigne(), p.getColonne(), p.getZ()));
            }
        }
    }
    std::vector<Pion*> voisins = GestionnaireVoisins::getVoisins(p, plateau);

    for (Pion* voisin : voisins) {
        if (voisin) {
            int voisinLigne = voisin->getLigne();
            int voisinColonne = voisin->getColonne();
            int voisinZ = voisin->getZ() + 1;

            mouvementsPossibles.push_back(new Mouvement(p.getId(), voisinLigne, voisinColonne, voisinZ, p.getLigne(), p.getColonne(), p.getZ()));
        }
    }

    return mouvementsPossibles;
}


std::vector<Mouvement*> Coccinelle::deplacementsPossibles(Pion& p, Joueur& j, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons
    std::vector<Pion*> voisins = GestionnaireVoisins::getVoisins(p, plateau);
    

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
                            Mouvement* mouv = new Mouvement(id, new_l, new_c, new_z, p.getLigne(), p.getColonne(), p.getZ());
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
    if (plateau.isVide() || j.getCouleur() != p.getCouleur()) {
        return mouvementsPossibles;
    }

    int ligne = p.getLigne();
    int colonne = p.getColonne();
    int z = p.getZ();

    if (z > 0) { // Déplacements comme une scarabe 
        std::set<std::tuple<int, int, int>> emplacementsVisites;

        // Déplacements comme une Reine 
        // Récupérer les cases vides autour
        std::vector<std::tuple<int, int, int>> casesVidesCoords = GestionnaireVoisins::getCasesVidesAutour(p, plateau);

        // Vérifier si le déplacement est valide (ne casse pas la ruche)
        for (std::tuple<int, int, int> caseVide : casesVidesCoords) {
            int new_ligne = std::get<0>(caseVide);
            int new_colonne = std::get<1>(caseVide);
            int new_z = 0;

            if (z > 0 || !GestionnaireMouvements::deplacementCasseRuche(&p, new_ligne, new_colonne, new_z, plateau)) {
                // Éviter les doublons
                if (emplacementsVisites.find({ new_ligne, new_colonne, new_z }) == emplacementsVisites.end()) {
                    emplacementsVisites.insert({ new_ligne, new_colonne, new_z });
                    mouvementsPossibles.push_back(new Mouvement(p.getId(), new_ligne, new_colonne, new_z, p.getLigne(), p.getColonne(), p.getZ()));
                }
            }
        }

        // Récupérer les voisins directs du Scarabée
        std::vector<Pion*> voisins = GestionnaireVoisins::getVoisins(p, plateau);

        // Modification du Z pour monter dessus
        for (Pion* voisin : voisins) {
            if (voisin) {
                while (plateau.getGrille()[voisin->getLigne()][voisin->getColonne()][voisin->getZ() + 1] != nullptr) {
                    voisin = plateau.getGrille()[voisin->getLigne()][voisin->getColonne()][voisin->getZ() + 1];
                }
                mouvementsPossibles.push_back(new Mouvement(p.getId(), voisin->getLigne(), voisin->getColonne(), voisin->getZ() + 1, p.getLigne(), p.getColonne(), p.getZ()));
            }
        }
        return mouvementsPossibles;
    }


    std::vector<Pion*> voisinsTypes = GestionnaireVoisins::getVoisins(ligne, colonne, plateau, z);

    std::unordered_set<std::string> typesVus;
    typesVus.insert("M"); //pour ne pas avoir de mouvement de moustique
    std::vector<Pion*> voisinsSansDoublon;

   for (const auto& voisinType : voisinsTypes) {
       if (voisinType != nullptr) {
           string v_type = voisinType->getType();

           // Vérifier si l'élément existe déjà
           if (typesVus.find(v_type) == typesVus.end()) {
               typesVus.insert(v_type); // Marque le type comme vu
 
               voisinsSansDoublon.push_back(voisinType); // Ajouter si l'élément n'existe pas
           }
           else {
               std::cout << "Type deja present ou Moustique" << std::endl;
           }
       }    
    }

    for (const auto& voisin : voisinsSansDoublon) {
        for (const auto& NouvMouv : voisin->deplacementsPossibles(p, j, plateau)) {
            bool estDejaPresent = false;
            for (const auto& mouvement : mouvementsPossibles) {
                if (mouvement->getPionId() == NouvMouv->getPionId() &&
                    mouvement->getCoordDest() == NouvMouv->getCoordDest() &&
                    mouvement->getCoordSrc() == NouvMouv->getCoordSrc()) {
                    estDejaPresent = true;
                    break;
                }
            }
            if (!estDejaPresent) {
                mouvementsPossibles.push_back(NouvMouv);
            }
        }
    } 

    return mouvementsPossibles;
}
