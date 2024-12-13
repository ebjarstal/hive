#include "Controleur.h"
#include "gestionnaireSauvegarde.h"
#include "VuePion.h"

Controleur::Controleur(Partie* partie, QObject* parent)
    : QObject(parent), partie(partie) {
}

void Controleur::commencerPartie() {
    emit miseAJourPlateau();

    GestionnaireSauvegarde::sauvegarde(*partie);
    partie->getPlateau().afficher();
    // afficher le plateau en qt

    // Créer et ajouter un pion à la scène
    VuePion* pion = new VuePion();
    pion->setCouleur(Qt::darkRed);
    pion->setType("R");
    emit afficherPlateauScene(pion);

    // jouerTour() fait crash l'app qt
}

void Controleur::jouerTour() {
    Joueur* joueurCourant = (partie->getNombreTour() % 2 == 1) ? partie->getJoueur1() : partie->getJoueur2();
    partie->jouerUnTour(joueurCourant);
    emit miseAJourPlateau();

    // Déterminer le gagnant après chaque tour
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
