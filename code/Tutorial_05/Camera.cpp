#include "Camera.h"
#include <QDebug>

const QVector3D Camera::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera::LocalRight(1.0f, 0.0f, 0.0f);

// Transform By (Add/Scale)
void Camera::translate(const QVector3D &dt)
{
  m_dirty = true;
  m_translation += dt;
}

void Camera::rotate(const QQuaternion &dr)
{
  m_dirty = true;
  m_rotation = dr * m_rotation;
}

// Transform To (Setters)
void Camera::setTranslation(const QVector3D &t)
{
  m_dirty = true;
  m_translation = t;
}

void Camera::setRotation(const QQuaternion &r)
{
  m_dirty = true;
  m_rotation = r;
}

// Accessors
const QMatrix4x4 &Camera::toMatrix()
{
  if (m_dirty)
  {
	m_dirty = false;
	m_world.setToIdentity();
	m_world.rotate(m_rotation.conjugated());
	m_world.translate(-m_translation);
  }
  return m_world;
}

// Queries
QVector3D Camera::forward() const
{
  return m_rotation.rotatedVector(LocalForward);
}

QVector3D Camera::up() const
{
  return m_rotation.rotatedVector(LocalUp);
}

QVector3D Camera::right() const
{
  return m_rotation.rotatedVector(LocalRight);
}

// Qt Streams
QDebug operator<<(QDebug dbg, const Camera &transform)
{
  dbg << "Camera\n{\n";
  dbg << "Position: <" << transform.translation().x() << ", " << transform.translation().y() << ", " << transform.translation().z() << ">\n";
  dbg << "Rotation: <" << transform.rotation().x() << ", " << transform.rotation().y() << ", " << transform.rotation().z() << " | " << transform.rotation().scalar() << ">\n}";
  return dbg;
}

QDataStream &operator<<(QDataStream &out, const Camera &transform)
{
  out << transform.m_translation;
  out << transform.m_rotation;
  return out;
}

QDataStream &operator>>(QDataStream &in, Camera &transform)
{
  in >> transform.m_translation;
  in >> transform.m_rotation;
  transform.m_dirty = true;
  return in;
}

