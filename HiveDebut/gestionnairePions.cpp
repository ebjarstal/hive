#include "gestionnairePions.h"
#include "plateau.h"

// Obtenir le Pion a la position specifiee dans la grille
Pion* GestionnairePions::getPion(int ligne, int colonne, const Plateau& plateau, int z){
    if (plateau.estValide(ligne, colonne, z)) {
        return plateau.grille[ligne][colonne][z];
    }
    throw std::out_of_range("Position de grille invalide"); // Lancer une exception si la position est invalide
}


std::vector<std::tuple<Pion*, int, int, int>> GestionnairePions::getPions(Plateau& plateau){
    std::vector<std::tuple<Pion*, int, int, int>> pions;
    for (unsigned int l = 0; l < plateau.nb_lignes; ++l) {
        for (unsigned int c = 0; c < plateau.nb_colonnes; ++c) {
            for (int z = 0; z < plateau.grille[l][c].size(); ++z) {
                if (plateau.grille[l][c][z] != nullptr) {
                    pions.push_back(std::make_tuple(plateau.grille[l][c][z], l, c, z));
                }
            }
        }
    }
    return pions;
}

// Definir un Pion a la position specifiee dans la grille
void GestionnairePions::setPion(int ligne, int colonne, int z, Pion* pion, Plateau& plateau) {

    if (plateau.estValide(ligne, colonne, z)) {
        delete plateau.grille[ligne][colonne][z]; // Supprimer le Pion existant pour eviter une fuite de memoire
        pion->setLigne(ligne);
        pion->setColonne(colonne);
        pion->setZ(z);
        Pion::resetPion(pion, pion->getId());
        plateau.grille[ligne][colonne][z] = pion;
        //std::cout << "Pion ajoute aux coordonnees suivantes : (" << colonne << "," << ligne << "," << z << ")\n";
    }
    else {
        throw std::out_of_range("Position de grille invalide"); // Lancer une exception si la position est invalide
    }

    bool estSurBordure = (ligne == 0 || colonne == 0 || ligne == plateau.nb_lignes - 1 || colonne == plateau.nb_colonnes - 1);
    if (estSurBordure) {
        // Redimensionner le plateau pour ajouter de l'espace autour
        plateau.redimensionnerPlateau();
    }
}

// Supprimer le pion aux coordonnees specifiees pour  viter une fuite de memoire
void GestionnairePions::deletePion(Pion& p, Plateau& plateau) {
    int ligne = p.getLigne();
    int colonne = p.getColonne();
    int z = p.getZ();
    if (plateau.estValide(ligne, colonne, z)) {
        plateau.grille[ligne][colonne][z] = nullptr;
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