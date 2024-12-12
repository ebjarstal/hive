#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include "Constantes.h"

class FenetrePrincipale : public QMainWindow {
    Q_OBJECT

public:
    FenetrePrincipale(QWidget* parent = nullptr);
    ~FenetrePrincipale();

private:
    void initPageMenu();
    void initPageNouvellePartie();
    void initPageChargerPartie();

    void afficherNouvellePartie();
    void afficherChargerPartie();
    void retourMenu();

    QGridLayout* gridLayout;
    QStackedWidget* stackedWidget;
    QWidget* widgetCentral;
    QLabel* labelEtat;
    QPushButton* boutonNouvellePartie;
    QPushButton* boutonChargerPartie;
    QPushButton* boutonQuitter;
    QPushButton* boutonRetourNouvellePartie;
    QPushButton* boutonRetourChargerPartie;
};
