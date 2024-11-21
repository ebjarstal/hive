#include "command.h"
#include "plateau.h"
#include "pions.h"
#include "joueur.h"
#include "mouvement.h"

void PoserPionCommand::execute() {
    GestionnairePions::setPion(mouv->getLigne(), mouv->getColonne(), mouv->getZ(), pion, plateau);
	joueur.pionsEnMain.erase(std::remove_if(joueur.pionsEnMain.begin(), joueur.pionsEnMain.end(), [&](Pion* p) { return p->getId() == pion->getId(); }), joueur.pionsEnMain.end()); // Ne supprime rien
}

void PoserPionCommand::undo() {
    GestionnairePions::deletePion(*pion, plateau);
    joueur.pionsEnMain.push_back(pion);
}

std::string PoserPionCommand::getDescription() const {
	std::ostringstream oss;
	int j = 1;
	oss << "Pion ID: " << pion->getId()
		<< " De (" << mouv->getOldLigne() << ", " << mouv->getOldColonne() << ", " << mouv->getOldZ() << ")"
		<< " A (" << mouv->getLigne() << ", " << mouv->getColonne() << ", " << mouv->getZ() << ") "
		<< j;
	return oss.str();
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
	oss << "Pion ID: " << pion->getId()
		<< " De (" << mouv->getOldLigne() << ", " << mouv->getOldColonne() << ", " << mouv->getOldZ() << ")"
		<< " A (" << mouv->getLigne() << ", " << mouv->getColonne() << ", " << mouv->getZ() << ") "
		<< j;
	return oss.str();
}