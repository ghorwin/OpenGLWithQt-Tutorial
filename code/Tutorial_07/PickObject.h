#ifndef PICKOBJECT_H
#define PICKOBJECT_H

#include <QVector3D>

struct PickObject {
	PickObject(float z, unsigned int id) :
		m_z(z), m_objectId(id), m_faceID(0)
	{
	}
	PickObject(float z, unsigned int id, unsigned int faceId) :
		m_z(z), m_objectId(id), m_faceID(faceId)
	{
	}

	float m_z; // the depths of the intersection point
	unsigned int m_objectId; // the object clicked on
	unsigned int m_faceID; // the actual triangle/plane clicked on
};


/*! Tests if a line (with equation p = p1 + t * d) hits a plane, defined by
	p = x * a  +  y * b. Returns true if intersection is found, and returns
	the z-component of intersection point.
*/
bool intersectsPlane(const QVector3D & a,
				const QVector3D & b,
				const QVector3D & normal,
				const QVector3D & offset,
				const QVector3D & p1,
				const QVector3D & d,
				float & z);


#endif // PICKOBJECT_H
