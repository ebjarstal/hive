#include "partie.h"
#include "plateau.h"
#include "pions.h"
#include "gestionnaireMouvements.h"
#include "gestionnairePions.h"
#include "gestionnaireVoisins.h"
#include "gestionnaireSauvegarde.h"
#include "joueur.h"
#include "mouvement.h"
#include "Constantes.h"

#include <QApplication>
#include "FenetrePrincipale.h"

int main(int argc, char* argv[]) {
    int choix = 0;
    std::cout << "Interface console (0) ou interface Qt (1) ?" << std::endl;
    std::cin >> choix;
    if (choix == 0) {

        Plateau plateau(TAILLE_PLATEAU, TAILLE_PLATEAU, NB_COUCHES);
        UsineDePions* usine = new UsineDePions;
        Partie partie(plateau, usine);
        partie.setup();
        while (partie.partieTerminee() != true) {
            GestionnaireSauvegarde::sauvegarde(partie);
            partie.getPlateau().afficher();
            partie.jouerUnTour(partie.getJoueur1());
            if (partie.partieTerminee() == true) break;
            partie.getPlateau().afficher();
            partie.jouerUnTour(partie.getJoueur2());
        }
    }
    else if (choix == 1) {
        QApplication app(argc, argv);
        FenetrePrincipale fenetre;
        fenetre.show();
        return app.exec();
    }
    else {
        std::cout << "Choix invalide" << std::endl;
    }
    return 0;
}
