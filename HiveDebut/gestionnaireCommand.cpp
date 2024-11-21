#include "GestionnaireCommand.h"

std::stack<std::unique_ptr<Command>> GestionnaireCommand::historique;

void GestionnaireCommand::executeCommand(std::unique_ptr<Command> cmd) {
    // Ex�cuter la commande
    cmd->execute();
    // Ajouter la commande � l'historique
    historique.push(std::move(cmd));
}

void GestionnaireCommand::undoCommand() {
    if (!historique.empty()) {

        // Obtenir la derni�re commande
        auto cmd = std::move(historique.top());
        historique.pop();

        // Annuler la commande
        cmd->undo();

    }
}