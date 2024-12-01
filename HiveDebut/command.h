#pragma once

#include <iostream>

class Plateau;
class Partie;
class Mouvement;
class Pion;
class Joueur;

class Command {
public:
	virtual ~Command() {}
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual std::string getDescription() const = 0;  // Méthode pour la description
};

class MouvementCommand : public Command {
public:
	MouvementCommand(Partie& p, Mouvement* mouv);
	virtual void execute() override;
	virtual void undo() override;
	virtual std::string getDescription() const override;
	Mouvement* getMouvement() const { return mouv; }

private:
	Partie& partie;
	Plateau& plateau;
	Mouvement* mouv;
	Pion* pion;
	Joueur* joueur;
};