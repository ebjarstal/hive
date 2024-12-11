#ifndef VUE_PLATEAU_H
#define VUE_PLATEAU_H

#include <QList>
#include "VuePion.h"
// include "Partie.h"

class VuePlateau {
public:
	// constructeur
	VuePlateau();

	// getters
	QList<VuePion*> getPions();

	// setters

	// méthodes publiques
	void poserPions(int x, int y, int nb_colonnes, int nb_lignes);
private:
	void creerLignePions(int x, int y, int nb_colonnes);
	QList<VuePion*> pions;
};

#endif // VUE_PLATEAU_H