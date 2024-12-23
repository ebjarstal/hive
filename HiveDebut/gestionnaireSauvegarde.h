#pragma once

#include <iostream>
#include <stack>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <map>
#include <algorithm>
#include <vector>

class Partie;

class GestionnaireSauvegarde {
public:
    static void creerDossierSiInexistant(const std::string& cheminDossier);
    static bool chargementSauvegardePartie(Partie& p, const std::string dossierSauvegarde);  // Charge une partie depuis une liste de fichier de sauvegarde
    static bool chargerPartie(Partie& p);  // Charge une partie à partir du fichier texte de sauvegarde
    static void sauvegarde(Partie& p);
    static std::vector<std::string> listerSauvegardes();
};
