#pragma once

#include <stack>
#include "command.h"

class GestionnaireCommand {
public:
	void executeCommand(Command* cmd);
	void undoCommand();

private:
	static std::stack<Command*> historique;
	friend class Partie;
};