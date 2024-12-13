#include "VuePion.h"
#include <QBrush>
#include <QLinearGradient>
#include <QPainter>
#include "Controleur.h"
#include "Constantes.h"

VuePion::VuePion(QGraphicsItem* parent) : QGraphicsPolygonItem(parent), estPose(false) {
    // dessiner l'hexagone avec les points nécessaires
    QVector<QPointF> hexPoints;
    hexPoints << QPointF(1, 0) << QPointF(2, 0.58) << QPointF(2, 1.73)
        << QPointF(1, 2.31) << QPointF(0, 1.73) << QPointF(0, 0.58);

    // augmente la taille de la forme
    for (size_t i = 0, n = hexPoints.size(); i < n; i++) {
        hexPoints[i] *= SCALE_BY;
    }

    // crée un hexagone avec les points de taille augmentée
    QPolygonF hexagone(hexPoints);

    // dessine l'hexagone
    setPolygon(hexagone);

    // initialise
    setEstPose(false);
    setType(QString(""));

    // couleur par défaut
    setCouleur(Qt::white);
}

QColor VuePion::getCouleur() {
    return couleur;
}

void VuePion::setCouleur(QColor nouvelleCouleur) {
    couleur = nouvelleCouleur;

    // donne le look métallique
    QLinearGradient gradient(0, 0, 1, 1);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient.setColorAt(0, couleur.lighter(150));
    gradient.setColorAt(0.5, couleur);
    gradient.setColorAt(1, couleur.darker(150));

    QBrush brush(gradient);
    setBrush(brush);
}

QString VuePion::getType() {
    return type;
}

void VuePion::setType(QString nouveauType) {
    type = nouveauType;
    update(); // Redessiner l'élément pour afficher le nouveau type
}

bool VuePion::getEstPose() {
    return estPose;
}

void VuePion::setEstPose(bool nouvelEtat) {
    estPose = nouvelEtat;
}

void VuePion::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    // TODO ?
}

void VuePion::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    // Appeler la méthode de base pour dessiner l'hexagone
    QGraphicsPolygonItem::paint(painter, option, widget);

    // Dessiner le texte au centre de l'hexagone
    painter->setPen(Qt::white);
    QFont font = painter->font();
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, type);
}
