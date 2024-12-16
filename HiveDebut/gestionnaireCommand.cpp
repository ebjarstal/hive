#include "GestionnaireCommand.h"
#include "partie.h"

void GestionnaireCommand::executeCommand(Partie& partie, Command* cmd) {
    // Ex�cuter la commande
    cmd->execute();
    // Ajouter la commande � l'historique
    partie.getHistorique().push(cmd);
}

void GestionnaireCommand::undoCommand(Partie& partie) {
    if (!partie.getHistorique().empty()) {

        // Obtenir la derni�re commande
        auto cmd = std::move(partie.getHistorique().top());
        partie.getHistorique().pop();

        // Annuler la commande
        cmd->undo();

    }
}