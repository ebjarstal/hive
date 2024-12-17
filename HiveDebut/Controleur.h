#pragma once

#include "VuePion.h"
#include "VuePlateau.h"
#include <QObject>
#include <QList>
#include <string>
#include "partie.h"

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
    QList<VuePion*> getPiocheJoueur1() const { return piocheJoueur1; }
    QList<VuePion*> getPiocheJoueur2() const { return piocheJoueur2; }
    QString getAQuiDeJouer() const { return aQuiDeJouer; }

    // setters
    void setAQuiDeJouer(QString nomJoueur);

private:
    QList<VuePion*> piocheJoueur1;
    QList<VuePion*> piocheJoueur2;
    QString aQuiDeJouer;

signals:
    void afficherPlateauDebut();
    void afficherPiochesDebut();
    void miseAJourPlateau();
    void partieTerminee(const QString& message);

public slots:
    void commencerPartie();
    void jouerTour();
    void annulerMouvement();
};
