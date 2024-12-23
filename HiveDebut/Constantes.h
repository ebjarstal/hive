#pragma once

#include <Qstring>

const int    INDEX_MENU = 0;
const int    INDEX_NOUVELLE_PARTIE = 1;
const int    INDEX_JOUER_CONTRE_IA = 2;
const int    INDEX_JOUER_DEUX_JOUEURS = 3;
const int    INDEX_CHARGER_PARTIE = 4;
const int    INDEX_PARTIE_EN_COURS = 5;

const int    SCALE_BY = 20;
const int    TAILLE_PLATEAU = 15;
const int    NB_COUCHES = 5;
const int    ESPACEMENT_PIONS = 2;
const int    ESPACEMENT_VERTICAL_PIONS_PIOCHE = 2;
const int    NB_PIONS_DEBUT = 5;
const int    LARGEUR_ECRAN = 1024;
const int    HAUTEUR_ECRAN = 768;

const double p1_x = 1;
const double p1_y = 0;

const double p2_x = 2;
const double p2_y = 0.58;

const double p3_x = 2;
const double p3_y = 1.73;

const double p4_x = 1;
const double p4_y = 2.31;

const double p5_x = 0;
const double p5_y = 1.73;

const double p6_x = 0;
const double p6_y = 0.58;

const double LARGEUR_PIONS = (p2_x - p6_x) * SCALE_BY;
const double HAUTEUR_PIONS = (p4_y - p1_y) * SCALE_BY;
const double PRESQUE_DEUX_TIERS_HAUTEUR_PION = (p3_y - p1_y) * SCALE_BY;