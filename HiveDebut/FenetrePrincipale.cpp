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

    pionEnCoursDeTraitement = nullptr;

    QVBoxLayout* mainLayout = new QVBoxLayout(widgetCentral);
    mainLayout->addWidget(stackedWidget);

    connect(boutonQuitter, &QPushButton::clicked, this, &FenetrePrincipale::close);

    // Initialiser le contrôleur de jeu
    Plateau* plateau = new Plateau(TAILLE_PLATEAU, TAILLE_PLATEAU, 5);
    Partie* partie = new Partie(*plateau);
    controleur = new Controleur(partie, this);

    // Connecter les signaux et les slots
    connect(controleur, &Controleur::miseAJourPlateau, this, &FenetrePrincipale::onMiseAJourPlateau);
    connect(controleur, &Controleur::partieTerminee, this, &FenetrePrincipale::onPartieTerminee);
    connect(controleur, &Controleur::afficherPlateauDebut, this, &FenetrePrincipale::afficherPlateauDebut);
    connect(controleur, &Controleur::afficherPiochesDebut, this, &FenetrePrincipale::afficherPiochesDebut);
    connect(boutonCommencerPartieContreIA, &QPushButton::clicked, this, &FenetrePrincipale::commencerPartieContreIA);
    connect(boutonCommencerPartieDeuxJoueurs, &QPushButton::clicked, this, &FenetrePrincipale::commencerPartieDeuxJoueurs);
}

FenetrePrincipale::~FenetrePrincipale() {
    // TODO
}

void FenetrePrincipale::onMiseAJourPlateau() {
    std::cout << "Execution de onMiseAJourPlateau()" << std::endl;
}

void FenetrePrincipale::onPartieTerminee(const QString& message) {
    // Afficher un message de fin de partie
    QMessageBox::information(this, "Partie terminée", message);
}

QWidget* FenetrePrincipale::creerPage(const QString& title, QPushButton*& boutonRetour, bool ajouterBoutonRetour) {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    QLabel* labelTitle = new QLabel(title, this);
    labelTitle->setAlignment(Qt::AlignCenter);
    labelTitle->setStyleSheet("font-size: 24px; font-weight: bold;");

    if (ajouterBoutonRetour) {
        boutonRetour = new QPushButton("Retour", this);
        QHBoxLayout* topLayout = new QHBoxLayout();
        topLayout->addWidget(boutonRetour);
        topLayout->addStretch(1);
        layout->addLayout(topLayout);
    }
    else {
        boutonRetour = nullptr;
    }

    layout->addWidget(labelTitle);
    layout->setAlignment(labelTitle, Qt::AlignTop | Qt::AlignHCenter);
    layout->addStretch(1);

    return page;
}

void FenetrePrincipale::ajouterLabelAvecLineEditTexte(QVBoxLayout* layout, const QString& labelText, QLineEdit*& lineEdit, int maxWidth) {
    QLabel* label = new QLabel(labelText, this);
    lineEdit = new QLineEdit(this);
    lineEdit->setText(QString("Texte"));
    label->setMaximumWidth(maxWidth);
    lineEdit->setMaximumWidth(maxWidth);
    layout->addWidget(label, 0, Qt::AlignCenter);
    layout->addWidget(lineEdit, 0, Qt::AlignCenter);
}

void FenetrePrincipale::ajouterLabelAvecLineEditNombre(QVBoxLayout* layout, const QString& labelText, QLineEdit*& lineEdit, int maxWidth) {
    QLabel* label = new QLabel(labelText, this);
    lineEdit = new QLineEdit(this);
    lineEdit->setText(QString("0"));
    lineEdit->setValidator(new QIntValidator(0, 100, this)); // Limite de 0 à 100 par exemple
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
    QWidget* pageMenu = creerPage("Bienvenue dans le jeu Hive!", boutonRetourNouvellePartie, false);
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

    ajouterLabelAvecLineEditTexte(layout, "Nom du joueur:", champNomJoueur1IA);
    ajouterLabelAvecLineEditTexte(layout, "Nom de la sauvegarde:", champNomSauvegardeIA);
    ajouterLabelAvecLineEditNombre(layout, "Nombre de retours en arrière possibles:", champNombreRetoursIA);
    ajouterCheckbox(layout, "Extension Moustique", checkboxExtensionMoustique);
    ajouterCheckbox(layout, "Extension Coccinelle", checkboxExtensionCoccinelle);
    ajouterCheckbox(layout, "Extension Araignée", checkboxExtensionAraignee);
    ajouterBouton(layout, "Commencer la partie", boutonCommencerPartieContreIA);

    layout->addStretch(1);
    stackedWidget->addWidget(pageJouerContreIA);

    connect(boutonRetourJouerContreIA, &QPushButton::clicked, this, &FenetrePrincipale::retourNouvellePartie);
}

