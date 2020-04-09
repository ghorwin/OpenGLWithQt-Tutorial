#include "PickObject.h"

#include <QDebug>

/* Solves equation system with Cramer's rule:
	 a x + c y = e
	 b x + d y = f
*/
bool solve(double a, double b, double c,  double d,  double e,  double f, double & x, double & y) {
	double det = a*d - b*c;
	if (det == 0.)
		return false;

	x = (e*d - c*f)/det;
	y = (a*f - e*b)/det;
	return true;
}


bool intersectsRect(const QVector3D & a,
				const QVector3D & b,
				const QVector3D & normal,
				const QVector3D & offset,
				const QVector3D & p1,
				const QVector3D & d,
				float & z)
{
	// first the normal test

	double angle = QVector3D::dotProduct(d, normal)/qAbs(d.length());
	if (angle >= 0)
		return false; // same direction, no intersection possible

	// compute intersection point on line
	double t = QVector3D::dotProduct(offset - p1, normal) / QVector3D::dotProduct(d, normal);
	// outside viewing range?
	if (t < 0 || t > 1)
		return false;

	// now determine location on plane
	QVector3D x0 = p1 + t*d;

	QVector3D rhs = x0 - offset; // right hand side of equation system:  a * x  +  b * y = (x - offset)

	// we have three possible ways to get the intersection point, try them all until we succeed
	double x,y;
	// rows 1 and 2
	if (solve(a.x(), a.y(), b.x(), b.y(), rhs.x(), rhs.y(), x, y)) {
		if (x > 0 && x < 1 && y > 0 && y < 1) {
			z = x0.z();
			qDebug() << "Intersection found (1) at z = " << z;
			return true;
		}
		else
			return false;
	}
	// rows 1 and 3
	if (solve(a.x(), a.z(), b.x(), b.z(), rhs.x(), rhs.z(), x, y)) {
		if (x > 0 && x < 1 && y > 0 && y < 1) {
			z = x0.z();
			qDebug() << "Intersection found (2) at z = " << z;
			return true;
		}
		else
			return false;
	}
	// rows 2 and 3
	if (solve(a.y(), a.z(), b.y(), b.z(), rhs.y(), rhs.z(), x, y)) {
		if (x > 0 && x < 1 && y > 0 && y < 1) {
			z = x0.z();
			qDebug() << "Intersection found (3) at z = " << z;
			return true;
		}
		else
			return false;
	}

	return false;
}

