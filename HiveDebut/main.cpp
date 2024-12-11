#include "VuePartie.h"
#include <QtWidgets/QApplication>
#include "partie.h"
#include "plateau.h"
#include "pions.h"
#include "gestionnaireMouvements.h"
#include "gestionnairePions.h"
#include "gestionnaireVoisins.h"
#include "gestionnaireSauvegarde.h"
#include "joueur.h"
#include "mouvement.h"

VuePartie* partie = nullptr;

using namespace std;

int main(int argc, char *argv[]) {
    //unsigned int length = 20;

    //Plateau plateau(length, length, 5);
    //Partie partie(plateau);
    //partie.setup();
    //while (partie.partieTerminee() != true) {
    //    GestionnaireSauvegarde::sauvegarde(partie);
    //    partie.getPlateau().afficher();
    //    partie.jouerUnTour(partie.getJoueur1());
    //    if (partie.partieTerminee() == true) break;
    //    partie.getPlateau().afficher();
    //    partie.jouerUnTour(partie.getJoueur2());
    //}

    QApplication a(argc, argv);

    //HiveClass w;
    //w.show();

    partie = new VuePartie();
    partie->show();
    partie->setup();

    return a.exec();
    // return 0;
}