void FenetrePrincipale::initPageJouerDeuxJoueurs() {
    QWidget* pageJouerDeuxJoueurs = creerPage("Jouer à deux en local", boutonRetourJouerDeuxJoueurs);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(pageJouerDeuxJoueurs->layout());

    ajouterLabelAvecLineEditTexte(layout, "Nom du joueur 1:", champNomJoueur1DeuxJoueurs);
    ajouterLabelAvecLineEditTexte(layout, "Nom du joueur 2:", champNomJoueur2DeuxJoueurs);
    ajouterLabelAvecLineEditTexte(layout, "Nom de la sauvegarde:", champNomSauvegardeDeuxJoueurs);
    ajouterLabelAvecLineEditNombre(layout, "Nombre de retours en arrière possibles:", champNombreRetoursDeuxJoueurs);
    ajouterCheckbox(layout, "Extension Moustique", checkboxExtensionMoustique);
    ajouterCheckbox(layout, "Extension Coccinelle", checkboxExtensionCoccinelle);
    ajouterCheckbox(layout, "Extension Araignée", checkboxExtensionAraignee);
    ajouterBouton(layout, "Commencer la partie", boutonCommencerPartieDeuxJoueurs);

    layout->addStretch(1);
    stackedWidget->addWidget(pageJouerDeuxJoueurs);

    connect(boutonRetourJouerDeuxJoueurs, &QPushButton::clicked, this, &FenetrePrincipale::retourNouvellePartie);
}

void FenetrePrincipale::initPageChargerPartie() {
    QWidget* pageChargerPartie = creerPage("Charger partie", boutonRetourChargerPartie);
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(pageChargerPartie->layout());

    ajouterBouton(layout, "Sélectionner une sauvegarde", boutonOuvrirFichier);
    labelNomSauvegarde = new QLabel(this);
    labelNomSauvegarde->setAlignment(Qt::AlignCenter);
    layout->addWidget(labelNomSauvegarde, 0, Qt::AlignCenter);

    labelFichierCharge = new QLabel(this);
    labelFichierCharge->setAlignment(Qt::AlignCenter);
    labelFichierCharge->setStyleSheet("color: green; font-weight: bold;");
    layout->addWidget(labelFichierCharge, 0, Qt::AlignCenter);

    ajouterBouton(layout, "Lancer le jeu", boutonChargerPartieSauvegarde);

    layout->addStretch(1);
    stackedWidget->addWidget(pageChargerPartie);

    connect(boutonRetourChargerPartie, &QPushButton::clicked, this, &FenetrePrincipale::retourMenu);
    connect(boutonOuvrirFichier, &QPushButton::clicked, this, &FenetrePrincipale::ouvrirFileDialog);
    connect(boutonChargerPartieSauvegarde, &QPushButton::clicked, this, &FenetrePrincipale::chargerPartieSauvegarde);
}

void FenetrePrincipale::ouvrirFileDialog() {
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier de sauvegarde", "", "Fichiers texte (*.txt)");
    if (!fichier.isEmpty()) {
        QFileInfo fileInfo(fichier);
        QString nomFichier = fileInfo.completeBaseName(); // Obtenir le nom du fichier sans l'extension
        labelNomSauvegarde->setText(nomFichier);
        labelFichierCharge->setText("Fichier de sauvegarde chargé");
        boutonChargerPartieSauvegarde->setVisible(true);
    }
}

void FenetrePrincipale::commencerPartieContreIA() {
    QString nomJoueur = champNomJoueur1IA->text();
    QString nomSauvegarde = champNomSauvegardeIA->text();
    bool ok;
    int nbUndo = champNombreRetoursIA->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Erreur", "Le nombre de retours en arrière doit être un entier.");
        return;
    }

    std::cout << "nomJoueur: " << nomJoueur.toStdString() << std::endl;
    std::cout << "nomSauvegarde: " << nomSauvegarde.toStdString() << std::endl;
    std::cout << "nbUndo: " << nbUndo << std::endl;

    Joueur* joueur1 = new JoueurHumain(nomJoueur.toStdString(), controleur->partie->initialiserPions(RED), RED, *(controleur->partie));
    Joueur* joueur2 = new JoueurIA("IA", controleur->partie->initialiserPions(WHITE), WHITE, *(controleur->partie));

    controleur->partie->setNomPartie(nomSauvegarde.toStdString());
    controleur->partie->setNbUndo(nbUndo);
    controleur->partie->setJoueur1(joueur1);
    controleur->partie->setJoueur2(joueur2);
    // partie->setExtensions(extensionMoustique, extensionCoccinelle, extensionAraignee);

    controleur->commencerPartie();
}

