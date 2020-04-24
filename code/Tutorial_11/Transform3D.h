/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

Code is taken from https://www.trentreed.net/blog/qt5-opengl-part-3b-camera-control

************************************************************************************/


#ifndef TRANSFORM3D_H
#define TRANSFORM3D_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

/*! A typical 3D transformation class.
	Implements lazy evaluation - calling any of the modification functions
	will only store info about the modification - only by retrieving the
	matrix with toMatrix(), the actual matrix is being returned.
*/
class Transform3D {
public:
	// Constructors
	Transform3D();

	// Transform By (Add/Scale)
	void translate(const QVector3D &dt);
	void translate(float dx, float dy, float dz);
	void scale(const QVector3D &ds);
	void scale(float dx, float dy, float dz);
	void scale(float factor);
	void rotate(const QQuaternion &dr);
	void rotate(float angle, const QVector3D &axis);
	void rotate(float angle, float ax, float ay, float az);
	void grow(const QVector3D &ds);
	void grow(float dx, float dy, float dz);
	void grow(float factor);

	// Transform To (Setters)
	void setTranslation(const QVector3D &t);
	void setTranslation(float x, float y, float z);
	void setScale(const QVector3D &s);
	void setScale(float x, float y, float z);
	void setScale(float k);
	void setRotation(const QQuaternion &r);
	void setRotation(float angle, const QVector3D &axis);
	void setRotation(float angle, float ax, float ay, float az);

	// Accessors
	const QVector3D& translation() const;
	const QVector3D& scale() const;
	const QQuaternion& rotation() const;
	const QMatrix4x4& toMatrix() const;

protected:
	QVector3D m_translation;
	QVector3D m_scale;
	QQuaternion m_rotation;
	mutable QMatrix4x4 m_world; // is updated in the const toMatrix() function
	mutable bool m_dirty;
//	char _padding[3]; // additional padding characters to align class to 4 byte boundary (if missing, compiler would do this automatically)

#ifndef QT_NO_DATASTREAM
	friend QDataStream &operator<<(QDataStream &out, const Transform3D &transform);
	friend QDataStream &operator>>(QDataStream &in, Transform3D &transform);
#endif
};

Q_DECLARE_TYPEINFO(Transform3D, Q_MOVABLE_TYPE);

inline Transform3D::Transform3D() : m_scale(1.0f, 1.0f, 1.0f), m_dirty(true) {}

// Transform By (Add/Scale)
inline void Transform3D::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void Transform3D::scale(float dx, float dy,float dz) { scale(QVector3D(dx, dy, dz)); }
inline void Transform3D::scale(float factor) { scale(QVector3D(factor, factor, factor)); }
inline void Transform3D::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Transform3D::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }
inline void Transform3D::grow(float dx, float dy, float dz) { grow(QVector3D(dx, dy, dz)); }
inline void Transform3D::grow(float factor) { grow(QVector3D(factor, factor, factor)); }

// Transform To (Setters)
inline void Transform3D::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); }
inline void Transform3D::setScale(float x, float y, float z) { setScale(QVector3D(x, y, z)); }
inline void Transform3D::setScale(float k) { setScale(QVector3D(k, k, k)); }
inline void Transform3D::setRotation(float angle, const QVector3D &axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Transform3D::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Accessors
inline const QVector3D& Transform3D::translation() const { return m_translation; }
inline const QVector3D& Transform3D::scale() const { return m_scale; }
inline const QQuaternion& Transform3D::rotation() const { return m_rotation; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const Transform3D &transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const Transform3D &transform);
QDataStream &operator>>(QDataStream &in, Transform3D &transform);
#endif

#endif // TRANSFORM3D_H
