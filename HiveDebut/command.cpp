#include "command.h"
#include "partie.h"
#include "pions.h"
#include "joueur.h"
#include "mouvement.h"

PoserPionCommand::PoserPionCommand(Partie& p, Mouvement* mouv) : partie(p), mouv(mouv), plateau(partie.getPlateau()) {
	pion = Pion::getPionById(mouv->getPionId());
	joueur = partie.getJoueur1();
	if (joueur->getCouleur() != pion->getCouleur()) {
		joueur = partie.getJoueur2();
	}
}

void PoserPionCommand::execute() {
    GestionnairePions::setPion(mouv->getLigne(), mouv->getColonne(), mouv->getZ(), pion, plateau);
	joueur->pionsEnMain.erase(std::remove_if(joueur->pionsEnMain.begin(), joueur->pionsEnMain.end(), [&](Pion* p) { return p->getId() == pion->getId(); }), joueur->pionsEnMain.end()); // Ne supprime rien
}

void PoserPionCommand::undo() {
    GestionnairePions::deletePion(*pion, plateau);
    joueur->pionsEnMain.push_back(pion);
}

std::string PoserPionCommand::getDescription() const {
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

DeplacerPionCommand::DeplacerPionCommand(Partie& p, Mouvement* mouv) : partie(p), mouv(mouv), plateau(partie.getPlateau()) {
	pion = Pion::getPionById(mouv->getPionId());
	joueur = partie.getJoueur1();
	if (joueur->getCouleur() != pion->getCouleur()) {
		joueur = partie.getJoueur2();
	}
}

void DeplacerPionCommand::execute() {
    GestionnairePions::movePion(mouv->getLigne(), mouv->getColonne(), mouv->getZ(), pion, plateau);

}

void DeplacerPionCommand::undo() {
    GestionnairePions::movePion(mouv->getOldLigne(), mouv->getOldColonne(), mouv->getOldZ(), pion, plateau);
}

std::string DeplacerPionCommand::getDescription() const {
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