#include "Controleur.h"
#include "gestionnaireSauvegarde.h"
#include "VuePion.h"

Controleur::Controleur(Partie* partie, QObject* parent)
    : QObject(parent), partie(partie) {
}

void Controleur::commencerPartie() {
    // emit miseAJourPlateau();

    GestionnaireSauvegarde::sauvegarde(*partie);  // sauvegarde la nouvelle partie
    partie->getPlateau().afficher();  // affiche le plateau en console (a supprimer)

    emit afficherPlateauDebut();  // affiche le plateau en qt
    emit afficherPiochesDebut();  // affiche les pioches en qt

    // jouerTour() fait crash l'app qt
}

void Controleur::jouerTour() {
    Joueur* joueurCourant = (partie->getNombreTour() % 2 == 1) ? partie->getJoueur1() : partie->getJoueur2();
    partie->jouerUnTour(joueurCourant);
    emit miseAJourPlateau();

    // D�terminer le gagnant apr�s chaque tour
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