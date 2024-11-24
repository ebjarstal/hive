#include "gestionnaireMouvements.h"
#include "plateau.h"
#include "joueur.h"

// Retourne vrai si le pion peut bouger (ne casse pas la ruche)
bool GestionnaireMouvements::cassageRuche(Pion& p, Plateau& plateau) {

    // Verification si le pion est pose
    if (GestionnairePions::estPose(p) == false) {
        std::cout << "Le pion n est pas pose sur le plateau\n";
        return true;
    }

    // On doit avoir des voisins, nous sommes pas censé aller dans cette boucle
    if (GestionnaireVoisins::nombreVoisins(p, plateau) == 0) {
        std::cout << "Le pion n a pas de voisins\n";
        return false;
    }

    // Verification de la rupture de la ruche
    std::vector<Pion*> voisins = GestionnaireVoisins::getVoisins(p, plateau);
    std::vector<Pion*> ruche_ref;

    // Sauvegarde des données du pion qui va etre supprimé
    int ref_l = p.getLigne();
    int ref_c = p.getColonne();
    int ref_z = p.getZ();

    GestionnairePions::deletePion(p, plateau); // V rification du cassage de la ruche en supprimant le pion
    
    for (Pion* voisin : voisins) {
        // compare chaque voisin du pion this avec le pion p
        if (voisin) {
            if (ruche_ref.empty()) {
                ruche_ref = GestionnaireVoisins::getRuche(voisin, plateau); // Initialisation de la reference
            }
            // Verification si les ruches des voisins sont les m mes si on enl ve le pion
            else if (GestionnaireVoisins::getRuche(voisin, plateau) != ruche_ref) {
                std::cout << "Le deplacement du pion casse la ruche !\n";
                GestionnairePions::setPion(ref_l, ref_c, ref_z, &p, plateau); // On remet le pion apr s v rification
                return true;
            }
        }
    }
    GestionnairePions::setPion(ref_l, ref_c, ref_z, &p, plateau); // On remet le pion apr s v rification
    return false;
}


std::list<Mouvement*> GestionnaireMouvements::emplacementsPossibles(Pion& p, Plateau& plateau) {
    std::list<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons

    if (plateau.isVide()) {
        // Ajouter la case du milieu du plateau à mouvementsPossibles
        int milieuLigne = plateau.getNbLignes() / 2 - 1;
        int milieuColonne = plateau.getNbColonnes() / 2 - 1;
        mouvementsPossibles.push_back(new Mouvement(p.getId(), milieuLigne, milieuColonne, 0, -1, -1, -1));
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
                if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && distCarree == 9 && !deplacementCasseRuche(&p, v_ligne, v_colonne, v_z, plateau)) {
                    // Vérifier si cet emplacement a déjà été visité
                    if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                        // Ajouter l'emplacement au set pour éviter les doublons
                        emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                        // Ajouter le mouvement à la liste des mouvements possibles
                        mouvementsPossibles.push_back(new Mouvement(p.getId(), v_ligne, v_colonne, v_z, ligne, colonne, z));
                    }
                }
            }
        }
    }

    return mouvementsPossibles;
}

std::vector<std::tuple<Pion*, int, int, int>> GestionnaireMouvements::getPionsBougeables(Plateau& plateau, Joueur& j) {
    const std::vector<std::tuple<Pion*, int, int, int>>& pionsSurPlateau = GestionnairePions::getPions(plateau);
    std::vector<std::tuple<Pion*, int, int, int>> pionsBougeables;
    // Pour chaque pion sur le plateau, on regarde s'il peut bouger
    for (std::tuple<Pion*, int, int, int> pions : pionsSurPlateau) {
        Pion* pion = std::get<0>(pions);
        if (!(GestionnaireMouvements::cassageRuche(*pion, plateau))) {
            //std::cout << "Le pion suivant ne casse pas la ruche : " << pion->getColonne() << " " << pion->getLigne() << " " << pion->getZ() << "\n";
            pionsBougeables.push_back(pions);
        }
    }
    return pionsBougeables;
}

// Booléen si le déplacement casse la ruche ou non
bool GestionnaireMouvements::deplacementCasseRuche(Pion* pion, int newLigne, int newColonne, int newZ, Plateau& plateau) {
    std::vector<Pion*> rucheAvant = GestionnaireVoisins::getRuche(pion, plateau);

    // D placer temporairement le pion
    int oldLigne = pion->getLigne();
    int oldColonne = pion->getColonne();
    int oldZ = pion->getZ();
    GestionnairePions::deletePion(*pion, plateau);
    GestionnairePions::setPion(newLigne, newColonne, newZ, pion, plateau);

    std::vector<Pion*> rucheApres = GestionnaireVoisins::getRuche(pion, plateau);

    // Remettre le pion   sa position initiale
    GestionnairePions::deletePion(*pion, plateau);
    GestionnairePions::setPion(oldLigne, oldColonne, oldZ, pion, plateau);

    return rucheAvant.size() != rucheApres.size();
}

