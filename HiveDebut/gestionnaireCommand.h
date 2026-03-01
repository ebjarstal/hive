#pragma once

#include <memory>
#include <stack>
#include "command.h"

class Partie;

class GestionnaireCommand {
public:
	static void executeCommand(Partie& partie, std::unique_ptr<Command> cmd);
	static void undoCommand(Partie& partie);
};
