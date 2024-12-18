#include "usine.h"
#include "pions.h"

UsineDePions::UsineDePions() {
    // Initialisation des nombres de pions pour chaque type de pion standard
    nombreDePions["R"] = { 1,true };  // Reine
    nombreDePions["S"] = { 3, true };  // Sauterelle
    nombreDePions["F"] = { 3, true };  // Fourmi
    nombreDePions["K"] = { 2, true };  // Scarabee
    nombreDePions["A"] = { 2, true };  // Araignee
  
    //mettre les extension à false de base
    nombreDePions["X"] = { 1, false };  // Coccinelle
    nombreDePions["C"] = { 1, false };  // Cloporte
    nombreDePions["M"] = { 1, false };  // Moustique

    //ajoute des types si besoin de plus
}

Pion* UsineDePions::creerPion(const std::string& type, const std::string& couleur) {
    auto it = nombreDePions.find(type);
    if (it != nombreDePions.end() && it->second.second && it->second.first > 0) {

        if (type == "R") return new Reine(couleur);
        if (type == "S") return new Sauterelle(couleur);
        if (type == "F") return new Fourmi(couleur);
        if (type == "K") return new Scarabee(couleur);
        if (type == "A") return new Araignee(couleur);
        if (type == "X") return new Coccinelle(couleur);
        if (type == "C") return new Cloporte(couleur);
        if (type == "M") return new Moustique(couleur);
    }
    return nullptr;  // Retourne nullptr si le type est invalide ou s'il n'y a plus de pions disponibles
 }
    


Pion* UsineDePions::creerPion(int id, const std::string& type, const std::string& couleur) {
    auto it = nombreDePions.find(type);
    if (it != nombreDePions.end() && it->second.second && it->second.first > 0) {

        if (type == "R") return new Reine(id, couleur);
        if (type == "S") return new Sauterelle(id, couleur);
        if (type == "F") return new Fourmi(id, couleur);
        if (type == "K") return new Scarabee(id, couleur);
        if (type == "A") return new Araignee(id, couleur);
        if (type == "X") return new Coccinelle(id, couleur);
        if (type == "C") return new Cloporte(id, couleur);
        if (type == "M") return new Moustique(id, couleur);
    }
    return nullptr;  // Retourne nullptr si le type est invalide ou s'il n'y a plus de pions disponibles
}
