#include "FenetrePrincipale.h"

FenetrePrincipale::FenetrePrincipale(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Hive (version Qt)");
    setFixedSize(LARGEUR_ECRAN, HAUTEUR_ECRAN); // Bloquer la taille de la fenêtre

    widgetCentral = new QWidget(this);
    setCentralWidget(widgetCentral);

    stackedWidget = new QStackedWidget(this);

    // Initialiser les pages
    initPageMenu();
    initPageNouvellePartie();
    initPageChargerPartie();

    QVBoxLayout* mainLayout = new QVBoxLayout(widgetCentral);
    mainLayout->addWidget(stackedWidget);

    connect(boutonQuitter, &QPushButton::clicked, this, &FenetrePrincipale::close);
}

FenetrePrincipale::~FenetrePrincipale() {
    // TODO
}

void FenetrePrincipale::initPageMenu() {
    QWidget* pageAccueil = new QWidget(this);
    QVBoxLayout* layoutAccueil = new QVBoxLayout(pageAccueil);
    labelEtat = new QLabel("Bienvenue dans le jeu Hive!", this);
    boutonNouvellePartie = new QPushButton("Nouvelle partie", this);
    boutonChargerPartie = new QPushButton("Charger une sauvegarde", this);
    boutonQuitter = new QPushButton("Quitter", this);

    // Centrer le labelEtat en haut et augmenter la taille de la police
    labelEtat->setAlignment(Qt::AlignCenter);
    labelEtat->setStyleSheet("font-size: 24px; font-weight: bold;");

    // Définir la taille minimale des boutons pour qu'ils soient de la même taille
    QSize buttonSize(200, 50);
    boutonNouvellePartie->setMinimumSize(buttonSize);
    boutonChargerPartie->setMinimumSize(buttonSize);
    boutonQuitter->setMinimumSize(buttonSize);

    // Ajouter les widgets à la page d'accueil
    layoutAccueil->addWidget(labelEtat);
    layoutAccueil->setAlignment(labelEtat, Qt::AlignTop | Qt::AlignHCenter);
    layoutAccueil->addStretch(1);
    layoutAccueil->addWidget(boutonNouvellePartie);
    layoutAccueil->setAlignment(boutonNouvellePartie, Qt::AlignCenter);
    layoutAccueil->addWidget(boutonChargerPartie);
    layoutAccueil->setAlignment(boutonChargerPartie, Qt::AlignCenter);
    layoutAccueil->addWidget(boutonQuitter);
    layoutAccueil->setAlignment(boutonQuitter, Qt::AlignCenter);
    layoutAccueil->addStretch(1);

    stackedWidget->addWidget(pageAccueil);

    connect(boutonNouvellePartie, &QPushButton::clicked, this, &FenetrePrincipale::afficherNouvellePartie);
    connect(boutonChargerPartie, &QPushButton::clicked, this, &FenetrePrincipale::afficherChargerPartie);
}

void FenetrePrincipale::initPageNouvellePartie() {
    QWidget* pageNouvellePartie = new QWidget(this);
    QVBoxLayout* layoutNouvellePartie = new QVBoxLayout(pageNouvellePartie);
    boutonRetourNouvellePartie = new QPushButton("Retour", this);
    QLabel* labelNouvellePartie = new QLabel("Nouvelle partie", this);
    labelNouvellePartie->setAlignment(Qt::AlignCenter);
    labelNouvellePartie->setStyleSheet("font-size: 24px; font-weight: bold;");

    // Ajouter le bouton retour en haut à gauche
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(boutonRetourNouvellePartie);
    topLayout->addStretch(1);

    layoutNouvellePartie->addLayout(topLayout);
    layoutNouvellePartie->addWidget(labelNouvellePartie);
    layoutNouvellePartie->setAlignment(labelNouvellePartie, Qt::AlignTop | Qt::AlignHCenter);

    stackedWidget->addWidget(pageNouvellePartie);

    connect(boutonRetourNouvellePartie, &QPushButton::clicked, this, &FenetrePrincipale::retourMenu);
}

void FenetrePrincipale::initPageChargerPartie() {
    QWidget* pageChargerPartie = new QWidget(this);
    QVBoxLayout* layoutChargerPartie = new QVBoxLayout(pageChargerPartie);
    boutonRetourChargerPartie = new QPushButton("Retour", this);
    QLabel* labelChargerPartie = new QLabel("Charger partie", this);
    labelChargerPartie->setAlignment(Qt::AlignCenter);
    labelChargerPartie->setStyleSheet("font-size: 24px; font-weight: bold;");

    // Ajouter le bouton retour en haut à gauche
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(boutonRetourChargerPartie);
    topLayout->addStretch(1);

    layoutChargerPartie->addLayout(topLayout);
    layoutChargerPartie->addWidget(labelChargerPartie);
    layoutChargerPartie->setAlignment(labelChargerPartie, Qt::AlignTop | Qt::AlignHCenter);

    stackedWidget->addWidget(pageChargerPartie);

    connect(boutonRetourChargerPartie, &QPushButton::clicked, this, &FenetrePrincipale::retourMenu);
}

void FenetrePrincipale::afficherNouvellePartie() {
    // Afficher la page de nouvelle partie
    stackedWidget->setCurrentIndex(INDEX_NOUVELLE_PARTIE);
}

void FenetrePrincipale::afficherChargerPartie() {
    // Afficher la page de charger partie
    stackedWidget->setCurrentIndex(INDEX_CHARGER_PARTIE);
}

void FenetrePrincipale::retourMenu() {
    // Afficher la page menu
    stackedWidget->setCurrentIndex(INDEX_MENU);
}
