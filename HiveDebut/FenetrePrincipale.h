#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QFileDialog>
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

    void ouvrirFileDialog();

    QGridLayout* gridLayout;
    QStackedWidget* stackedWidget;
    QWidget* widgetCentral;
    QLabel* labelEtat;

    QPushButton* boutonNouvellePartie;
    QPushButton* boutonJouerContreIA;
    QPushButton* boutonJouerDeuxJoueurs;
    QPushButton* boutonChargerPartie;
    QPushButton* boutonQuitter;

    QLabel* labelFichierCharge;
    QPushButton* boutonLancerJeu;

    QPushButton* boutonRetourNouvellePartie;
    QPushButton* boutonRetourJouerContreIA;
    QPushButton* boutonRetourJouerDeuxJoueurs;
    QPushButton* boutonRetourChargerPartie;

    QPushButton* boutonOuvrirFichier;
    QLabel* labelCheminFichier;
};
