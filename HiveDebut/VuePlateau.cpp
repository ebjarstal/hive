#include "VuePlateau.h"
#include "Constantes.h"

VuePlateau::VuePlateau(QGraphicsScene* scene, Plateau* plateau)
    : scene(scene), plateau(plateau) {
}

void VuePlateau::initialiserPlateau(int x, int y) {
    grilleVuePions.clear();

    int decalage_x = (LARGEUR_PIONS + ESPACEMENT_PIONS) / 2;
    int decalage_y = PRESQUE_DEUX_TIERS_HAUTEUR_PION + ESPACEMENT_PIONS;

    int nb_lignes = plateau->getNbLignes();
    int nb_colonnes = plateau->getNbColonnes();
    int nb_couches = plateau->getNbCouches();

    grilleVuePions.resize(nb_lignes);
    for (int i = 0; i < nb_lignes; ++i) {
        grilleVuePions[i].resize(nb_colonnes);
        for (int j = 0; j < nb_colonnes; ++j) {
            grilleVuePions[i][j].resize(nb_couches, nullptr);
        }
    }

    for (unsigned int i = 0; i < nb_lignes; i++) {
        creerLignePions(x + (i % 2) * decalage_x, y + i * decalage_y, i, nb_colonnes);
    }
}

void VuePlateau::creerLignePions(int x, int y, int ligne, int nb_colonnes) {
    // cr�e une ligne de pions aux coordonn�es indiqu�es (x, y) avec le nombre de colonnes sp�cifi� (nb_colonnes)
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
            vuePion->setLigne(ligne);
            vuePion->setColonne(i);
            vuePion->setZ(0);
        }
        vuePion->setPionAssocie(pion);
        vuePion->setEstPose(true);
        vuePion->setPos(x + (LARGEUR_PIONS + ESPACEMENT_PIONS) * i, y);
        scene->addItem(vuePion);
        grilleVuePions[ligne][i][0] = vuePion;
    }
}

QList<VuePion*> VuePlateau::getListePionsPlateau() const {
    QList<VuePion*> listePions;
    for (const auto& ligne : grilleVuePions) {
        for (const auto& colonne : ligne) {
            for (VuePion* vuePion : colonne) {
                if (vuePion != nullptr) {
                    listePions.append(vuePion);
                }
            }
        }
    }
    return listePions;
}

VuePion* VuePlateau::getVuePion(int ligne, int colonne, int couche) const {
    if (ligne >= 0 && ligne < grilleVuePions.size() &&
        colonne >= 0 && colonne < grilleVuePions[ligne].size() &&
        couche >= 0 && couche < grilleVuePions[ligne][colonne].size()) {
        return grilleVuePions[ligne][colonne][couche];
    }
    return nullptr;
}
