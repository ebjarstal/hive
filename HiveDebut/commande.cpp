#include "commande.h"

MouvementCommande::MouvementCommande(Mouvement* m, Partie& p) : mouvement(m), partie(p) {
    pion = Pion::getPionById(mouvement->getPionId());
    joueur = partie.getJoueur1();
    if (joueur->getCouleur() != pion->getCouleur()) {
        joueur = partie.getJoueur2();
    }
}

void MouvementCommande::execute() {
    if (mouvement->getOldLigne() == -1 && mouvement->getOldColonne() == -1 && mouvement->getOldZ() == -1) {

        partie.getPlateau().getGestionnairePions().setPion(mouvement->getLigne(), mouvement->getColonne(), mouvement->getZ(), pion, partie.getPlateau());
        auto it = std::find(joueur->getPionsEnMain().begin(), joueur->getPionsEnMain().end(), pion);

        if (it != joueur->getPionsEnMain().end()) {
            joueur->getPionsEnMain().erase(it);
        }
    }
    else {
        partie.getPlateau().getGestionnairePions().movePion(mouvement->getLigne(), mouvement->getColonne(), mouvement->getZ(), pion, partie.getPlateau());
    }
}

void MouvementCommande::undo() {
    if (mouvement->getOldLigne() == -1 && mouvement->getOldColonne() == -1 && mouvement->getOldZ() == -1) {
        partie.getPlateau().getGestionnairePions().deletePion(*pion, partie.getPlateau());
        joueur->getPionsEnMain().push_back(pion);
    }
    else {
        partie.getPlateau().getGestionnairePions().movePion(mouvement->getOldLigne(), mouvement->getOldColonne(), mouvement->getOldZ(), pion, partie.getPlateau());
    }
}