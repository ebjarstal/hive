// usine.h
#pragma once

#include <map>
#include <vector>
#include <string>
#include "pions.h" // Inclure le fichier d'en-t�te de Pion

class UsineDePions {
private:
    // Un dictionnaire pour suivre le nombre de pions de chaque type
    std::map<std::string, unsigned int> nombreDePions;

public:
    UsineDePions();

    // M�thode pour cr�er un pion en fonction de son type
    Pion* creerPion(const std::string& type, const std::string& couleur);

    // M�thode pour ajouter un nouveau type de pion (ex. pour des extensions)
    void ajouterExtension(const std::string& type, unsigned int nombre) {
        nombreDePions[type] = nombre;
    }

    // M�thode pour lister les pions disponibles et leur nombre
    std::map<std::string, unsigned int> getNombreDePions() const {
        return nombreDePions;
    }
};
