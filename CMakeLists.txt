cmake_minimum_required(VERSION 3.16)
project(Hive VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 COMPONENTS Core Gui Widgets REQUIRED)
endif()

set(SOURCES
    HiveDebut/main.cpp
    HiveDebut/command.cpp
    HiveDebut/Controleur.cpp
    HiveDebut/FenetrePrincipale.cpp
    HiveDebut/gestionnaireCommand.cpp
    HiveDebut/gestionnaireMouvements.cpp
    HiveDebut/gestionnairePions.cpp
    HiveDebut/gestionnaireSauvegarde.cpp
    HiveDebut/gestionnaireVoisins.cpp
    HiveDebut/joueur.cpp
    HiveDebut/mouvement.cpp
    HiveDebut/partie.cpp
    HiveDebut/pions.cpp
    HiveDebut/plateau.cpp
    HiveDebut/usine.cpp
    HiveDebut/VuePion.cpp
    HiveDebut/VuePlateau.cpp
)

add_executable(Hive ${SOURCES})
target_include_directories(Hive PRIVATE HiveDebut)

if(Qt6_FOUND)
    target_link_libraries(Hive PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
else()
    target_link_libraries(Hive PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
endif()
