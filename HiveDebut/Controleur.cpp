#include "Controleur.h"
#include "gestionnaireSauvegarde.h"
#include "VuePion.h"

#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

Controleur::Controleur(Partie* partie, QObject* parent) : QObject(parent), partie(partie), vuePlateau(nullptr) {}

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

    updatePioches();

    emit afficherPlateauDebut();  // affiche le plateau en qt
    emit afficherPiochesEtAQuiDeJouer();  // affiche les pioches en qt

    // jouerTour() fait crash l'app qt
}

void Controleur::updatePioches() {
    // Supprimer les VuePion existants dans les pioches
    piocheJoueur1.clear();
    piocheJoueur2.clear();

    // Recréer les pioches de chaque joueur
    for (size_t i = 0; i < partie->getJoueur1()->getPionsEnMain().size(); i++) {
        VuePion* pion = new VuePion();
        pion->setAttributs(std::string(RED), false, QString::fromStdString(partie->getJoueur1()->getPionsEnMain()[i]->getType()));
        pion->setPionAssocie(partie->getJoueur1()->getPionsEnMain()[i]);
        piocheJoueur1.append(pion);
    }
    for (size_t j = 0; j < partie->getJoueur2()->getPionsEnMain().size(); j++) {
        VuePion* pion = new VuePion();
        pion->setAttributs(std::string(WHITE), false, QString::fromStdString(partie->getJoueur2()->getPionsEnMain()[j]->getType()));
        pion->setPionAssocie(partie->getJoueur2()->getPionsEnMain()[j]);
        piocheJoueur2.append(pion);
    }
}

void Controleur::faireMouvement(VuePion* pionABouger, VuePion* pionARemplacer) {
    // s'assurer que les pions sont bien definis
    if (pionABouger == nullptr || pionARemplacer == nullptr) {
        return;
    }

    // s'assurer que le pion a remplacer est bien un pion deja pose sur le plateau
    if (pionARemplacer->getEstPose() == false) {
        return;
    }

    // determiner s'il s'agit d'un emplacement ou d'un deplacement
    // s'il s'agit d'un emplacement:
    if (pionABouger->getEstPose() == false) {
        placerPion(pionABouger, pionARemplacer);
    }
    // s'il s'agit d'un deplacement
    else {
        deplacerPion(pionABouger, pionARemplacer);
    }
}

void Controleur::placerPion(VuePion* pionAPlacer, VuePion* pionARemplacer) {
    int new_ligne = pionARemplacer->getLigne();
    int new_colonne = pionARemplacer->getColonne();
    int new_couche = pionARemplacer->getZ();

    // les trois lignes suivantes permettent d'executer le placement du pion du cote du vrai plateau
    Mouvement* emplacementChoisi = new Mouvement(pionAPlacer->getPionAssocie()->getId(), new_ligne, new_colonne, new_couche, -1, -1, -1);
    auto poserPionCommand = new MouvementCommand(*partie, emplacementChoisi);
    GestionnaireCommand::executeCommand(*partie, poserPionCommand);

    // retirer le pion de la pioche du joueur1
    if (piocheJoueur1.removeOne(pionAPlacer)) {
        piocheJoueur1.resize(piocheJoueur1.size());
    }
    // on check pour joueur2
    else if (piocheJoueur2.removeOne(pionAPlacer)) {
        piocheJoueur2.resize(piocheJoueur2.size());
    }
}


void Controleur::deplacerPion(VuePion* pionADeplacer, VuePion* pionARemplacer) {
    int old_ligne = pionADeplacer->getPionAssocie()->getLigne();
    int old_colonne = pionADeplacer->getPionAssocie()->getColonne();
    int old_couche = pionADeplacer->getPionAssocie()->getZ();

    int new_ligne = pionARemplacer->getLigne();
    int new_colonne = pionARemplacer->getColonne();
    int new_couche = pionARemplacer->getZ();

    // les trois lignes suivantes permettent d'executer le placement du pion du cote du vrai plateau
    Mouvement* emplacementChoisi = new Mouvement(pionADeplacer->getPionAssocie()->getId(), new_ligne, new_colonne, new_couche, old_ligne, old_colonne, old_couche);
    auto poserPionCommand = new MouvementCommand(*partie, emplacementChoisi);
    GestionnaireCommand::executeCommand(*partie, poserPionCommand);

    // retirer le pion de la pioche du joueur1
    if (piocheJoueur1.removeOne(pionADeplacer)) {
        piocheJoueur1.resize(piocheJoueur1.size());
    }
    // on check pour joueur2
    else if (piocheJoueur2.removeOne(pionADeplacer)) {
        piocheJoueur2.resize(piocheJoueur2.size());
    }
}

void Controleur::setAQuiDeJouer(QString nomJoueur) {
    aQuiDeJouer = nomJoueur;
}

void Controleur::jouerTour() {
    // Alterner le joueur courant
    if (aQuiDeJouer == QString::fromStdString(partie->getJoueur1()->getNom())) {
        aQuiDeJouer = QString::fromStdString(partie->getJoueur2()->getNom());
    }
    else {
        aQuiDeJouer = QString::fromStdString(partie->getJoueur1()->getNom());
    }

    // Incrémenter le nombre de tours joués
    partie->setNombreTour(partie->getNombreTour() + 1);

    // Sauvegarder la partie après chaque tour
    GestionnaireSauvegarde::sauvegarde(*partie);

    // Déterminer le gagnant après chaque tour
    if (partie->partieTerminee()) {
        Joueur* gagnant = partie->determinerGagnant();
        QString message = gagnant ? QString("%1 gagne !!").arg(QString::fromStdString(gagnant->getNom())) : "Egalite !!";
        emit partieTerminee(message);
    }
}

void Controleur::annulerMouvement() {
    partie->annulerMouvement();
}

void Controleur::updateVuePlateau() {
    // Supprimer les VuePion existants
    //QList<VuePion*> listePions = vuePlateau->getListePionsPlateau();
    //for (VuePion* pion : listePions) {
    //    delete pion;
    //}

    // Réinitialiser le plateau
    vuePlateau->initialiserPlateau(190, 100);
}
