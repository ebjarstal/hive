#include "gestionnaireMouvements.h"
#include "plateau.h"

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

std::vector<Mouvement*> GestionnaireMouvements::emplacementsPossibles(Pion& p, Plateau& plateau) {
    std::vector<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsUniques;

    // V rifier si le plateau est vide
    if (plateau.isVide()) {
        // Si le plateau est vide, ajouter seulement le centre comme emplacement possible
        emplacementsUniques.insert({ (plateau.nb_lignes - 1) / 2, (plateau.nb_colonnes - 1) / 2, 0 });
    }
    else {
        // R cup rer tous les pions pr sents sur le plateau
        std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = GestionnairePions::getPions(plateau);

        // V rifier si c'est le deuxi me tour (un seul pion sur le plateau)
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
            // Parcourir tous les pions de la m me couleur pour ajouter les emplacements valides
            for (const auto& pionTuple : pionsSurPlateau) {
                Pion* pionActuel = std::get<0>(pionTuple);
                if (pionActuel->getCouleur() == p.getCouleur()) {
                    int ligne = std::get<1>(pionTuple);
                    int colonne = std::get<2>(pionTuple);
                    int z = std::get<3>(pionTuple);

                    // Obtenir les coordonn es des cases voisines de ce pion
                    std::vector<std::tuple<int, int, int>> voisinsCoords = GestionnaireVoisins::getVoisinsCoords(ligne, colonne, plateau, z);

                    // Parcourir chaque case voisine
                    for (const auto& voisinCoord : voisinsCoords) {
                        int v_ligne = std::get<0>(voisinCoord);
                        int v_colonne = std::get<1>(voisinCoord);
                        int v_z = std::get<2>(voisinCoord);

                        // V rifier si la case voisine est vide
                        if (GestionnairePions::getPion(v_ligne, v_colonne, plateau, v_z) == nullptr) {
                            emplacementsUniques.insert({ v_ligne, v_colonne, v_z });
                        }
                    }
                }
            }
        }
    }

    // Convertir les emplacements uniques en objets Mouvement et les ajouter   la liste des mouvements possibles
    for (const auto& emplacement : emplacementsUniques) {
        int ligne = std::get<0>(emplacement);
        int colonne = std::get<1>(emplacement);
        int z = std::get<2>(emplacement);
        mouvementsPossibles.push_back(new Mouvement(p.getId(), ligne, colonne, z, -1, -1, -1));
    }

    return mouvementsPossibles;
}

std::vector<std::tuple<Pion*, int, int, int>> GestionnaireMouvements::getPionsBougeables(Plateau& plateau) {
    const std::vector<std::tuple<Pion*, int, int, int>>& pionsSurPlateau = GestionnairePions::getPions(plateau);
    std::vector<std::tuple<Pion*, int, int, int>> pionsBougeables;
    // Pour chaque pion sur le plateau, on regarde s'il peut bouger
    for (std::tuple<Pion*, int, int, int> pions: pionsSurPlateau) {
        Pion* pion = std::get<0>(pions);
        if (!(cassageRuche(*pion, plateau))) {
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

// Filtre les deplacements valides de tous les deplacements possibles
std::vector<Mouvement*> GestionnaireMouvements::filtrerDeplacementsValides(const std::vector<Mouvement*>& deplacements, Pion* pion, Plateau& plateau) {
    std::vector<Mouvement*> deplacementsValides;
    for (Mouvement* m : deplacements) {
        if (!deplacementCasseRuche(pion, m->getLigne(), m->getColonne(), m->getZ(), plateau)) {
            m->setCoordSrc(make_tuple(pion->getLigne(), pion->getColonne(), pion->getZ()));
            deplacementsValides.push_back(m);
        }
    }
    return deplacementsValides;
}