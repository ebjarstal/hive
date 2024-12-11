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
		// explications: - chaque ligne g�n�r�e est d�cal�e vers le bas de i * decalage_y
		//               - une ligne sur deux doit commencer plus � droite que la pr�c�dente. Le i % 2 permet de faire �a
		//				 - en fin de compte, on g�n�re le plateau en le cr�ant ligne par ligne, gr�ce � la m�thode priv�e creerLignePions
	}
}

void VuePlateau::creerLignePions(int x, int y, int nb_colonnes) {
	// cr�e une ligne de pions vides aux coordonn�es indiqu�es (x, y) avec le nombre de colonnes sp�cifi� (nb_colonnes)
	for (size_t i = 0, n = nb_colonnes; i < n; i++) {
		VuePion* pion = new VuePion();
		pion->setPos(x + (LARGEUR_PIONS + ESPACEMENT_PIONS) * i, y);
		pions.append(pion);
		partie->scene->addItem(pion);

		// set le propri�taire des pions � "personne"
		pion->setProprietaire(QString("personne"));
		pion->setEstPose(true);
	}
}