std::vector<Mouvement*> GestionnaireMouvements::genererTousLesMouvements(Plateau& p, Joueur& joueur) {
    std::vector<Mouvement*> mouvements;

    // Obtenir tous les pions du joueur sur le plateau
    std::vector<Pion*> pionsJoueur = GestionnairePions::getPionsEnJeu(p, joueur.getCouleur());
    std::vector<Pion*> pionsEnMain = joueur.getPionsEnMain();

    for (Pion* pion : pionsEnMain) {
        // Obtenir les emplacements valides pour poser ce pion
        std::list<Mouvement*> placementsPossibles = emplacementsPossibles(*pion, p);
        std::cout << "Nombre placements possibles de mouvements générés : " << placementsPossibles.size() << std::endl;
        // Ajouter les mouvements de placement à la liste des mouvements possibles
        mouvements.insert(mouvements.end(), placementsPossibles.begin(), placementsPossibles.end());
    }

    std::cout << "Nombre premiere etape de mouvements générés : " << mouvements.size() << std::endl;
    // Cas où le joueur n'a aucun pion sur le plateau : générer les placements possibles
    if (!pionsJoueur.empty()) {
        // Cas normal : générer les mouvements pour les pions déjà en jeu
        for (Pion* pion : pionsJoueur) {
            // Obtenir les emplacements possibles pour ce pion
            std::list<Mouvement*> deplacementsValides = deplacementsPossibles(*pion, p);

            // Ajouter les déplacements valides à la liste des mouvements possibles
            mouvements.insert(mouvements.end(), deplacementsValides.begin(), deplacementsValides.end());

            // Libérer les emplacements inutilisés
            for (Mouvement* emplacement : deplacementsValides) {
                if (std::find(deplacementsValides.begin(), deplacementsValides.end(), emplacement) == deplacementsValides.end()) {
                    delete emplacement;
                }
            }
        }
    }
    std::cout << "Nombre total de mouvements générés : " << mouvements.size() << std::endl;

    return mouvements;
}


std::list<Mouvement*> GestionnaireMouvements::deplacementsPossiblesAraignee(Pion& p, Plateau& plateau) {
    std::list<Mouvement*> mouvementsPossibles;
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
                if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && distCarree == 9 && !deplacementCasseRuche(&p, v_ligne, v_colonne, v_z, plateau)) {
                    // Vérifier si cet emplacement a déjà été visité
                    if (emplacementsVisites.find({ v_ligne, v_colonne, v_z }) == emplacementsVisites.end()) {
                        // Ajouter l'emplacement au set pour éviter les doublons
                        emplacementsVisites.insert({ v_ligne, v_colonne, v_z });
                        // Ajouter le mouvement à la liste des mouvements possibles
                        mouvementsPossibles.push_back(new Mouvement(p.getId(), v_ligne, v_colonne, v_z, ligne, colonne, z));
                    }
                }
            }
        }
    }

    return mouvementsPossibles;
}

std::list<Mouvement*> GestionnaireMouvements::deplacementsPossiblesReine(Pion& p, Plateau& plateau) {
    std::list<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;

    // Si le plateau est vide, aucun mouvement possible
    if (plateau.isVide()) {
        return mouvementsPossibles;
    }

    // Position actuelle de la Reine
    int ligne = p.getLigne();
    int colonne = p.getColonne();
    int z = p.getZ();

    // Récupérer les coordonnées des voisins du pion Reine
    std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);
    
    // Récupérer les coordonnées des cases vides autour du pion Reine
    std::vector<std::tuple<int, int, int>> casesVidesCoords = GestionnaireVoisins::getCasesVidesAutour(p, plateau);

    // Parcourir les voisins pour récupérer leurs cases vides
    for (const auto& voisinCoord : voisinsCoords) {
        int v_ligne = std::get<0>(voisinCoord);
        int v_colonne = std::get<1>(voisinCoord);
        int v_z = std::get<2>(voisinCoord);

        // Récupérer les coordonnées des cases vides autour des voisins
        std::vector<std::tuple<int, int, int>> casesVidesCoordsVoisin = GestionnaireVoisins::getCasesVidesAutour(v_ligne, v_colonne,v_z,plateau);

        // Trouver les cases communes entre les cases vides autour de la Reine et celles des voisins
        for (const auto& caseVide : casesVidesCoords) {
            if (std::find(casesVidesCoordsVoisin.begin(), casesVidesCoordsVoisin.end(), caseVide) != casesVidesCoordsVoisin.end()) { // std::find() Retourne casesVidesCoordsVoisin.end() si l'élément caseVide n'est pas dans casesVidesCoordsVoisin
                int c_ligne = std::get<0>(caseVide);
                int c_colonne = std::get<1>(caseVide);
                int c_z = std::get<2>(caseVide);

                // Vérifier si le déplacement est valide (ne casse pas la ruche)
                if (!deplacementCasseRuche(&p, c_ligne, c_colonne, c_z, plateau)) {
                    // Éviter les doublons
                    if (emplacementsVisites.find({ c_ligne, c_colonne, c_z }) == emplacementsVisites.end()) {
                        emplacementsVisites.insert({ c_ligne, c_colonne, c_z });
                        mouvementsPossibles.push_back(new Mouvement(p.getId(), c_ligne, c_colonne, c_z, ligne, colonne, z));
                    }
                }
            }
        }
    }

    return mouvementsPossibles;
}


std::list<Mouvement*> GestionnaireMouvements::deplacementsPossibles(Pion& p, Plateau& plateau) {
    std::list<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsVisites;  // Set pour éviter les doublons

    if (plateau.isVide()) {
        return mouvementsPossibles;
    }
    else {
        if (p.getType() == "R") {
            return deplacementsPossiblesReine(p, plateau); // Appel de la fonction dédiée pour la Reine
        }

        else if (p.getType() == "A") {
            return deplacementsPossiblesAraignee(p, plateau); // Appel de la fonction dédiée pour la Reine
        }

        // Ajouter les autres cas et supprimer le else à la fin

        else{
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
                if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr && !deplacementCasseRuche(&p, v_ligne, v_colonne, v_z, plateau)) {
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
    }

    return mouvementsPossibles;
    }
}

