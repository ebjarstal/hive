#ifndef VUE_PION_H
#define VUE_PION_H

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>

class VuePion : public QGraphicsPolygonItem {
public:
    // constructeur
    VuePion(QGraphicsItem* parent = nullptr);

    // getters
    QColor  getCouleur();
    bool    getEstPose();
    QString getType();

    // setters
    void setCouleur(QColor nouvelleCouleur);
    void setEstPose(bool nouvelEtat);
    void setType(QString type);

    // event
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

private:
    QColor  couleur;
    bool    estPose;
    QString type;
};

#endif // VUE_PION_H