void FenetrePrincipale::commencerPartieDeuxJoueurs() {
    QString nomJoueur1 = champNomJoueur1DeuxJoueurs->text();
    QString nomJoueur2 = champNomJoueur2DeuxJoueurs->text();
    QString nomSauvegarde = champNomSauvegardeDeuxJoueurs->text();
    bool ok;
    int nbUndo = champNombreRetoursDeuxJoueurs->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Erreur", "Le nombre de retours en arrière doit être un entier.");
        return;
    }

    std::cout << "nomJoueur1: " << nomJoueur1.toStdString() << std::endl;
    std::cout << "nomJoueur2: " << nomJoueur2.toStdString() << std::endl;
    std::cout << "nomSauvegarde: " << nomSauvegarde.toStdString() << std::endl;
    std::cout << "nbUndo: " << nbUndo << std::endl;

    Joueur* joueur1 = new JoueurHumain(nomJoueur1.toStdString(), controleur->partie->initialiserPions(RED), RED, *(controleur->partie));
    Joueur* joueur2 = new JoueurHumain(nomJoueur2.toStdString(), controleur->partie->initialiserPions(WHITE), WHITE, *(controleur->partie));

    controleur->partie->setNomPartie(nomSauvegarde.toStdString());
    controleur->partie->setNbUndo(nbUndo);
    controleur->partie->setJoueur1(joueur1);
    controleur->partie->setJoueur2(joueur2);
    // partie->setExtensions(extensionMoustique, extensionCoccinelle, extensionAraignee);

    controleur->commencerPartie();
}

