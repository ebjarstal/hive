#include "VuePion.h"
#include <QBrush>
#include "VuePartie.h"
#include "Constantes.h"

extern VuePartie* partie;

VuePion::VuePion(QGraphicsItem* parent) {
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
	estPose = false;
}

QString VuePion::getProprietaire() {
	return proprietaire;
}

void VuePion::setProprietaire(QString joueur) {
	proprietaire = joueur;

	// change la couleur du pion en fonction du proprietaire
	// les propriétaires sont parmis {joueur1, joueur2, personne}
	// joueur1 est à gauche de l'écran, joueur2 est à droite de l'écran
	if (joueur == QString("personne")) {
		QBrush pinceau;
		pinceau.setColor(Qt::lightGray);
		setBrush(pinceau);
		setOpacity(0.1);
	}
	else if (joueur == QString("temporaire")) {
		QLinearGradient gradient(0, 0, 1, 1);
		gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
		gradient.setColorAt(0, Qt::lightGray);
		gradient.setColorAt(1, Qt::darkGray);
		QBrush pinceau(gradient);
		setBrush(pinceau);
		setOpacity(0.5);
	}
	else if (joueur == QString(JOUEUR1)) {
		QLinearGradient gradient(0, 0, 1, 1);
		gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
		gradient.setColorAt(0, Qt::blue);
		gradient.setColorAt(1, Qt::darkBlue);
		QBrush pinceau(gradient);
		setBrush(pinceau);
	}
	else if (joueur == QString(JOUEUR2)) {
		QLinearGradient gradient(0, 0, 1, 1);
		gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
		gradient.setColorAt(0, Qt::red);
		gradient.setColorAt(1, Qt::darkRed);
		QBrush pinceau(gradient);
		setBrush(pinceau);
	}
}

bool VuePion::getEstPose() {
	return estPose;
}

void VuePion::setEstPose(bool etat) {
	estPose = etat;
}

void VuePion::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	// si le pion n'est PAS posé, le "ramasser" avec le curseur
	if (getEstPose() == false) {
		partie->ramasserPion(this);
	}

	else if (getEstPose() == true && getProprietaire() == partie->getAQuiDeJouer()) {
		partie->ramasserPion(this);
	}

	// si ce pion EST posé, alors le remplacer
	else if (partie->pionEnCoursDeTraitement != nullptr && getProprietaire() == QString("personne")) {
		partie->poserPion(this);
	}
}
