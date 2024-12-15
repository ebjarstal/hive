﻿#pragma once

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
#include <QIntValidator>
#include <QGraphicsScene>
#include <QGraphicsView>
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

    QWidget* pagePartieEnCours;

    void afficherNouvellePartie();
    void afficherJouerContreIA();
    void afficherJouerDeuxJoueurs();
    void afficherChargerPartie();

    void retourMenu();
    void retourNouvellePartie();

    QWidget* creerPage(const QString& title, QPushButton*& boutonRetour, bool ajouterBoutonRetour = true);
    void ajouterLabelAvecLineEditTexte(QVBoxLayout* layout, const QString& labelText, QLineEdit*& lineEdit, int maxWidth = 400);
    void ajouterLabelAvecLineEditNombre(QVBoxLayout* layout, const QString& labelText, QLineEdit*& lineEdit, int maxWidth = 400);
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
    QPushButton* boutonChargerPartieSauvegarde;

    QLineEdit* champNomJoueur1IA;
    QLineEdit* champNomSauvegardeIA;
    QLineEdit* champNombreRetoursIA;

    QLineEdit* champNomJoueur1DeuxJoueurs;
    QLineEdit* champNomJoueur2DeuxJoueurs;
    QLineEdit* champNomSauvegardeDeuxJoueurs;
    QLineEdit* champNombreRetoursDeuxJoueurs;

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
    QLabel* labelNomSauvegarde;

    QGraphicsScene* scene;
    VuePlateau* vuePlateau;
    QGraphicsView* vuePartie;

    Controleur* controleur;

private slots:
    void commencerPartieContreIA();
    void commencerPartieDeuxJoueurs();

    void ouvrirFileDialog();
    void chargerPartieSauvegarde();

    void afficherPlateauDebut();
    void afficherPiochesDebut();

    void dessinerPanneauJoueur(int x, int y, int largeur, int hauteur, QColor couleur, double opacite);

    void onMiseAJourPlateau();
    void onPartieTerminee(const QString& message);
};