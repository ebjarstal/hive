#pragma once

#include <iostream>
#include <tuple>
#include <functional>
#include <vector>

// Permet d'utiliser les couleurs dans la console
#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

class Mouvement {
private:
    int pionId;
    std::tuple<int, int, int> coordDest;
    std::tuple<int, int, int> coordSrc;
    std::function<void()> callback;
    std::function<void()> undoCallback;
public:
    Mouvement(int id, int ligne, int colonne, int z, int oldLigne, int oldColonne, int oldZ)
        : pionId(id), coordDest(std::make_tuple(ligne, colonne, z)), coordSrc(std::make_tuple(oldLigne, oldColonne, oldZ)) {}
    int getPionId() const { return pionId; }
    int getLigne() const { return std::get<0>(coordDest); }
    int getColonne() const { return std::get<1>(coordDest); }
    int getZ() const { return std::get<2>(coordDest); }
    int getOldLigne() const { return std::get<0>(coordSrc); }
    int getOldColonne() const { return std::get<1>(coordSrc); }
    int getOldZ() const { return std::get<2>(coordSrc); }

    void setCoordSrc(std::tuple<int, int, int> cSrc) { coordSrc = cSrc; }
    std::tuple<int, int, int> getCoordDest() { return coordDest; }
    std::tuple<int, int, int> getCoordSrc() { return coordSrc; }

    std::function<void()> getCallback() const { return callback; }
    std::function<void()> getUndoCallback() const { return undoCallback; }
    void setCallback(std::function<void()> cb) { callback = cb; }
    void executeCallback(std::function<void()> cb) { if (cb) cb(); }
    bool hasCallback() const { return callback != nullptr && undoCallback != nullptr; }

    void ajouterMouvementManuel(std::vector<Mouvement*>& mouvementsPossibles, Mouvement* nouveauMouvement);
};