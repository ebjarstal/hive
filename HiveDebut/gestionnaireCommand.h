#pragma once

#include <stack>
#include "command.h"

class Partie;

class GestionnaireCommand {
public:
	static void executeCommand(Partie& partie, Command* cmd);
	static void undoCommand(Partie& partie);

};