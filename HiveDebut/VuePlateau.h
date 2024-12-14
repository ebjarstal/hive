#pragma once

#include <QGraphicsItem>
#include <QGraphicsScene>
#include "plateau.h"
#include "VuePion.h"

class VuePlateau {
public:
    VuePlateau(QGraphicsScene* scene, Plateau* plateau);

    void initialiserPlateau(int x, int y);
    void creerLignePions(int x, int y, int ligne, int nb_colonnes);

private:
    QGraphicsScene* scene;
    Plateau* plateau;
};
