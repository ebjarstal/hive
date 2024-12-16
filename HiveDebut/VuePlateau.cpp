#include "VuePlateau.h"
#include "Constantes.h"

VuePlateau::VuePlateau(QGraphicsScene* scene, Plateau* plateau)
    : scene(scene), plateau(plateau) {
}

void VuePlateau::initialiserPlateau(int x, int y) {
    scene->clear();
    listePionsPlateau.clear(); // Clear the list when initializing the plateau

    int decalage_x = (LARGEUR_PIONS + ESPACEMENT_PIONS) / 2;
    int decalage_y = PRESQUE_DEUX_TIERS_HAUTEUR_PION + ESPACEMENT_PIONS;

    int nb_lignes = plateau->getNbLignes();
    int nb_colonnes = plateau->getNbColonnes();

    for (unsigned int i = 0; i < nb_lignes; i++) {
        creerLignePions(x + (i % 2) * decalage_x, y + i * decalage_y, i, nb_colonnes);
    }
}

void VuePlateau::creerLignePions(int x, int y, int ligne, int nb_colonnes) {
    // crée une ligne de pions vides aux coordonnées indiquées (x, y) avec le nombre de colonnes spécifié (nb_colonnes)
    for (size_t i = 0, n = nb_colonnes; i < n; i++) {
        Pion* pion = plateau->getGrille()[ligne][i][0];
        VuePion* vuePion = new VuePion();
        if (pion != nullptr) {
            vuePion->setCouleur(pion->getCouleur());
            vuePion->setType(QString::fromStdString(pion->getType()));
        }
        else {
            vuePion->setCouleur(Qt::white);
            vuePion->setType(QString(""));
        }
        vuePion->setPionAssocie(pion);
        vuePion->setEstPose(true);
        vuePion->setPos(x + (LARGEUR_PIONS + ESPACEMENT_PIONS) * i, y);
        scene->addItem(vuePion);
        listePionsPlateau.append(vuePion);
    }
}
