#ifndef VUE_PION_H
#define VUE_PION_H

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>

#define u_i unsigned int

struct Coord3D {
	u_i x, y, z;
	Coord3D(u_i x_, u_i y_, u_i z_) : x(x_), y(y_), z(z_) {}
};

class VuePion : public QGraphicsPolygonItem {
public:
	// constructeur
	VuePion(QGraphicsItem* parent = nullptr);

	// getters
	// std::list<Coord3D> getEmplacementsPossibles();
	// std::list<Coord3D> getDeplacementsPossibles();
	bool getEstPose();
	QString getProprietaire();

	// setters
	void setProprietaire(QString joueur);
	void setEstPose(bool etat);

	// event
	void mousePressEvent(QGraphicsSceneMouseEvent* event);

private:
	bool estPose;
	QString proprietaire;
	QString type;
};

#endif // VUE_PION_H