#include "VuePartie.h"
#include "VuePion.h"
#include "VuePlateau.h"
// #include "ui_Partie.h"
#include "Constantes.h"


VuePartie::VuePartie(QWidget* parent) : QGraphicsView(parent) {
    // ui->setupUi(this);

    // setup l'écran
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(LARGEUR_ECRAN, HAUTEUR_ECRAN);

    // setup la scène
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, LARGEUR_ECRAN, HAUTEUR_ECRAN);
    scene->setBackgroundBrush(Qt::white); // met le fond d'écran en blanc

    setScene(scene);

    // initialisation de variables
    pionEnCoursDeTraitement = nullptr;

    // connect(ui->quitterButton, &QPushButton::clicked, this, &Partie::close);
}

void VuePartie::setup() {
    // test à supprimer
    scene->clear();
    plateau = new VuePlateau();
    plateau->poserPions(175, 100, TAILLE_PLATEAU, TAILLE_PLATEAU);
    dessinerInterface();
    creerPionsDebut();
}

void VuePartie::dessinerPanneauJoueur(int x, int y, int largeur, int hauteur, QColor couleur, double opacite) {
    // dessine un panneau aux coordonnées spécifiées avec les propriétés spécifiées
    QGraphicsRectItem* panneau = new QGraphicsRectItem(x, y, largeur, hauteur);
    QBrush pinceau;
    pinceau.setStyle(Qt::SolidPattern);
    pinceau.setColor(couleur);
    panneau->setBrush(pinceau);
    panneau->setOpacity(opacite);
    scene->addItem(panneau);
}

void VuePartie::dessinerInterface() {
    // dessine le panneau du joueur à gauche
    dessinerPanneauJoueur(0, 0, 150, 768, Qt::darkGray, 1);

    // dessine le panneau du joueur à droite
    dessinerPanneauJoueur(874, 0, 150, 768, Qt::darkGray, 1);

    // place le texte du joueur 1
    QGraphicsTextItem* joueur1 = new QGraphicsTextItem(QString("Pioche de: ") + QString(JOUEUR1));
    joueur1->setPos(25, 0);
    scene->addItem(joueur1);

    // place le texte du joueur 2
    QGraphicsTextItem* joueur2 = new QGraphicsTextItem(QString("Pioche de: ") + QString(JOUEUR2));
    joueur2->setPos(874 + 25, 0);
    scene->addItem(joueur2);

    // place le texte "Au tour de: {joueur1 ou joueur2}"
    aQuiDeJouerTexte = new QGraphicsTextItem();
    setAQuiDeJouer(JOUEUR1);
    aQuiDeJouerTexte->setPos(450, 0);
    aQuiDeJouerTexte->setDefaultTextColor(Qt::black);
    scene->addItem(aQuiDeJouerTexte);
}

QString VuePartie::getAQuiDeJouer() {
    return aQuiDeJouer_;
}

void VuePartie::setAQuiDeJouer(QString joueur) {
    // on change l'attribut public
    aQuiDeJouer_ = joueur;

    // on change le QGraphicsTextItem
    aQuiDeJouerTexte->setPlainText(QString("Au tour de: ") + joueur);
}

void VuePartie::creerNouveauPion(QString joueur) {
    // crée le pion qu'il faudra ensuite mettre dans la pioche du joueur
    VuePion* pion = new VuePion();
    pion->setProprietaire(joueur);
    pion->setEstPose(false);

    // ajoute le nouveau pion dans la pioche du joueur
    if (joueur == QString(JOUEUR1)) {
        pionsJoueur1.append(pion);
    }
    else if (joueur == QString(JOUEUR2)) {
        pionsJoueur2.append(pion);
    }

    // met à jour l'affichage de la pioche
    dessinerPions();
}

void VuePartie::creerPionsDebut() {
    // crée les pions du joueur 1
    for (size_t i = 0; i < NB_PIONS_DEBUT; i++) {
        creerNouveauPion(QString(JOUEUR1));
    }

    // crée les pions du joueur 2
    for (size_t j = 0; j < NB_PIONS_DEBUT; j++) {
        creerNouveauPion(QString(JOUEUR2));
    }

    // met à jour l'affichage
    dessinerPions();
}