void FenetrePrincipale::chargerPartieSauvegarde() {
    QString nomFichierSauvegarde = labelNomSauvegarde->text();
    if (nomFichierSauvegarde.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun fichier de sauvegarde sélectionné.");
        return;
    }

    std::string nomFichierSauvegardeStd = nomFichierSauvegarde.toStdString();
    Partie* partie = controleur->partie;
    partie->setNomPartie(nomFichierSauvegardeStd);

    if (GestionnaireSauvegarde::chargerPartie(*partie)) {
        std::cout << "Partie chargée avec succès depuis " << nomFichierSauvegardeStd << std::endl;
        controleur->commencerPartie();
        stackedWidget->setCurrentIndex(INDEX_PARTIE_EN_COURS); // Rediriger vers la page "Partie en cours"
    }
    else {
        QMessageBox::warning(this, "Erreur", "Erreur lors du chargement de la partie.");
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

void FenetrePrincipale::afficherPlateauDebut() {
    scene = new QGraphicsScene(this);

    // Créer une instance de VuePlateau en utilisant le plateau de la partie
    vuePlateau = new VuePlateau(scene, &(controleur->partie->getPlateau()));
    vuePlateau->initialiserPlateau(190, 100);

    // Définir le fond de la scène en blanc
    scene->setBackgroundBrush(Qt::white);

    vuePartie = new QGraphicsView(scene, this);
    vuePartie->setFixedSize(LARGEUR_ECRAN, HAUTEUR_ECRAN);

    // Définir les limites de la scène pour s'assurer qu'elle commence à (0, 0)
    scene->setSceneRect(0, 0, LARGEUR_ECRAN, HAUTEUR_ECRAN);

    // Ajouter la vue directement à stackedWidget
    stackedWidget->addWidget(vuePartie);
    stackedWidget->setCurrentWidget(vuePartie);

    // Connecter tous les VuePion dans le plateau
    QList<VuePion*> listePions = vuePlateau->getListePionsPlateau();
    for (VuePion* pion : listePions) {
        connect(pion, &VuePion::pionClique, this, &FenetrePrincipale::onPionClique);
    }
}

void FenetrePrincipale::afficherPiochesDebut() {
    dessinerPanneauJoueur(0, 0, 170, HAUTEUR_ECRAN, Qt::darkGray, 0.5);  // panneau du joueur 1 (a gauche)
    dessinerPanneauJoueur(LARGEUR_ECRAN - 170, 0, 170, HAUTEUR_ECRAN, Qt::darkGray, 0.5);  // panneau du joueur 2 (a droite)

    QGraphicsTextItem* textePiocheJoueur1 = new QGraphicsTextItem("Pioche " + QString::fromStdString(controleur->partie->getJoueur1()->getNom()));
    // Définir la position du texte
    textePiocheJoueur1->setPos(10, 10);
    // Définir les propriétés du texte (facultatif)
    QFont font("Arial", 13, QFont::Bold);
    textePiocheJoueur1->setFont(font);
    textePiocheJoueur1->setDefaultTextColor(Qt::black);
    scene->addItem(textePiocheJoueur1);

    QGraphicsTextItem* textePiocheJoueur2 = new QGraphicsTextItem("Pioche " + QString::fromStdString(controleur->partie->getJoueur2()->getNom()));
    // Définir la position du texte
    textePiocheJoueur2->setPos(864, 10);
    // Définir les propriétés du texte (facultatif)
    textePiocheJoueur2->setFont(font);
    textePiocheJoueur2->setDefaultTextColor(Qt::black);
    scene->addItem(textePiocheJoueur2);

    texteTour = new QGraphicsTextItem("Au tour de: " + controleur->getAQuiDeJouer());
    texteTour->setPos(LARGEUR_ECRAN / 2 - 100, 10);
    texteTour->setFont(font);
    texteTour->setDefaultTextColor(Qt::black);
    scene->addItem(texteTour);

    // on dessine les pions de départ des joueurs
    dessinerPionsPiochesJoueurs();
}

void FenetrePrincipale::dessinerPanneauJoueur(int x, int y, int largeur, int hauteur, QColor couleur, double opacite) {
    // dessine un panneau aux coordonnées spécifiées avec les propriétés spécifiées
    QGraphicsRectItem* panneau = new QGraphicsRectItem(x, y, largeur, hauteur);

    // Crée un dégradé linéaire pour donner un effet métallique
    QLinearGradient gradient(0, 0, largeur, hauteur);
    gradient.setColorAt(0, couleur.lighter(150));
    gradient.setColorAt(0.5, couleur);
    gradient.setColorAt(1, couleur.darker(150));

    controleur->partie->getJoueur1()->getPionsEnMain(); // renvoie un vector de pions

    QBrush pinceau(gradient);
    panneau->setBrush(pinceau);
    panneau->setOpacity(opacite);
    scene->addItem(panneau);
}

void FenetrePrincipale::dessinerPionsPiochesJoueurs() {

    // dessine les pions du joueur 1
    for (size_t i = 0; i < controleur->getPiocheJoueur1().size(); i++) {
        VuePion* pion = controleur->getPiocheJoueur1()[i];
        pion->setPos(60, 40 + (HAUTEUR_PIONS + ESPACEMENT_VERTICAL_PIONS_PIOCHE) * i);
        scene->addItem(pion);

        connect(pion, &VuePion::pionClique, this, &FenetrePrincipale::onPionClique);
    }
    // dessine les pions du joueur 2
    for (size_t j = 0; j < controleur->getPiocheJoueur2().size(); j++) {
        VuePion* pion = controleur->getPiocheJoueur2()[j];
        pion->setPos(904, 40 + (HAUTEUR_PIONS + ESPACEMENT_VERTICAL_PIONS_PIOCHE) * j);
        scene->addItem(pion);

        connect(pion, &VuePion::pionClique, this, &FenetrePrincipale::onPionClique);
    }
}

void FenetrePrincipale::onPionClique(VuePion* pion) {
    std::cout << "pion clique " << pion << std::endl;
    if (pionEnCoursDeTraitement == nullptr) {
        if (pion->getEstPose() == false) {
            // SI AU TOUR DE JOUEUR1 ET QU'ON CLIQUE SUR UN PION DE SA PIOCHE
            if (controleur->getAQuiDeJouer() == QString::fromStdString(controleur->partie->getJoueur1()->getNom())) {
                if (pion->getCouleur() == Qt::darkRed) {
                    positionDeDepart = pion->pos();
                    pionEnCoursDeTraitement = pion;
                    return;
                }
            }
            // SI AU TOUR DE JOUEUR2 ET QU'ON CLIQUE SUR UN PION DE SA PIOCHE
            else if (controleur->getAQuiDeJouer() == QString::fromStdString(controleur->partie->getJoueur2()->getNom())) {
                if (pion->getCouleur() == Qt::darkBlue) {
                    positionDeDepart = pion->pos();
                    pionEnCoursDeTraitement = pion;
                    return;
                }
            }
        }
    }

    // SI IL Y A UN PION EN COURS DE TRAITEMENT (CAD QU IL Y A UN PION A PLACER / DEPLACER)
    else {
        if (pion->getEstPose() == true && pion->getCouleur() == Qt::white) {

        }
    }
}
