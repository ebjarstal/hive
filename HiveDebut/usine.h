// usine.h
#pragma once

#include <map>
#include <vector>
#include <string>
#include "pions.h" // Inclure le fichier d'en-tête de Pion

class UsineDePions {
private:
    // Un dictionnaire pour suivre le nombre de pions de chaque type et l'état de l'extension
    std::map<std::string, std::pair<unsigned int, bool>> nombreDePions;

public:
    UsineDePions();

    // Méthode pour créer un pion en fonction de son type
    Pion* creerPion(const std::string& type, const std::string& couleur);
    Pion* creerPion(int id, const std::string& type, const std::string& couleur);

    //Méthode qui active une extension
    void setExtensionActive(const std::string& type) {
        if (nombreDePions.find(type) != nombreDePions.end()) {
            nombreDePions[type].second = true;
        }
    }

    //Méthode qui active ou désactive une extension 
    /*void setOrUnsetExtensionActive(const std::string& type) {
        if (nombreDePions.find(type) != nombreDePions.end()) {
            if (type != "R" && type != "K" && type != "F" && type != "S" && type != "A") {
                bool etat = nombreDePions[type].second;
                nombreDePions[type].second = !etat;
            }
        }
    }*/

    // Méthode pour lister les pions disponibles et leur nombre
    std::map<std::string, std::pair<unsigned int, bool>> getNombreDePions() const {
        return nombreDePions;
    }
};
