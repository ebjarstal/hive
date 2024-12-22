#include "FenetrePrincipale.h"

FenetrePrincipale::FenetrePrincipale(QWidget* parent) : QMainWindow(parent) {
    // Initialiser le contrôleur de jeu
    Plateau* plateau = new Plateau(TAILLE_PLATEAU, TAILLE_PLATEAU, NB_COUCHES);
    UsineDePions* usine = new UsineDePions;
    Partie* partie = new Partie(*plateau, usine);
    controleur = new Controleur(partie, this);

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

    // Connecter les signaux et les slots
    connect(controleur, &Controleur::partieTerminee, this, &FenetrePrincipale::onPartieTerminee);
    connect(controleur, &Controleur::afficherPlateauDebut, this, &FenetrePrincipale::afficherPlateauDebut);
    connect(controleur, &Controleur::afficherPiochesEtAQuiDeJouer, this, &FenetrePrincipale::afficherPiochesEtAQuiDeJouer);
    connect(controleur, &Controleur::mouvementAnnule, this, &FenetrePrincipale::updateAffichage);
    connect(boutonCommencerPartieContreIA, &QPushButton::clicked, this, &FenetrePrincipale::commencerPartieContreIA);
    connect(boutonCommencerPartieDeuxJoueurs, &QPushButton::clicked, this, &FenetrePrincipale::commencerPartieDeuxJoueurs);
}

FenetrePrincipale::~FenetrePrincipale() {
    // Supprimer les checkboxes dynamiques
    for (QCheckBox* checkbox : checkboxesExtensions) {
        delete checkbox;
    }
    checkboxesExtensions.clear();

    // Supprimer les widgets dynamiquement alloués
    delete champNomJoueur1IA;
    delete champNomSauvegardeIA;
    delete champNombreRetoursIA;
    delete champNomJoueur1DeuxJoueurs;
    delete champNomJoueur2DeuxJoueurs;
    delete champNomSauvegardeDeuxJoueurs;
    delete champNombreRetoursDeuxJoueurs;
    delete labelNomSauvegarde;
    delete labelFichierCharge;
    delete boutonNouvellePartie;
    delete boutonChargerPartie;
    delete boutonQuitter;
    delete boutonRetourNouvellePartie;
    delete boutonRetourChargerPartie;
    delete boutonCommencerPartieContreIA;
    delete boutonCommencerPartieDeuxJoueurs;
    delete boutonOuvrirFichier;
    delete boutonChargerPartieSauvegarde;
    delete boutonAnnulerMouvementJoueur1;
    delete boutonAnnulerMouvementJoueur2;
    delete texteTour;
    delete vuePartie;
    delete scene;
    delete stackedWidget;
    delete widgetCentral;

    // Supprimer le contrôleur
    delete controleur;
}

