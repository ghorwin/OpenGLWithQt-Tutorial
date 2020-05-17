#ifndef PICKOBJECT_H
#define PICKOBJECT_H

#include <QVector3D>

/*! An Object to hold information on the clicked-on object. */
struct PickObject {
	PickObject(float dist, unsigned int id) :
		m_dist(dist), m_objectId(id), m_faceId(0)
	{
	}
	PickObject(float dist, unsigned int id, unsigned int faceId) :
		m_dist(dist), m_objectId(id), m_faceId(faceId)
	{
	}

	float m_dist; // the normalized distance of the intersection point from starting point of pick line
	unsigned int m_objectId; // the object clicked on
	unsigned int m_faceId; // the actual triangle/plane clicked on
};


/*! Tests if a line (with equation p = p1 + t * d) hits a plane, defined by
	p = x * a  +  y * b. Returns true if intersection is found, and returns
	the normalized distance (t) between intersection point and point p1.
*/
bool intersectsRect(const QVector3D & a,
				const QVector3D & b,
				const QVector3D & normal,
				const QVector3D & offset,
				const QVector3D & p1,
				const QVector3D & d,
				float & dist);


#endif // PICKOBJECT_H
