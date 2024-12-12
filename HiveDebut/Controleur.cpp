#include "Controleur.h"

Controleur::Controleur(Partie* partie, QObject* parent)
    : QObject(parent), partie(partie) {
}

void Controleur::commencerPartie() {
    // partie->setup();
    emit miseAJourPlateau();
}

void Controleur::jouerTour() {
    Joueur* joueurCourant = (partie->getNombreTour() % 2 == 1) ? partie->getJoueur1() : partie->getJoueur2();
    partie->jouerUnTour(joueurCourant);
    emit miseAJourPlateau();

    if (partie->partieTerminee()) {
        Joueur* gagnant = partie->determinerGagnant();
        QString message = gagnant ? QString("%1 gagne !!").arg(QString::fromStdString(gagnant->getNom())) : "Egalite !!";
        emit partieTerminee(message);
    }
}

void Controleur::annulerMouvement() {
    partie->annulerMouvement();
    emit miseAJourPlateau();
}
