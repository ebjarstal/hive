#pragma once

#include "Constantes.h"
#include "VuePion.h"
#include "VuePlateau.h"
#include <QObject>
#include <QList>
#include <string>
#include "partie.h"
#include "mouvement.h"
#include "gestionnaireCommand.h"

class Controleur : public QObject {
    Q_OBJECT

public:
    Controleur(Partie* partie, QObject* parent = nullptr);
    Partie* partie;
    VuePlateau* vuePlateau;

    void faireMouvement(VuePion* pionABouger, VuePion* pionARemplacer);
    void placerPion(VuePion* pionAPlacer, VuePion* pionARemplacer);
    void deplacerPion(VuePion* pionADeplacer, VuePion* pionARemplacer);

    // getters
    QList<VuePion*> getPiocheJoueur1() { return piocheJoueur1; }
    QList<VuePion*> getPiocheJoueur2() { return piocheJoueur2; }
    QString getAQuiDeJouer() const { return aQuiDeJouer; }

    // setters
    void setAQuiDeJouer(QString nomJoueur);

    // updates
    void updateVuePlateau();
    void updatePioches();

private:
    QList<VuePion*> piocheJoueur1;
    QList<VuePion*> piocheJoueur2;
    QString aQuiDeJouer;

signals:
    void afficherPlateauDebut();
    void afficherPiochesEtAQuiDeJouer();
    void miseAJourPlateau();
    void partieTerminee(const QString& message);

public slots:
    void commencerPartie();
    void jouerTour();
    void annulerMouvement();
};