void FenetrePrincipale::onPartieTerminee(const QString& message) {
    // Afficher un message de fin de partie
    QMessageBox::information(this, "Partie terminée", message);
    this->close();
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

    // Récupérer les extensions disponibles
    auto extensions = controleur->partie->getUsine()->getNombreDePions();
    for (const auto& pair : extensions) {
        const std::string& type = pair.first;
        if (type != "R" && type != "K" && type != "F" && type != "S" && type != "A") {
            QCheckBox* checkbox = new QCheckBox(QString::fromStdString(type), this);
            layout->addWidget(checkbox, 0, Qt::AlignCenter);
            checkboxesExtensions.push_back(checkbox);
        }
    }

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

    // Récupérer les extensions disponibles
    auto extensions = controleur->partie->getUsine()->getNombreDePions();
    for (const auto& pair : extensions) {
        const std::string& type = pair.first;
        if (type != "R" && type != "K" && type != "F" && type != "S" && type != "A") {
            QCheckBox* checkbox = new QCheckBox(QString::fromStdString(type), this);
            layout->addWidget(checkbox, 0, Qt::AlignCenter);
            checkboxesExtensions.push_back(checkbox);
        }
    }

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

void FenetrePrincipale::activerExtensions() {
    for (QCheckBox* checkbox : checkboxesExtensions) {
        if (checkbox->isChecked()) {
            controleur->partie->getUsine()->setExtensionActive(checkbox->text().toStdString());
        }
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

    Joueur* joueur1 = new JoueurHumain(nomJoueur.toStdString(), controleur->partie->initialiserPions(RED), RED, nbUndo, *(controleur->partie));
    Joueur* joueur2 = new JoueurIA("IA", controleur->partie->initialiserPions(WHITE), WHITE, *(controleur->partie), nbUndo);
    controleur->partie->setJoueur1(joueur1);
    controleur->partie->setJoueur2(joueur2);

    controleur->partie->setNomPartie(nomSauvegarde.toStdString());
    controleur->partie->getJoueur1()->setNbUndo(nbUndo);
    controleur->partie->getJoueur2()->setNbUndo(nbUndo);

    // Activer les extensions en fonction des checkbox
    activerExtensions();

    controleur->initialiserPioches();
    controleur->commencerPartie();
    updateAffichage();
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

    Joueur* joueur1 = new JoueurHumain(nomJoueur1.toStdString(), controleur->partie->initialiserPions(RED), RED, nbUndo, *(controleur->partie));
    Joueur* joueur2 = new JoueurHumain(nomJoueur2.toStdString(), controleur->partie->initialiserPions(WHITE), WHITE, nbUndo, *(controleur->partie));
    controleur->partie->setJoueur1(joueur1);
    controleur->partie->setJoueur2(joueur2);

    controleur->partie->setNomPartie(nomSauvegarde.toStdString());
    controleur->partie->getJoueur1()->setNbUndo(nbUndo);
    controleur->partie->getJoueur2()->setNbUndo(nbUndo);

    // Activer les extensions en fonction des checkbox
    activerExtensions();

    controleur->initialiserPioches();
    controleur->commencerPartie();
    updateAffichage();
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
        controleur->commencerPartie();
        stackedWidget->setCurrentIndex(INDEX_PARTIE_EN_COURS); // Rediriger vers la page "Partie en cours"
    }
    else {
        QMessageBox::warning(this, "Erreur", "Erreur lors du chargement de la partie.");
    }
    controleur->updatePioches();
    updateAffichage();
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
    controleur->vuePlateau = new VuePlateau(scene, &(controleur->partie->getPlateau()));
    controleur->vuePlateau->initialiserPlateau(190, 100);

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
    QList<VuePion*> listePions = controleur->vuePlateau->getListePionsPlateau();
    for (VuePion* pion : listePions) {
        connect(pion, &VuePion::pionClique, this, &FenetrePrincipale::onPionClique);
    }
}

void FenetrePrincipale::afficherPiochesEtAQuiDeJouer() {
    dessinerPanneauJoueur(0, 0, 170, HAUTEUR_ECRAN, Qt::darkGray, 0.5);  // panneau du joueur 1 (à gauche)
    dessinerPanneauJoueur(LARGEUR_ECRAN - 170, 0, 170, HAUTEUR_ECRAN, Qt::darkGray, 0.5);  // panneau du joueur 2 (à droite)

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

    // Ajouter les boutons d'annulation de mouvement
    boutonAnnulerMouvementJoueur1 = new QPushButton("Retour arrière", this);
    boutonAnnulerMouvementJoueur1->setGeometry(220, 30, 150, 30);
    boutonAnnulerMouvementJoueur1->show();

    boutonAnnulerMouvementJoueur2 = new QPushButton("Retour arrière", this);
    boutonAnnulerMouvementJoueur2->setGeometry(650, 30, 150, 30);
    boutonAnnulerMouvementJoueur2->show();

    // Connecter les boutons aux slots appropriés
    connect(boutonAnnulerMouvementJoueur1, &QPushButton::clicked, controleur, &Controleur::annulerMouvementJoueur1);
    connect(boutonAnnulerMouvementJoueur2, &QPushButton::clicked, controleur, &Controleur::annulerMouvementJoueur2);

    // Dessiner les pions de départ des joueurs
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
    // si le pion clique est un pion temporairement gris
    // CAD si il y a un mouvement a faire lorsque on clique dessus
    if (std::find(pionsTemporairementGris.begin(), pionsTemporairementGris.end(), pion) != pionsTemporairementGris.end()
        && pionEnCoursDeTraitement != nullptr) {
        // fait le mouvement qu'il y a à faire du cote de la grille de pion
        // et de la grille de vuepion à l'aide des classes du jeu console

        // force le joueur à placer sa reine si nécessaire
        if (controleur->doitPlacerReine(*controleur->partie->getJoueur1(), pionEnCoursDeTraitement)) {
            resetPionsTemporairementModifies();
            pionEnCoursDeTraitement = nullptr;
            updateAffichage();
        }
        else if (controleur->doitPlacerReine(*controleur->partie->getJoueur2(), pionEnCoursDeTraitement)) {
            resetPionsTemporairementModifies();
            pionEnCoursDeTraitement = nullptr;
            updateAffichage();
        }
        // sinon le coup est joué
        else {
            controleur->faireMouvement(pionEnCoursDeTraitement, pion);
            controleur->jouerTour();
            resetPionsTemporairementModifies();
            pionEnCoursDeTraitement = nullptr;
            // on met l'affichage a jour
            updateAffichage();
        }
        return;
    }

    resetPionsTemporairementModifies();

    // si il n'y a pas encore de pion selectionne
    if (pionEnCoursDeTraitement == nullptr) {
        // si click sur pion vide
        if (pion->getType() == QString("") && pion->getEstPose() == true) {
            return;
        }

        // si click sur pion joueur 1
        else if (pion->getCouleur() == Qt::darkRed && controleur->getAQuiDeJouer() == QString::fromStdString(controleur->partie->getJoueur1()->getNom())) {
            // si click sur pion pioche
            if (pion->getEstPose() == false) {
                pionEnCoursDeTraitement = pion;
                afficherEmplacementsPossibles(pion);
                return;
            }
            // si click sur pion pose sur plateau
            else {
                if (controleur->reinePlacee(*controleur->partie->getJoueur1())) {
                    pionEnCoursDeTraitement = pion;
                    afficherDeplacementsPossibles(pion);
                }
                return;
            }
        }

        // si click sur pion joueur 2
        else if (pion->getCouleur() == Qt::darkBlue && controleur->getAQuiDeJouer() == QString::fromStdString(controleur->partie->getJoueur2()->getNom())) {
            // si click sur pion pioche
            if (pion->getEstPose() == false) {
                pionEnCoursDeTraitement = pion;
                afficherEmplacementsPossibles(pion);
                return;
            }
            // si click sur pion pose sur plateau
            else {
                if (controleur->reinePlacee(*controleur->partie->getJoueur2())) {
                    pionEnCoursDeTraitement = pion;
                    afficherDeplacementsPossibles(pion);
                }
            }
        }
    }

    // si il y a deja un pion selectionne
    else {
        // si click sur pion vide
        if (pion->getType() == QString("") && pion->getEstPose() == true) {
            pionEnCoursDeTraitement = nullptr;
            return;
        }

        // si click sur pion joueur 1
        else if (pion->getCouleur() == Qt::darkRed && controleur->getAQuiDeJouer() == QString::fromStdString(controleur->partie->getJoueur1()->getNom())) {
            // si click sur pion pioche
            if (pion->getEstPose() == false) {
                afficherEmplacementsPossibles(pion);
                pionEnCoursDeTraitement = pion;
                return;
            }
            // si click sur pion pose sur plateau
            else {
                pionEnCoursDeTraitement = pion;
                afficherDeplacementsPossibles(pion);
                return;
            }
        }

        // si click sur pion joueur 2
        else if (pion->getCouleur() == Qt::darkBlue && controleur->getAQuiDeJouer() == QString::fromStdString(controleur->partie->getJoueur2()->getNom())) {
            // si click sur pion pioche
            if (pion->getEstPose() == false) {
                afficherEmplacementsPossibles(pion);
                pionEnCoursDeTraitement = pion;
                return;
            }
            // si click sur pion pose sur plateau
            else {
                pionEnCoursDeTraitement = pion;
                afficherDeplacementsPossibles(pion);
                return;
            }
        }
    }
}

void FenetrePrincipale::afficherEmplacementsPossibles(VuePion* pion) {
    resetPionsTemporairementModifies();

    // Récupérer les emplacements possibles
    Pion* pionAssocie = pion->getPionAssocie();
    std::vector<Mouvement*> emplacements = pionAssocie->emplacementsPossibles(*pionAssocie, controleur->partie->getPlateau());

    // Parcourir les emplacements possibles et changer la couleur des VuePion correspondants en gris
    for (Mouvement* mouvement : emplacements) {
        VuePion* vuePion = controleur->vuePlateau->getVuePion(mouvement->getLigne(), mouvement->getColonne(), mouvement->getZ());
        if (vuePion != nullptr) {
            vuePion->setCouleur(Qt::black);
            vuePion->setOpacity(1);
            pionsTemporairementGris.append(vuePion);
        }
    }

    // change l'apparence du pion selectionne pour le remarquer plus facilement
    pion->setCouleur(Qt::darkGray);
}

void FenetrePrincipale::afficherDeplacementsPossibles(VuePion* pion) {
    resetPionsTemporairementModifies();

    // Récupérer les déplacements possibles
    Pion* pionAssocie = pion->getPionAssocie();
    Joueur* joueurAssocie = (pionAssocie->getCouleur() == RED) ? controleur->partie->getJoueur1() : controleur->partie->getJoueur2();
    std::vector<Mouvement*> deplacements = pionAssocie->deplacementsPossibles(*pionAssocie, *joueurAssocie, controleur->partie->getPlateau());

    // Parcourir les déplacements possibles et changer la couleur des VuePion correspondants en gris
    for (Mouvement* mouvement : deplacements) {
        //std::cout << "De (" << mouvement->getOldLigne() << ", " << mouvement->getOldColonne() << ", " << mouvement->getOldZ() << ") à ("
        //    << mouvement->getLigne() << ", " << mouvement->getColonne() << ", " << mouvement->getZ() << ")" << std::endl;

        VuePion* vuePion = controleur->vuePlateau->getVuePion(mouvement->getLigne(), mouvement->getColonne(), mouvement->getZ());
        if (vuePion != nullptr) {
            vuePion->setCouleur(Qt::black);
            vuePion->setOpacity(0.9);
            pionsTemporairementGris.append(vuePion);

            // Vérifier si le vuePion est déjà dans la scène
            if (!scene->items().contains(vuePion)) {
                scene->addItem(vuePion);
            }
        }
    }

    // change l'apparence du pion selectionne pour le remarquer plus facilement
    pion->setCouleur(Qt::darkGray);
}

void FenetrePrincipale::resetPionsTemporairementModifies() {
    if (pionEnCoursDeTraitement != nullptr) {
        (pionEnCoursDeTraitement->getPionAssocie()->getCouleur() == RED) ? pionEnCoursDeTraitement->setCouleur(Qt::darkRed) : pionEnCoursDeTraitement->setCouleur(Qt::darkBlue);
    }

    for (VuePion* pionCourant : pionsTemporairementGris) {
        pionCourant->setCouleur(Qt::white);
        pionCourant->setOpacity(0.2);

        if (pionCourant->getType().isEmpty() && pionCourant->getZ() > 0) {
            int ligne = pionCourant->getLigne();
            int colonne = pionCourant->getColonne();
            int z = pionCourant->getZ();

            bool existePionDessousNonVide = false;
            VuePion* vuePionBelow = controleur->vuePlateau->getVuePion(ligne, colonne, z-1);
            if (vuePionBelow != nullptr && !vuePionBelow->getType().isEmpty()) {
                existePionDessousNonVide = true;
            }

            if (existePionDessousNonVide) {
                scene->removeItem(pionCourant);
            }
        }
    }
    pionsTemporairementGris.clear();
}

void FenetrePrincipale::updateAffichage() {
    scene->clear();

    // afficher le tour
    
    // afficher le plateau
    controleur->updateVuePlateau();
    QList<VuePion*> listePions = controleur->vuePlateau->getListePionsPlateau();
    for (VuePion* pion : listePions) {
        connect(pion, &VuePion::pionClique, this, &FenetrePrincipale::onPionClique);
    }

    controleur->updatePioches();
    afficherPiochesEtAQuiDeJouer();
}
