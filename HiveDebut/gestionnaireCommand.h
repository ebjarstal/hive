#pragma once

#include <stack>
#include "command.h"

class GestionnaireCommand {
public:
	void executeCommand(std::unique_ptr<Command> cmd);
	void undoCommand();
	static std::stack<std::unique_ptr<Command>> historique;

private:
	//static std::stack<std::unique_ptr<Command>> historique;
	friend class Partie;
};