#pragma once

#include <iostream>

class Joueur;
class Plateau;
class Pion;
class Mouvement;

class Command {
public:
	virtual ~Command() {}
	virtual void execute() = 0;
};

class PoserPionCommand : public Command {
public:
	PoserPionCommand(Joueur& j, Plateau& plt, Pion* pion, Mouvement* mouv) : joueur(j), plateau(plt), pion(pion), mouv(mouv) {}
	virtual void execute();
private:
	Joueur& joueur;
	Plateau& plateau;
	Pion* pion;
	Mouvement* mouv;
};

class DeplacerPionCommand : public Command {
public:
	DeplacerPionCommand(Joueur& j, Plateau& plt, Pion* pion, Mouvement* mouv) : joueur(j), plateau(plt), pion(pion), mouv(mouv) {}
	virtual void execute();
private:
	Joueur& joueur;
	Plateau& plateau;
	Pion* pion;
	Mouvement* mouv;
};