#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QButtonGroup>
#include "Constantes.h"

class FenetrePrincipale : public QMainWindow {
    Q_OBJECT

public:
    FenetrePrincipale(QWidget* parent = nullptr);
    ~FenetrePrincipale();

private:
    void initPageMenu();
    void initPageNouvellePartie();
    void initPageJouerContreIA();
    void initPageJouerDeuxJoueurs();
    void initPageChargerPartie();

    void afficherNouvellePartie();
    void afficherJouerContreIA();
    void afficherJouerDeuxJoueurs();
    void afficherChargerPartie();

    void retourMenu();
    void retourNouvellePartie();

    QGridLayout* gridLayout;
    QStackedWidget* stackedWidget;
    QWidget* widgetCentral;
    QLabel* labelEtat;

    QPushButton* boutonNouvellePartie;
    QPushButton* boutonJouerContreIA;
    QPushButton* boutonJouerDeuxJoueurs;
    QPushButton* boutonChargerPartie;
    QPushButton* boutonQuitter;

    QPushButton* boutonRetourNouvellePartie;
    QPushButton* boutonRetourJouerContreIA;
    QPushButton* boutonRetourJouerDeuxJoueurs;
    QPushButton* boutonRetourChargerPartie;
};
