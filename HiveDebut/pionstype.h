#pragma once

#include "pions.h"

class Reine : public Pion {
public:
    Reine(const string& couleur) : Pion("Reine", couleur) {}
};

class Sauterelle : public Pion {
public:
    Sauterelle(const string& couleur) : Pion("Sauterelle", couleur) {}
};

class Fourmi : public Pion {
public:
    Fourmi(const string& couleur) : Pion("Fourmi", couleur) {}
};


class Araignee : public Pion {
public:
    Araignee(const string& couleur) : Pion("Araignee", couleur) {}
};

class Scarabee : public Pion {
public:
    Scarabee(const string& couleur) : Pion("Scarabee", couleur) {}
};



class Coccinelle : public Pion {
public:
    Coccinelle(const string& couleur) : Pion("Coccinelle", couleur) {}
};

class Cloporte : public Pion {
public:
    Cloporte(const string& couleur) : Pion("Cloporte", couleur) {}
};

class Moustique : public Pion {
public:
    Moustique(const string& couleur) : Pion("Moustique", couleur) {}
};