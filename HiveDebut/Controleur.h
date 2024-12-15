#pragma once

#include "VuePion.h"
#include "VuePlateau.h"
#include <QObject>
#include <QList>
#include "partie.h"

class Controleur : public QObject {
    Q_OBJECT

public:
    Controleur(Partie* partie, QObject* parent = nullptr);
    Partie* partie;

    // getters

    // setters

private:
    QList<VuePion*> piocheJoueur1;
    QList<VuePion*> piocheJoueur2;

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
