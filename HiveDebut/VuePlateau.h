#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QList>
#include "plateau.h"
#include "VuePion.h"

class VuePlateau {
public:
    VuePlateau(QGraphicsScene* scene, Plateau* plateau);

    void initialiserPlateau(int x, int y);
    void creerLignePions(int x, int y, int ligne, int nb_colonnes);

    // setters

    // getters
    QList<VuePion*> getListePionsPlateau() const;
    VuePion* getVuePion(int ligne, int colonne, int couche) const;
    Plateau* getPlateau() { return plateau; }
    std::vector<std::vector<std::vector<VuePion*>>> getGrilleVuePions() { return grilleVuePions; }

private:
    QGraphicsScene* scene;
    Plateau* plateau;
    std::vector<std::vector<std::vector<VuePion*>>> grilleVuePions;
};
