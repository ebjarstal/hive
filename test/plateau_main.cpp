#include "../include/plateau.h"

void test1() {
    int n = 6;
    Plateau plateau(n, n); // Crée une grille n * n

    plateau.setPion(1, 0, new Pion("R"));
    plateau.setPion(2, 2, new Pion("A"));
    plateau.setPion(4, 5, new Pion("S"));
    plateau.setPion(3, 5, new Pion("F"));

    plateau.afficher();

    // Test pour récupérer la valeur des pions de la grille
    try {
        std::cout << "Valeur du pion en (1, 0): " << plateau.getPion(1, 0)->getNom() << std::endl;
        std::cout << "Valeur du pion en (2, 2): " << plateau.getPion(2, 2)->getNom() << std::endl;
        std::cout << "Valeur du pion en (4, 5): " << plateau.getPion(4, 5)->getNom() << std::endl;
        std::cout << "Valeur du pion en (1, 1): " << (plateau.getPion(1, 1) ? plateau.getPion(1, 1)->getNom() : 0) << std::endl; // Devrait être 0
        std::cout << "Valeur du pion en (3, 5): " << plateau.getPion(3, 5)->getNom() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
    }

    // Test hors de portée
    try {
        plateau.getPion(6, 6); // Doit renvoyer une exception
    } catch (const std::out_of_range& e) {
        std::cerr << "Exception attrapee: " << e.what() << std::endl;
    }

    // Supprime les pions alloués dynamiquement
    for (int ligne = 0; ligne < n; ++ligne) {
        for (int colonne = 0; colonne < n; ++colonne) {
            delete plateau.getPion(ligne, colonne);
        }
    }
}

void test2() {
    int n = 6;
    Plateau plateau(n, n); // Crée une grille n * n

    plateau.setPion(1, 0, new Pion("R"));
    plateau.setPion(2, 2, new Pion("A"));
    plateau.setPion(4, 5, new Pion("S"));
    plateau.setPion(3, 5, new Pion("F"));

    plateau.afficher();

    // Test pour récupérer les voisins des pions de la grille
    try {
        int x = 1, y = 1;
        auto voisins = plateau.getVoisins(x, y);
        std::cout << "Voisins du pion en (" << x << ", " << y << "): ";
        for (auto voisin : voisins) {
            if (voisin) {
                std::cout << voisin->getNom() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
    }
}

void test3() {
    int n = 9;
    Plateau plateau(n, n);
    
    plateau.setPion(3, 4, new Pion("O"));
    plateau.setPion(4, 3, new Pion("O"));
    plateau.setPion(4, 5, new Pion("O"));
    plateau.setPion(5, 3, new Pion("O"));
    plateau.setPion(5, 4, new Pion("O"));
    plateau.setPion(5, 5, new Pion("O"));
    plateau.setPion(6, 3, new Pion("O"));
    plateau.setPion(6, 5, new Pion("O"));

    plateau.afficher();

    try {
        int x = 6, y = 3;
        auto voisins = plateau.getVoisins(x, y);
        std::cout << "Voisins du pion en (" << x << ", " << y << "): ";
        for (auto voisin : voisins) {
            if (voisin) {
                std::cout << voisin->getNom() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << e.what() << std::endl;
    }
}

void start() {
    int n = 6;
    Plateau plateau(n, n);
    char choix;
    int x, y, newX, newY;
    std::string nom;

    while (true) {
        plateau.afficher();
        std::cout << "Voulez-vous (p)lacer un nouveau pion ou (d)eplacer un pion existant? (q pour quitter): ";
        std::cin >> choix;

        if (choix == 'q') break;

        if (choix == 'p') {
            std::cout << "Entrez les coordonnees (x y) pour placer le pion: ";
            std::cin >> x >> y;
            std::cout << "Entrez le nom du pion (char): ";
            std::cin >> nom;

            if (nom.length() != 1) {
                std::cout << "Le nom du pion doit être un seul caractère." << std::endl;
                continue;
            }

            if (plateau.getPion(x, y) == nullptr) {
                if (plateau.hasVoisin(x, y) || plateau.isVide()) {
                    plateau.setPion(x, y, new Pion(nom));
                } else {
                    std::cout << "Le nouvel emplacement doit avoir au moins un voisin existant" << std::endl;
                }
            } else {
                std::cout << "Il y a deja un pion a cet emplacement" << std::endl;
            }
        } else if (choix == 'd') {
            std::cout << "Entrez les coordonnees (x y) du pion a deplacer: ";
            std::cin >> x >> y;
            std::cout << "Entrez les nouvelles coordonnees (newX newY): ";
            std::cin >> newX >> newY;

            if (plateau.getPion(x, y) != nullptr && plateau.getPion(newX, newY) == nullptr) {
                if (plateau.hasVoisin(x, y)) {
                    Pion* pion = plateau.getPion(x, y);
                    plateau.setPion(newX, newY, pion);
                    plateau.deletePion(x, y);  // pourquoi ça coupe là???
                } else {
                    std::cout << "Le nouvel emplacement doit avoir au moins un voisin existant" << std::endl;
                }
            } else {
                std::cout << "Deplacement invalide" << std::endl;
            }
        } else {
            std::cout << "Choix invalide" << std::endl;
        }
    }
}

int main() {
    // test1();
    // test2();
    // test3();
    start();
    return 0;
}
