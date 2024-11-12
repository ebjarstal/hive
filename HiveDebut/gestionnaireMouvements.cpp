#include "gestionnaireMouvements.h"
#include "plateau.h"

// Retourne vrai si le pion peut bouger (ne casse pas la ruche ou a la place suffisante)
bool GestionnaireMouvements::peutBouger(Pion& p, Plateau& plateau) {

    // Verification si le pion est pose
    if (plateau.gestionnairePions.estPose(p) == false) {
        std::cout << "Le pion n est pas pose sur le plateau\n";
        return false;
    }

    // Verification de la rupture de la ruche
    if (plateau.gestionnaireVoisins.nombreVoisins(p, plateau) == 0) {
        std::cout << "Le pion n a pas de voisins\n";
        return true;
    }

    std::vector<Pion*> voisins = plateau.gestionnaireVoisins.getVoisins(p, plateau);
    std::vector<Pion*> ruche_ref;

    int ref_l = p.getLigne();
    int ref_c = p.getColonne();
    int ref_z = p.getZ();
    plateau.gestionnairePions.deletePion(p, plateau); // V rification du cassage de la ruche en supprimant le pion
    for (Pion* voisin : voisins) {
        // compare chaque voisin du pion this avec le pion p
        if (voisin) {
            if (ruche_ref.empty()) {
                ruche_ref = plateau.gestionnaireVoisins.getRuche(voisin, plateau); // Initialisation de la r f rence
            }
            // V rification si les ruches des voisins sont les m mes si on enl ve le pion
            else if (plateau.gestionnaireVoisins.getRuche(voisin, plateau) != ruche_ref) {
                std::cout << "Le deplacement du pion casse la ruche !";
                plateau.gestionnairePions.setPion(ref_l, ref_c, ref_z, &p, plateau); // On remet le pion apr s v rification
                return false;
            }
        }
    }
    plateau.gestionnairePions.setPion(ref_l, ref_c, ref_z, &p, plateau); // On remet le pion apr s v rification
    return true;
}

std::list<Mouvement*> GestionnaireMouvements::emplacementsPossibles(const Pion& p, Plateau& plateau) {
    std::list<Mouvement*> mouvementsPossibles;
    std::set<std::tuple<int, int, int>> emplacementsUniques;

    // V rifier si le plateau est vide
    if (plateau.isVide()) {
        // Si le plateau est vide, ajouter seulement le centre comme emplacement possible
        emplacementsUniques.insert({ (plateau.nb_lignes - 1) / 2, (plateau.nb_colonnes - 1) / 2, 0 });
    }
    else {
        // R cup rer tous les pions pr sents sur le plateau
        std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = plateau.gestionnairePions.getPions(plateau);

        // V rifier si c'est le deuxi me tour (un seul pion sur le plateau)
        if (pionsSurPlateau.size() == 1) {
            // Ajouter tous les voisins du pion existant comme emplacements possibles
            int ligne = std::get<1>(pionsSurPlateau[0]);
            int colonne = std::get<2>(pionsSurPlateau[0]);
            int z = std::get<3>(pionsSurPlateau[0]);

            std::vector<std::tuple<int, int, int>> voisinsCoords = plateau.gestionnaireVoisins.getVoisinsCoords(ligne, colonne, plateau, z);
            for (const auto& voisinCoord : voisinsCoords) {
                int v_ligne = std::get<0>(voisinCoord);
                int v_colonne = std::get<1>(voisinCoord);
                int v_z = std::get<2>(voisinCoord);

                if (plateau.gestionnairePions.getPion(v_ligne, v_colonne, plateau, v_z) == nullptr) {
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
                    std::vector<std::tuple<int, int, int>> voisinsCoords = plateau.gestionnaireVoisins.getVoisinsCoords(ligne, colonne, plateau, z);

                    // Parcourir chaque case voisine
                    for (const auto& voisinCoord : voisinsCoords) {
                        int v_ligne = std::get<0>(voisinCoord);
                        int v_colonne = std::get<1>(voisinCoord);
                        int v_z = std::get<2>(voisinCoord);

                        // V rifier si la case voisine est vide
                        if (plateau.gestionnairePions.getPion(v_ligne, v_colonne, plateau, v_z) == nullptr) {
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
        mouvementsPossibles.push_back(new Mouvement(ligne, colonne, z, -1, -1, -1));
    }

    return mouvementsPossibles;
}