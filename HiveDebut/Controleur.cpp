#include "Controleur.h"
#include "gestionnaireSauvegarde.h"
#include "VuePion.h"

#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

Controleur::Controleur(Partie* partie, QObject* parent) : QObject(parent), partie(partie) {}

void Controleur::commencerPartie() {
    // emit miseAJourPlateau();

    GestionnaireSauvegarde::sauvegarde(*partie);  // sauvegarde la nouvelle partie
    partie->getPlateau().afficher();  // affiche le plateau en console (a supprimer)

    if (partie->getNombreTour() % 2 == 1) {
        aQuiDeJouer = QString::fromStdString(partie->getJoueur1()->getNom());
    }
    else {
        aQuiDeJouer = QString::fromStdString(partie->getJoueur2()->getNom());
    }

    // cree les pioches de chaque joueur
    for (size_t i = 0; i < partie->getJoueur1()->getPionsEnMain().size(); i++) {
        VuePion* pion = new VuePion();
        pion->setAttributs(std::string(RED), false, QString::fromStdString(partie->getJoueur1()->getPionsEnMain()[i]->getType()));
        pion->setPionAssocie(partie->getJoueur1()->getPionsEnMain()[i]);
        piocheJoueur1.append(pion);
    }

    for (size_t j = 0; j < partie->getJoueur2()->getPionsEnMain().size(); j++) {
        VuePion* pion = new VuePion();
        pion->setAttributs(std::string(WHITE), false, QString::fromStdString(partie->getJoueur2()->getPionsEnMain()[j]->getType()));
        pion->setPionAssocie(partie->getJoueur1()->getPionsEnMain()[j]);
        piocheJoueur2.append(pion);
    }

    emit afficherPlateauDebut();  // affiche le plateau en qt
    emit afficherPiochesDebut();  // affiche les pioches en qt

    // jouerTour() fait crash l'app qt
}

void Controleur::setAQuiDeJouer(QString nomJoueur) {
    aQuiDeJouer = nomJoueur;
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

void Controleur::placerPion(VuePion* pionAPlacer, VuePion* pionARemplacer) {
    // encapsuler logique
    std::cout << "faire la methode Controleur::placerPion" << std::endl;
}

void Controleur::deplacerPion(VuePion* pionADeplacer, VuePion* pionARemplacer) {
    // encapsuler logique
    std::cout << "faire la methode Controleur::deplacerPion" << std::endl;
}
