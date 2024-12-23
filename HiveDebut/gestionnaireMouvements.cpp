#include "gestionnaireMouvements.h"
#include "plateau.h"
#include "joueur.h"

// Retourne vrai si le pion peut bouger (ne casse pas la ruche)
bool GestionnaireMouvements::cassageRuche(Pion& p, Plateau& plateau) {

    // Verification si le pion est pose
    if (GestionnairePions::estPose(p) == false) {
        return true;
    }

    // On doit avoir des voisins, nous sommes pas censé aller dans cette boucle
    if (GestionnaireVoisins::nombreVoisins(p, plateau) == 0) {
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
                GestionnairePions::setPion(ref_l, ref_c, ref_z, &p, plateau); // On remet le pion apr s v rification
                return true;
            }
        }
    }
    GestionnairePions::setPion(ref_l, ref_c, ref_z, &p, plateau); // On remet le pion apr s v rification
    return false;
}

std::vector<std::tuple<Pion*, int, int, int>> GestionnaireMouvements::getPionsBougeables(Plateau& plateau, Joueur& j) {
    const std::vector<std::tuple<Pion*, int, int, int>>& pionsSurPlateau = GestionnairePions::getPions(plateau);
    std::vector<std::tuple<Pion*, int, int, int>> pionsBougeables;
    // Pour chaque pion sur le plateau, on regarde s'il peut bouger
    for (std::tuple<Pion*, int, int, int> pions : pionsSurPlateau) {
        Pion* pion = std::get<0>(pions);

        if (plateau.getGrille()[pion->getLigne()][pion->getColonne()][pion->getZ()+1] == nullptr) {
            if (pion->getCouleur() == j.getCouleur()) {
                // Un pion ne casse pas la ruche s'il est au dessus d'un autre (scarabee)
                if ((pion->getZ() > 0) || !(GestionnaireMouvements::cassageRuche(*pion, plateau))) {
                    //std::cout << "Le pion suivant ne casse pas la ruche : " << pion->getColonne() << " " << pion->getLigne() << " " << pion->getZ() << "\n";
                    pionsBougeables.push_back(pions);
                }
            }
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
    std::vector<std::tuple<Pion*, int, int, int>> pionsJoueur = getPionsBougeables(p, joueur);
    std::vector<Pion*> pionsEnMain = joueur.getPionsEnMain();

    for (Pion* pion : pionsEnMain) {
        // Obtenir les emplacements valides pour poser ce pion
        std::vector<Mouvement*> placementsPossibles = pion->emplacementsPossibles(*pion, p);
        //std::cout << "Nombre placements possibles de mouvements générés : " << placementsPossibles.size() << std::endl;
        // Ajouter les mouvements de placement à la liste des mouvements possibles
        mouvements.insert(mouvements.end(), placementsPossibles.begin(), placementsPossibles.end());
    }

    //std::cout << "Nombre premiere etape de mouvements générés : " << mouvements.size() << std::endl;
    // Cas où le joueur n'a aucun pion sur le plateau : générer les placements possibles
    if (!pionsJoueur.empty()) {
        // Cas normal : générer les mouvements pour les pions déjà en jeu
        for (std::tuple<Pion*, int, int, int> tuple : pionsJoueur) {
            Pion* pion = std::get<0>(tuple);

            // Obtenir les emplacements possibles pour ce pion
            std::vector<Mouvement*> deplacementsValides = pion->deplacementsPossibles(*pion, joueur, p);

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

    return mouvements;
}

bool GestionnaireMouvements::estPassageOuvert(int ligne1, int colonne1, int ligne2, int colonne2, Plateau& plateau) {
    // Récupérer les voisins de la première case
    std::vector<std::tuple<int, int, int>> voisins1 = GestionnaireVoisins::getVoisinsCoords(ligne1, colonne1, plateau, 0);
    std::vector<std::tuple<int, int, int>> voisins2 = GestionnaireVoisins::getVoisinsCoords(ligne2, colonne2, plateau, 0);

    // Trouver les voisins communs
    std::set<std::tuple<int, int, int>> setVoisins1(voisins1.begin(), voisins1.end());
    std::set<std::tuple<int, int, int>> setVoisins2(voisins2.begin(), voisins2.end());

    std::vector<std::tuple<int, int, int>> voisinsCommuns;
    std::set_intersection(setVoisins1.begin(), setVoisins1.end(),
        setVoisins2.begin(), setVoisins2.end(),
        std::back_inserter(voisinsCommuns));

    // Vérifier si au moins un des voisins communs est vide
    for (const auto& voisin : voisinsCommuns) {
        int v_ligne = std::get<0>(voisin);
        int v_colonne = std::get<1>(voisin);
        int v_z = std::get<2>(voisin);

        if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr) {
            return true; // Un voisin commun est vide, passage ouvert
        }
    }

    return false; // Aucun voisin commun vide, passage bloqué
}
