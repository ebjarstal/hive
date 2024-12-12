#pragma once

#include <QObject>
#include "partie.h"

class Controleur : public QObject {
    Q_OBJECT

public:
    Controleur(Partie* partie, QObject* parent = nullptr);
    Partie* partie;

signals:
    void miseAJourPlateau();
    void partieTerminee(const QString& message);

public slots:
    void commencerPartie();
    void jouerTour();
    void annulerMouvement();
};

