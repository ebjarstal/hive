// usine.h
#pragma once

#include <map>
#include <vector>
#include <string>
#include "pions.h" // Inclure le fichier d'en-tête de Pion

class UsineDePions {
private:
    // Un dictionnaire pour suivre le nombre de pions de chaque type
    std::map<std::string, unsigned int> nombreDePions;

public:
    UsineDePions();

    // Méthode pour créer un pion en fonction de son type
    Pion* creerPion(const std::string& type, const std::string& couleur);

    // Méthode pour ajouter un nouveau type de pion (ex. pour des extensions)
    void ajouterExtension(const std::string& type, unsigned int nombre) {
        nombreDePions[type] = nombre;
    }

    // Méthode pour lister les pions disponibles et leur nombre
    std::map<std::string, unsigned int> getNombreDePions() const {
        return nombreDePions;
    }
};
