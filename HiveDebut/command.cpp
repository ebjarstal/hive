#include "command.h"
#include "plateau.h"
#include "pions.h"
#include "joueur.h"
#include "mouvement.h"

void PoserPionCommand::execute() {
    plateau.gestionnairePions.setPion(mouv->getLigne(), mouv->getColonne(), mouv->getZ(), pion, plateau);
    joueur.pionsEnMain.erase(std::remove(joueur.pionsEnMain.begin(), joueur.pionsEnMain.end(), pion), joueur.pionsEnMain.end());
}

void DeplacerPionCommand::execute() {
    plateau.gestionnairePions.movePion(mouv->getLigne(), mouv->getColonne(), mouv->getZ(), pion, plateau);

}