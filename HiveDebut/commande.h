#pragma once

#include "partie.h"

class Commande {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Commande() = default;
};

class MouvementCommande : public Commande {
private:
    Mouvement* mouvement;
    Partie& partie;

    Pion* pion;
    Joueur* joueur;

public:
    Mouvement* getMouvement() { return mouvement; }

    MouvementCommande(Mouvement* m, Partie& p);

    void execute() override;

    void undo() override;
};
