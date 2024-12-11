#include "VuePlateau.h"
#include "VuePartie.h"
#include "Constantes.h"

extern VuePartie* partie;

VuePlateau::VuePlateau() {

}

QList<VuePion*> VuePlateau::getPions() {
	return pions;
}

void VuePlateau::poserPions(int x, int y, int nb_lignes, int nb_colonnes) {
	int decalage_x = (LARGEUR_PIONS + ESPACEMENT_PIONS) / 2;
	int decalage_y = PRESQUE_DEUX_TIERS_HAUTEUR_PION + ESPACEMENT_PIONS;

	for (size_t i = 0, n = nb_lignes; i < n; i++) {
		creerLignePions(x + (i % 2) * decalage_x, y + i * decalage_y, nb_colonnes);
		// explications: - chaque ligne générée est décalée vers le bas de i * decalage_y
		//               - une ligne sur deux doit commencer plus à droite que la précédente. Le i % 2 permet de faire ça
		//				 - en fin de compte, on génère le plateau en le créant ligne par ligne, grâce à la méthode privée creerLignePions
	}
}

void VuePlateau::creerLignePions(int x, int y, int nb_colonnes) {
	// crée une ligne de pions vides aux coordonnées indiquées (x, y) avec le nombre de colonnes spécifié (nb_colonnes)
	for (size_t i = 0, n = nb_colonnes; i < n; i++) {
		VuePion* pion = new VuePion();
		pion->setPos(x + (LARGEUR_PIONS + ESPACEMENT_PIONS) * i, y);
		pions.append(pion);
		partie->scene->addItem(pion);

		// set le propriétaire des pions à "personne"
		pion->setProprietaire(QString("personne"));
		pion->setEstPose(true);
	}
}