#pragma once

#include "pions.h"

class Reine : public Pion {
public:
    Reine(const string& couleur) : Pion("R", couleur) {}
};

class Sauterelle : public Pion {
public:
    Sauterelle(const string& couleur) : Pion("S", couleur) {}
};

class Fourmi : public Pion {
public:
    Fourmi(const string& couleur) : Pion("F", couleur) {}
};


class Araignee : public Pion {
public:
    Araignee(const string& couleur) : Pion("A", couleur) {}
};

class Scarabee : public Pion {
public:
    Scarabee(const string& couleur) : Pion("C", couleur) {}
};



class Coccinelle : public Pion {
public:
    Coccinelle(const string& couleur) : Pion("X", couleur) {}
};

class Cloporte : public Pion {
public:
    Cloporte(const string& couleur) : Pion("P", couleur) {}
};

class Moustique : public Pion {
public:
    Moustique(const string& couleur) : Pion("M", couleur) {}
};