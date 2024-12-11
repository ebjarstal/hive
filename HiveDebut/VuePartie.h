#ifndef VUE_PARTIE_H
#define VUE_PARTIE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "VuePlateau.h"
#include <QMouseEvent>

/*
namespace Ui {
    class Partie;
}
*/

class VuePartie : public QGraphicsView {
public:
    // constructeur
    explicit VuePartie(QWidget* parent = nullptr);

    // méthodes publiques
    void setup();
    QString getAQuiDeJouer();
    void setAQuiDeJouer(QString joueur);
    void ramasserPion(VuePion* pion);
    void poserPion(VuePion* pionARemplacer);
    void tourSuivant();
    void retirerDePioche(VuePion* pion, QString joueur);

    // events
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

    // attributs publics
    QGraphicsScene* scene;
    VuePlateau* plateau;
    VuePion* pionEnCoursDeTraitement;
    VuePion* pionTemporaire;
    QPointF positionDeDepart;

private:
    // Ui::Partie* ui;
    // méthodes privées
    void dessinerPanneauJoueur(int x, int y, int largeur, int hauteur, QColor couleur, double opacite);
    void dessinerInterface();
    void creerNouveauPion(QString joueur);
    void creerPionsDebut();
    void creerPionTemporaire(QPointF position);
    void dessinerPions();

    // attributs privés
    QString aQuiDeJouer_;
    QGraphicsTextItem* aQuiDeJouerTexte;
    QList<VuePion*> pionsJoueur1;
    QList<VuePion*> pionsJoueur2;
};

#endif // VUE_PARTIE_H
