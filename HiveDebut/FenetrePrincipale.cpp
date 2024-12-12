// FenetrePrincipale.cpp
#include "FenetrePrincipale.h"

FenetrePrincipale::FenetrePrincipale(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Hive (version Qt)");
    setFixedSize(LARGEUR_ECRAN, HAUTEUR_ECRAN); // Bloquer la taille de la fenêtre

    widgetCentral = new QWidget(this);
    setCentralWidget(widgetCentral);

    // Initialiser les widgets pour la page de chargement de partie
    labelFichierCharge = new QLabel(this);
    labelFichierCharge->setVisible(false);
    boutonLancerJeu = new QPushButton("Lancer le jeu", this);
    boutonLancerJeu->setVisible(false);

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

QWidget* FenetrePrincipale::creerPage(const QString& title, QPushButton*& boutonRetour) {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    boutonRetour = new QPushButton("Retour", this);
    QLabel* labelTitle = new QLabel(title, this);
    labelTitle->setAlignment(Qt::AlignCenter);
    labelTitle->setStyleSheet("font-size: 24px; font-weight: bold;");

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(boutonRetour);
    topLayout->addStretch(1);

    layout->addLayout(topLayout);
    layout->addWidget(labelTitle);
    layout->setAlignment(labelTitle, Qt::AlignTop | Qt::AlignHCenter);
    layout->addStretch(1);

    return page;
}

void FenetrePrincipale::ajouterLabelAvecLineEdit(QVBoxLayout* layout, const QString& labelText, QLineEdit*& lineEdit, int maxWidth) {
    QLabel* label = new QLabel(labelText, this);
    lineEdit = new QLineEdit(this);
    label->setMaximumWidth(maxWidth);
    lineEdit->setMaximumWidth(maxWidth);
    layout->addWidget(label, 0, Qt::AlignCenter);
    layout->addWidget(lineEdit, 0, Qt::AlignCenter);
}

void FenetrePrincipale::ajouterCheckbox(QVBoxLayout* layout, const QString& labelText, QCheckBox*& checkBox, int maxWidth) {
    checkBox = new QCheckBox(labelText, this);
    checkBox->setMaximumWidth(maxWidth);
    layout->addWidget(checkBox, 0, Qt::AlignCenter);
}

void FenetrePrincipale::ajouterBouton(QVBoxLayout* layout, const QString& buttonText, QPushButton*& button, int minWidth, int minHeight, int maxWidth) {
    button = new QPushButton(buttonText, this);
    button->setMinimumSize(minWidth, minHeight);
    button->setMaximumWidth(maxWidth);
    layout->addWidget(button, 0, Qt::AlignCenter);
}

void FenetrePrincipale::initPageMenu() {
    QWidget* pageMenu = creerPage("Bienvenue dans le jeu Hive!", boutonRetourNouvellePartie);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(pageMenu->layout());

    ajouterBouton(layout, "Nouvelle partie", boutonNouvellePartie);
    ajouterBouton(layout, "Charger une sauvegarde", boutonChargerPartie);
    ajouterBouton(layout, "Quitter", boutonQuitter);

    layout->addStretch(1);
    stackedWidget->addWidget(pageMenu);

    connect(boutonNouvellePartie, &QPushButton::clicked, this, &FenetrePrincipale::afficherNouvellePartie);
    connect(boutonChargerPartie, &QPushButton::clicked, this, &FenetrePrincipale::afficherChargerPartie);
}

void FenetrePrincipale::initPageNouvellePartie() {
    QWidget* pageNouvellePartie = creerPage("Nouvelle partie", boutonRetourNouvellePartie);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(pageNouvellePartie->layout());

    ajouterBouton(layout, "Jouer contre une IA", boutonJouerContreIA);
    ajouterBouton(layout, "Jouer à deux en local", boutonJouerDeuxJoueurs);

    layout->addStretch(1);
    stackedWidget->addWidget(pageNouvellePartie);

    connect(boutonRetourNouvellePartie, &QPushButton::clicked, this, &FenetrePrincipale::retourMenu);
    connect(boutonJouerContreIA, &QPushButton::clicked, this, &FenetrePrincipale::afficherJouerContreIA);
    connect(boutonJouerDeuxJoueurs, &QPushButton::clicked, this, &FenetrePrincipale::afficherJouerDeuxJoueurs);
}

void FenetrePrincipale::initPageJouerContreIA() {
    QWidget* pageJouerContreIA = creerPage("Jouer contre une IA", boutonRetourJouerContreIA);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(pageJouerContreIA->layout());

    ajouterLabelAvecLineEdit(layout, "Nom du joueur:", champNomJoueur1);
    ajouterLabelAvecLineEdit(layout, "Nom de la sauvegarde:", champNomSauvegarde);
    ajouterLabelAvecLineEdit(layout, "Nombre de retours en arrière possibles:", champNombreRetours);
    ajouterCheckbox(layout, "Extension Moustique", checkboxExtensionMoustique);
    ajouterCheckbox(layout, "Extension Coccinelle", checkboxExtensionCoccinelle);
    ajouterCheckbox(layout, "Extension Araignée", checkboxExtensionAraignee);
    ajouterBouton(layout, "Commencer la partie", boutonCommencerPartie);

    layout->addStretch(1);
    stackedWidget->addWidget(pageJouerContreIA);

    connect(boutonRetourJouerContreIA, &QPushButton::clicked, this, &FenetrePrincipale::retourNouvellePartie);
}

void FenetrePrincipale::initPageJouerDeuxJoueurs() {
    QWidget* pageJouerDeuxJoueurs = creerPage("Jouer à deux en local", boutonRetourJouerDeuxJoueurs);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(pageJouerDeuxJoueurs->layout());

    ajouterLabelAvecLineEdit(layout, "Nom du joueur 1:", champNomJoueur1);
    ajouterLabelAvecLineEdit(layout, "Nom du joueur 2:", champNomJoueur2);
    ajouterLabelAvecLineEdit(layout, "Nom de la sauvegarde:", champNomSauvegarde);
    ajouterLabelAvecLineEdit(layout, "Nombre de retours en arrière possibles:", champNombreRetours);
    ajouterCheckbox(layout, "Extension Moustique", checkboxExtensionMoustique);
    ajouterCheckbox(layout, "Extension Coccinelle", checkboxExtensionCoccinelle);
    ajouterCheckbox(layout, "Extension Araignée", checkboxExtensionAraignee);
    ajouterBouton(layout, "Commencer la partie", boutonCommencerPartie);

    layout->addStretch(1);
    stackedWidget->addWidget(pageJouerDeuxJoueurs);

    connect(boutonRetourJouerDeuxJoueurs, &QPushButton::clicked, this, &FenetrePrincipale::retourNouvellePartie);
}

void FenetrePrincipale::initPageChargerPartie() {
    QWidget* pageChargerPartie = creerPage("Charger partie", boutonRetourChargerPartie);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(pageChargerPartie->layout());

    ajouterBouton(layout, "Sélectionner une sauvegarde", boutonOuvrirFichier);
    labelCheminFichier = new QLabel(this);
    labelCheminFichier->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelCheminFichier, 0, Qt::AlignCenter);

    labelFichierCharge = new QLabel(this);
    labelFichierCharge->setAlignment(Qt::AlignCenter);
    labelFichierCharge->setStyleSheet("color: green; font-weight: bold;");
    layout->addWidget(labelFichierCharge, 0, Qt::AlignCenter);

    ajouterBouton(layout, "Lancer le jeu", boutonLancerJeu);

    layout->addStretch(1);
    stackedWidget->addWidget(pageChargerPartie);

    connect(boutonRetourChargerPartie, &QPushButton::clicked, this, &FenetrePrincipale::retourMenu);
    connect(boutonOuvrirFichier, &QPushButton::clicked, this, &FenetrePrincipale::ouvrirFileDialog);
}

void FenetrePrincipale::ouvrirFileDialog() {
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier de sauvegarde", "", "Fichiers texte (*.txt)");
    if (!fichier.isEmpty()) {
        labelCheminFichier->setText(fichier);
        labelFichierCharge->setText("Fichier de sauvegarde chargé");
        boutonLancerJeu->setVisible(true);
        // Vous pouvez ajouter ici le code pour charger la partie à partir du fichier sélectionné
    }
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
