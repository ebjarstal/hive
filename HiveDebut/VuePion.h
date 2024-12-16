#ifndef VUE_PION_H
#define VUE_PION_H

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include "pions.h"
#include <string>

#define RED "\033[31m"
#define WHITE "\033[37m"
#define RESET "\033[37m"

class VuePion : public QObject, public QGraphicsPolygonItem {
    Q_OBJECT

public:
    // constructeur
    VuePion(QGraphicsItem* parent = nullptr);

    // getters
    QColor  getCouleur();
    bool    getEstPose();
    QString getType();

    Pion* getPionAssocie() { return pionAssocie; }

    // setters
    void setAttributs(std::string nouvelleCouleur, bool estPose, QString type);
    void setAttributs(QColor nouvelleCouleur, bool estPose, QString type);

    void setPionAssocie(Pion* pion);

    void setCouleur(std::string nouvelleCouleur);
    void setCouleur(QColor nouvelleCouleur);
    void setEstPose(bool nouvelEtat);
    void setType(QString type);

    // event
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    Pion* pionAssocie;

    QColor  couleur;
    bool    estPose;
    QString type;

signals:
    void pionClique(VuePion* pion);
};

#endif // VUE_PION_H
