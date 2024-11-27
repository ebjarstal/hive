#include "command.h"
#include "partie.h"
#include "pions.h"
#include "joueur.h"
#include "mouvement.h"

MouvementCommand::MouvementCommand(Partie& p, Mouvement* mouv) : partie(p), mouv(mouv), plateau(partie.getPlateau()) {
	pion = Pion::getPionById(mouv->getPionId());
	joueur = partie.getJoueur1();
	if (joueur->getCouleur() != pion->getCouleur()) {
		joueur = partie.getJoueur2();
	}
}

void MouvementCommand::execute() {
	if (mouv->getOldLigne() == -1 && mouv->getOldColonne() == -1 && mouv->getOldZ() == -1) {
		GestionnairePions::setPion(mouv->getLigne(), mouv->getColonne(), mouv->getZ(), pion, plateau);
		joueur->getPionsEnMain().erase(std::remove_if(joueur->getPionsEnMain().begin(), joueur->getPionsEnMain().end(), [&](Pion* p) { return p->getId() == pion->getId(); }), joueur->getPionsEnMain().end()); // Ne supprime rien
	}
	else {
		GestionnairePions::movePion(mouv->getLigne(), mouv->getColonne(), mouv->getZ(), pion, plateau);
	}

	bool estSurBordure = (mouv->getLigne() == 0 || mouv->getColonne() == 0 || mouv->getLigne() == plateau.getNbLignes() - 1 || mouv->getColonne() == plateau.getNbColonnes() - 1);
	if (estSurBordure) {
		// Redimensionner le plateau pour ajouter de l'espace autour
		plateau.redimensionnerPlateau();
	}
}

void MouvementCommand::undo() {
	if (mouv->getOldLigne() == -1 && mouv->getOldColonne() == -1 && mouv->getOldZ() == -1) {
		GestionnairePions::deletePion(*pion, plateau);
		joueur->getPionsEnMain().push_back(pion);
	}
	else {
		GestionnairePions::movePion(mouv->getOldLigne(), mouv->getOldColonne(), mouv->getOldZ(), pion, plateau);
	}
}

std::string MouvementCommand::getDescription() const {
	std::ostringstream oss;
	int j = 1;
	if (joueur == partie.getJoueur2()) {
		j = 2;
	}
	oss << "Pion ID: " << pion->getId()
		<< " De (" << mouv->getOldLigne() << ", " << mouv->getOldColonne() << ", " << mouv->getOldZ() << ")"
		<< " A (" << mouv->getLigne() << ", " << mouv->getColonne() << ", " << mouv->getZ() << ") "
		<< j;
	return oss.str();
}