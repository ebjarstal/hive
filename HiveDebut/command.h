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
	virtual void undo() = 0;
	virtual std::string getDescription() const = 0;  // Méthode pour la description
};

class PoserPionCommand : public Command {
public:
	PoserPionCommand(Joueur& j, Plateau& plt, Pion* pion, Mouvement* mouv) : joueur(j), plateau(plt), pion(pion), mouv(mouv) {}
	virtual void execute() override;
	virtual void undo() override;
	virtual std::string getDescription() const override;

private:
	Joueur& joueur;
	Plateau& plateau;
	Pion* pion;
	Mouvement* mouv;
};

class DeplacerPionCommand : public Command {
public:
	DeplacerPionCommand(Joueur& j, Plateau& plt, Pion* pion, Mouvement* mouv) : joueur(j), plateau(plt), pion(pion), mouv(mouv) {}
	virtual void execute() override;
	virtual void undo() override;
	virtual std::string getDescription() const override;
private:
	Joueur& joueur;
	Plateau& plateau;
	Pion* pion;
	Mouvement* mouv;
};