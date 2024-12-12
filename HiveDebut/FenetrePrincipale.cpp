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
    initPageJouerContreIA();
    initPageJouerDeuxJoueurs();
    initPageChargerPartie();

    QVBoxLayout* mainLayout = new QVBoxLayout(widgetCentral);
    mainLayout->addWidget(stackedWidget);

    connect(boutonQuitter, &QPushButton::clicked, this, &FenetrePrincipale::close);
}

FenetrePrincipale::~FenetrePrincipale() {
    // TODO
}

void FenetrePrincipale::initPageMenu() {
    QWidget* pageMenu = new QWidget(this);
    QVBoxLayout* layoutAccueil = new QVBoxLayout(pageMenu);
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

    stackedWidget->addWidget(pageMenu);

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

    // Créer les boutons pour sélectionner le mode de jeu
    boutonJouerContreIA = new QPushButton("Jouer contre une IA", this);
    boutonJouerDeuxJoueurs = new QPushButton("Jouer à deux en local", this);

    // Définir la taille minimale des boutons pour qu'ils soient de la même taille
    boutonJouerContreIA->setMinimumSize(200, 50);
    boutonJouerDeuxJoueurs->setMinimumSize(200, 50);

    // Ajouter les boutons au layout
    QVBoxLayout* layoutModeJeu = new QVBoxLayout();
    layoutModeJeu->addWidget(boutonJouerContreIA);
    layoutModeJeu->addWidget(boutonJouerDeuxJoueurs);
    layoutModeJeu->setAlignment(Qt::AlignCenter);

    layoutNouvellePartie->addLayout(topLayout);
    layoutNouvellePartie->addWidget(labelNouvellePartie);
    layoutNouvellePartie->setAlignment(labelNouvellePartie, Qt::AlignTop | Qt::AlignHCenter);
    layoutNouvellePartie->addStretch(1);
    layoutNouvellePartie->addLayout(layoutModeJeu);
    layoutNouvellePartie->addStretch(1);

    stackedWidget->addWidget(pageNouvellePartie);

    connect(boutonRetourNouvellePartie, &QPushButton::clicked, this, &FenetrePrincipale::retourMenu);
    connect(boutonJouerContreIA, &QPushButton::clicked, this, &FenetrePrincipale::afficherJouerContreIA);
    connect(boutonJouerDeuxJoueurs, &QPushButton::clicked, this, &FenetrePrincipale::afficherJouerDeuxJoueurs);
}

void FenetrePrincipale::initPageJouerContreIA() {
    QWidget* pageJouerContreIA = new QWidget(this);
    QVBoxLayout* layoutJouerContreIA = new QVBoxLayout(pageJouerContreIA);
    boutonRetourJouerContreIA = new QPushButton("Retour", this);
    QLabel* labelJouerContreIA = new QLabel("Jouer contre une IA", this);
    labelJouerContreIA->setAlignment(Qt::AlignCenter);
    labelJouerContreIA->setStyleSheet("font-size: 24px; font-weight: bold;");

    // Ajouter le bouton retour en haut à gauche
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(boutonRetourJouerContreIA);
    topLayout->addStretch(1);

    layoutJouerContreIA->addLayout(topLayout);
    layoutJouerContreIA->addWidget(labelJouerContreIA);
    layoutJouerContreIA->setAlignment(labelJouerContreIA, Qt::AlignTop | Qt::AlignHCenter);

    stackedWidget->addWidget(pageJouerContreIA);

    connect(boutonRetourJouerContreIA, &QPushButton::clicked, this, &FenetrePrincipale::retourNouvellePartie);
}

void FenetrePrincipale::initPageJouerDeuxJoueurs() {
    QWidget* pageJouerDeuxJoueurs = new QWidget(this);
    QVBoxLayout* layoutJouerDeuxJoueurs = new QVBoxLayout(pageJouerDeuxJoueurs);
    boutonRetourJouerDeuxJoueurs = new QPushButton("Retour", this);
    QLabel* labelJouerDeuxJoueurs = new QLabel("Jouer à deux en local", this);
    labelJouerDeuxJoueurs->setAlignment(Qt::AlignCenter);
    labelJouerDeuxJoueurs->setStyleSheet("font-size: 24px; font-weight: bold;");

    // Ajouter le bouton retour en haut à gauche
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(boutonRetourJouerDeuxJoueurs);
    topLayout->addStretch(1);

    layoutJouerDeuxJoueurs->addLayout(topLayout);
    layoutJouerDeuxJoueurs->addWidget(labelJouerDeuxJoueurs);
    layoutJouerDeuxJoueurs->setAlignment(labelJouerDeuxJoueurs, Qt::AlignTop | Qt::AlignHCenter);

    stackedWidget->addWidget(pageJouerDeuxJoueurs);

    connect(boutonRetourJouerDeuxJoueurs, &QPushButton::clicked, this, &FenetrePrincipale::retourNouvellePartie);
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

void FenetrePrincipale::afficherJouerContreIA() {
    // Afficher la page de jouer contre une IA
    stackedWidget->setCurrentIndex(INDEX_JOUER_CONTRE_IA);
}

void FenetrePrincipale::afficherJouerDeuxJoueurs() {
    // Afficher la page de jouer à deux en local
    stackedWidget->setCurrentIndex(INDEX_JOUER_DEUX_JOUEURS);
}

void FenetrePrincipale::afficherChargerPartie() {
    // Afficher la page de charger partie
    stackedWidget->setCurrentIndex(INDEX_CHARGER_PARTIE);
}

void FenetrePrincipale::retourMenu() {
    // Afficher la page menu
    stackedWidget->setCurrentIndex(INDEX_MENU);
}

void FenetrePrincipale::retourNouvellePartie() {
    // Afficher la page nouvelle partie
    stackedWidget->setCurrentIndex(INDEX_NOUVELLE_PARTIE);
}
