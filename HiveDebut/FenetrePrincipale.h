#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QFileDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>
#include "Constantes.h"
#include "Controleur.h"

class FenetrePrincipale : public QMainWindow {
    Q_OBJECT

public:
    FenetrePrincipale(QWidget* parent = nullptr);
    ~FenetrePrincipale();

private:
    void initPageMenu();
    void initPageNouvellePartie();
    void initPageJouerContreIA();
    void initPageJouerDeuxJoueurs();
    void initPageChargerPartie();

    void afficherNouvellePartie();
    void afficherJouerContreIA();
    void afficherJouerDeuxJoueurs();
    void afficherChargerPartie();

    void retourMenu();
    void retourNouvellePartie();

    void ouvrirFileDialog();

    QWidget* creerPage(const QString& title, QPushButton*& boutonRetour, bool ajouterBoutonRetour = true);
    void ajouterLabelAvecLineEdit(QVBoxLayout* layout, const QString& labelText, QLineEdit*& lineEdit, int maxWidth = 400);
    void ajouterCheckbox(QVBoxLayout* layout, const QString& labelText, QCheckBox*& checkBox, int maxWidth = 400);
    void ajouterBouton(QVBoxLayout* layout, const QString& buttonText, QPushButton*& button, int minWidth = 200, int minHeight = 50, int maxWidth = 400);

    QGridLayout* gridLayout;
    QStackedWidget* stackedWidget;
    QWidget* widgetCentral;
    QLabel* labelEtat;

    QPushButton* boutonNouvellePartie;
    QPushButton* boutonJouerContreIA;
    QPushButton* boutonJouerDeuxJoueurs;
    QPushButton* boutonChargerPartie;
    QPushButton* boutonQuitter;

    QLabel* labelFichierCharge;
    QPushButton* boutonLancerJeu;

    QLineEdit* champNomJoueur1;
    QLineEdit* champNomJoueur2;
    QLineEdit* champNomSauvegarde;
    QLineEdit* champNombreRetours;
    QCheckBox* checkboxExtensionMoustique;
    QCheckBox* checkboxExtensionCoccinelle;
    QCheckBox* checkboxExtensionAraignee;
    QPushButton* boutonCommencerPartieContreIA;
    QPushButton* boutonCommencerPartieDeuxJoueurs;

    QPushButton* boutonRetourNouvellePartie;
    QPushButton* boutonRetourJouerContreIA;
    QPushButton* boutonRetourJouerDeuxJoueurs;
    QPushButton* boutonRetourChargerPartie;

    QPushButton* boutonOuvrirFichier;
    QLabel* labelCheminFichier;

    Controleur* controleur;

private slots:
    void commencerPartieContreIA();
    void commencerPartieDeuxJoueurs();
    void onMiseAJourPlateau();
    void onPartieTerminee(const QString& message);
};
