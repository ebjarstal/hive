#IFNDEF PIONS_H
#DEFINE PIONS_H

#include "classes.h"
#include "tableau.h"
#include <iostream>
#include <string>
using namespace std;

class Reine{
    public:
    void deplacementsPossibles(); //retourner list de coordonnées dispo?
};
 
//ajouter attributs pour savoir combien de pion restant à poser pour classes avec plusieurs pions?
class Scarabees{
    public:
    void deplacementsPossibles();
};

class Araignees{
    public:
    void deplacementsPossibles();
};

class Coccinelle{
    public:
    void deplacementsPossibles();
};

class Moustique{
    public:
    void deplacementsPossibles();
};

class Cloporte{
    public:
    void deplacementsPossibles();
};

class Fourmis{
    public:
    void deplacementsPossibles();
};

class Sauterelles{
    public:
    void deplacementsPossibles();
};

#ENDIF
