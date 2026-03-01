#include "gestionnaireCommand.h"
#include "partie.h"

void GestionnaireCommand::executeCommand(Partie& partie, std::unique_ptr<Command> cmd) {
    // Exécuter la commande
    cmd->execute();
    // Ajouter la commande à l'historique
    partie.getHistorique().push(std::move(cmd));
}

void GestionnaireCommand::undoCommand(Partie& partie) {
    if (!partie.getHistorique().empty()) {
        // Obtenir et retirer la dernière commande
        auto cmd = std::move(partie.getHistorique().top());
        partie.getHistorique().pop();
        // Annuler la commande (cmd est libéré automatiquement en sortant de portée)
        cmd->undo();
    }
}