void VuePartie::creerPionTemporaire(QPointF position) {
    VuePion* pion = new VuePion();
    pion->setProprietaire("temporaire");
    pion->setEstPose(true);
    pion->setPos(position);
    pionTemporaire = pion;
    scene->addItem(pion);
}


void VuePartie::dessinerPions() {
    // itère dans la liste de pions et les dessine

    // retire les pions de la pioche du joueur 1 pour éviter les duplicats
    for (size_t i = 0, n = pionsJoueur1.size(); i < n; i++) {
        scene->removeItem(pionsJoueur1[i]);
    }

    // retire les pions de la pioche du joueur 2 pour éviter les duplicats
    for (size_t j = 0, n = pionsJoueur2.size(); j < n; j++) {
        scene->removeItem(pionsJoueur2[j]);
    }

    // dessine les pions du joueur 1
    for (size_t i = 0, n = pionsJoueur1.size(); i < n; i++) {
        VuePion* pion = pionsJoueur1[i];
        pion->setPos(30, 40 + (HAUTEUR_PIONS + ESPACEMENT_VERTICAL_PIONS_PIOCHE) * i);
        scene->addItem(pion);
    }

    // dessine les pions du joueur 2
    for (size_t j = 0, n = pionsJoueur2.size(); j < n; j++) {
        VuePion* pion = pionsJoueur2[j];
        pion->setPos(874 + 30, 40 + (HAUTEUR_PIONS + ESPACEMENT_VERTICAL_PIONS_PIOCHE) * j);
        scene->addItem(pion);
    }
}

void VuePartie::mouseMoveEvent(QMouseEvent* event) {
    // si il y a un pion à placer, le faire suivre le curseur de la souris
    if (pionEnCoursDeTraitement) {
        pionEnCoursDeTraitement->setPos(event->pos());
    }

    QGraphicsView::mouseMoveEvent(event);
}


void VuePartie::mousePressEvent(QMouseEvent* event) {
    // un clique droit lorsqu'on a ramassé un pion le remet à sa position initiale
    if (event->button() == Qt::RightButton) {
        if (pionEnCoursDeTraitement) {
            if (pionTemporaire != nullptr) {
                scene->removeItem(pionTemporaire);
                pionTemporaire = nullptr;
            }
            pionEnCoursDeTraitement->setPos(positionDeDepart);
            pionEnCoursDeTraitement = NULL;
            return;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void VuePartie::ramasserPion(VuePion* pion) {
    // ramasse le pion en argument
    if (pion->getProprietaire() == getAQuiDeJouer() && pionEnCoursDeTraitement == nullptr) {
        positionDeDepart = pion->pos();
        if (pion->getEstPose() == true) {
            creerPionTemporaire(positionDeDepart); // crée un pion appartenant à "personne" à la position de départ
        }
        pionEnCoursDeTraitement = pion;
        return;
    }
}


void VuePartie::poserPion(VuePion* pionARemplacer) {
    // remplace le pion spécifié par le pion à placer
    pionEnCoursDeTraitement->setPos(pionARemplacer->pos());
    plateau->getPions().removeAll(pionARemplacer);
    plateau->getPions().append(pionEnCoursDeTraitement);
    scene->removeItem(pionARemplacer);
    pionEnCoursDeTraitement->setEstPose(true);
    retirerDePioche(pionEnCoursDeTraitement, getAQuiDeJouer());
    pionEnCoursDeTraitement = nullptr;

    // s'il y a un pion temporaire, en le transforme en pion vide appartenant à "personne"
    if (pionTemporaire != nullptr) {
        pionTemporaire->setProprietaire("personne");
    }

    // faire en sorte que ce soit au tour du prochain joueur de jouer
    tourSuivant();
}

void VuePartie::tourSuivant() {
    if (getAQuiDeJouer() == QString(JOUEUR1)) {
        setAQuiDeJouer(QString(JOUEUR2));
    }
    else if (getAQuiDeJouer() == QString(JOUEUR2)) {
        setAQuiDeJouer(QString(JOUEUR1));
    }
}

void VuePartie::retirerDePioche(VuePion* pion, QString joueur) {
    if (joueur == QString(JOUEUR1)) {
        // retirer de la pioche du joueur1
        pionsJoueur1.removeAll(pion);
    }
    else if (joueur == QString(JOUEUR2)) {
        // retirer de la pioche du joueur2
        pionsJoueur2.removeAll(pion);
    }
}
