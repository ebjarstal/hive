#include "usine.h"
#include "pions.h"
#include "pionstype.h"

UsineDePions::UsineDePions() {
    // Initialisation des nombres de pions pour chaque type de pion standard
    nombreDePions["Reine"] = 1;
    nombreDePions["Sauterelle"] = 3;
    nombreDePions["Fourmi"] = 3;
    nombreDePions["Scarabee"] = 2;
    nombreDePions["Araignee"] = 3;

    nombreDePions["Coccinelle"] = 1;
    nombreDePions["Cloporte"] = 1;
    nombreDePions["Moustique"] = 1;

    //ajoute des types si besoin de plus
}

Pion* UsineDePions::creerPion(const std::string& type, const std::string& couleur) {
    if (getNombreDePions()[type] > 0) {
        // Décrémente le nombre de pions restants pour ce type
        nombreDePions[type]--;

        if (type == "Reine") return new Reine(couleur);
        if (type == "Sauterelle") return new Sauterelle(couleur);
        if (type == "Fourmi") return new Fourmi(couleur);
        if (type == "Scarabee") return new Scarabee(couleur);
        if (type == "Araignee") return new Araignee(couleur);
        if (type == "Coccinelle") return new Coccinelle(couleur);
        if (type == "Cloporte") return new Cloporte(couleur);
        if (type == "Moustique") return new Moustique(couleur);
    }
    return nullptr;  // Retourne nullptr si le type est invalide ou s'il n'y a plus de pions disponibles
}
