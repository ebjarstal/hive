#include "gestionnairePions.h"
#include "plateau.h"

// Obtenir le Pion a la position specifiee dans la grille
Pion* GestionnairePions::getPion(int ligne, int colonne, Plateau& plateau, int z){
    if (plateau.estValide(ligne, colonne, z)) {
        return plateau.getGrille()[ligne][colonne][z];
    }
    throw std::out_of_range("Position de grille invalide"); // Lancer une exception si la position est invalide
}


std::vector<std::tuple<Pion*, int, int, int>> GestionnairePions::getPions(Plateau& plateau){
    std::vector<std::tuple<Pion*, int, int, int>> pions;
    for (unsigned int l = 0; l < plateau.getNbLignes(); ++l) {
        for (unsigned int c = 0; c < plateau.getNbColonnes(); ++c) {
            for (int z = 0; z < plateau.getGrille()[l][c].size(); ++z) {
                if (plateau.getGrille()[l][c][z] != nullptr) {
                    pions.push_back(std::make_tuple(plateau.getGrille()[l][c][z], l, c, z));
                }
            }
        }
    }
    return pions;
}

// Definir un Pion a la position specifiee dans la grille
void GestionnairePions::setPion(int ligne, int colonne, int z, Pion* pion, Plateau& plateau) {

    if (plateau.estValide(ligne, colonne, z)) {
        std::vector<std::vector<std::vector<Pion*>>>& grille = plateau.getGrille();
        delete grille[ligne][colonne][z]; // Supprimer le Pion existant pour eviter une fuite de memoire
        pion->setLigne(ligne);
        pion->setColonne(colonne);
        pion->setZ(z);
        Pion::resetPion(pion, pion->getId());
        grille[ligne][colonne][z] = pion;
        //std::cout << "Pion ajoute aux coordonnees suivantes : (" << colonne << "," << ligne << "," << z << ")\n";
    }
    else {
        throw std::out_of_range("Position de grille invalide"); // Lancer une exception si la position est invalide
    }
}

// Supprimer le pion aux coordonnees specifiees pour  viter une fuite de memoire
void GestionnairePions::deletePion(Pion& p, Plateau& plateau) {
    int ligne = p.getLigne();
    int colonne = p.getColonne();
    int z = p.getZ();
    if (plateau.estValide(ligne, colonne, z)) {
        plateau.getGrille()[ligne][colonne][z] = nullptr;
        p.setLigne(0);
        p.setColonne(0);
        p.setZ(0);
        Pion::resetPion(&p, p.getId());
        //std::cout << "Pion supprime aux coordonnees suivantes : (" << colonne << "," << ligne << "," << z << ")\n";
    }
    else {
        throw std::out_of_range("Position de grille invalide");
    }
}

std::vector<Pion*> GestionnairePions::getPionsEnJeu(Plateau& plateau, string couleur) {
    // Récupérer tous les pions sur le plateau
    std::vector<std::tuple<Pion*, int, int, int>> pionsSurPlateau = getPions(plateau);

    // Vecteur pour stocker les pions appartenant à ce joueur
    std::vector<Pion*> pionsEnJeu;

    // Parcourir les pions sur le plateau
    for (size_t i = 0; i < pionsSurPlateau.size(); ++i) {
        Pion* pion = std::get<0>(pionsSurPlateau[i]); // Récupérer le pion
        if (pion->getCouleur() == couleur) {     // Vérifier la couleur
            pionsEnJeu.push_back(pion);              // Ajouter à la liste des pions du joueur
        }
    }

    return pionsEnJeu;
}

void GestionnairePions::movePion(int ligne, int colonne, int z, Pion* p, Plateau& plateau) {
    int old_ligne = p->getLigne();
    int old_colonne = p->getColonne();
    int old_z = p->getZ();
    if (plateau.estValide(old_ligne, old_colonne, old_z) && plateau.estValide(ligne, colonne, z)) {
        deletePion(*p, plateau);
        setPion(ligne, colonne, z, p, plateau);
    }
    else {
        throw std::out_of_range("Position de grille invalide");
    }
}

// Verifie les coordonnees du pion (par defaut elles sont a 0)
bool GestionnairePions::estPose(Pion& p){
    return p.getLigne() != -1 && p.getColonne() != -1 && p.getZ() != -1;
}
