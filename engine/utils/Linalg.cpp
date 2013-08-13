#include "Linalg.hpp"

#include <cmath>

/**
 * The angle between a straight and a plane
 */
GLfloat Linalg::planeStraightAngle(Vector3f planeNormal, Vector3f straight) {

	GLfloat numer = planeNormal.dot(straight);
	GLfloat denom = planeNormal.norm() * straight.norm();

	GLfloat fraction = numer / denom;
	if (fraction > 1) {
		fraction = 1;
	}

	return asin(fraction);
}

/**
 * The angle between two straights
 */
GLfloat Linalg::straightStraightAngle(Vector3f s1, Vector3f s2) {

	GLfloat numer = s1.dot(s2);
	GLfloat denom = s1.norm() * s2.norm();

	GLfloat fraction = numer / denom;
	if (fraction > 1) {
		fraction = 1;
	}

	return acos(fraction);
}

/**
 * Calculate a normal [of a plane] with three points lying on that plane
 */
Vector3f Linalg::calculateNormal(Vector3f point1, Vector3f point2, Vector3f point3) {
	Vector3f planeDirection1 = point2 - point1;
	Vector3f planeDirection2 = point3 - point1;

	Vector3f normal = planeDirection1.cross(planeDirection2);

	return normal;
}

/**
 * Reflect a vector
 *
 * Reflect a vector from a wall. The wall is described
 * by the given normal
 * v' = 2 * (v . n) * n - v
 * v' = v' * -1
 */
Vector3f Linalg::reflectVector(Vector3f vector, Vector3f normal) {
	normal.normalize();
	vector =  (normal * 2 * (vector.dot(normal))) - vector;
	vector *= -1;

	return vector;
}



