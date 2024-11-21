#include "GestionnaireCommand.h"

std::stack<Command*> GestionnaireCommand::historique;

void GestionnaireCommand::executeCommand(Command* cmd) {
    // Exécuter la commande
    cmd->execute();
    // Ajouter la commande à l'historique
    historique.push(cmd);
}

void GestionnaireCommand::undoCommand() {
    if (!historique.empty()) {

        // Obtenir la dernière commande
        auto cmd = std::move(historique.top());
        historique.pop();

        // Annuler la commande
        cmd->undo();

    }
}