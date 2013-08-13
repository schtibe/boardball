
#include <iostream>
#include <cmath>

#include "CollisionHandler.hpp"
#include "utils/Linalg.hpp"

using namespace std;

// TODO since we have the good polygon intersection
// algorithm, we might consider to drop the crap
// here and do all stuff based on polygons
// Even quads can be easily reduced to a polygon
// ....
// Although the straight to polygon collision works
// pretty well
//
//
// TODO the draw thing is not very good... if there's a
// collision and we stop testing, the boundaries from
// there on won't be drawn

/**
 * Register a static object like walls
 */
void CollisionHandler::registerStaticObject(CollidingObject *object) {
	staticObjects.push_back(object);

	boundariesInTheSystem += object->getBoundaryAmount();
}

/**
 * Register a moving object
 *
 * A moving object also has a callback function.
 * The function may return true or false to indicate, if in
 * case of a collision the handler should carry on testing
 * with the other objects or not.
 */
void CollisionHandler::registerMovingObject(
		CollidingObject *object,
		collision_callback func) {

	MovingObject mo;
	mo.object = object;
	mo.func = func;

	boundariesInTheSystem += object->getBoundaryAmount();
	movingObjects.push_back(mo);
}

/**
 * Draw a boundary
 */
void CollisionHandler::drawBoundary(Boundary *boundary) {

	// colouring
	GLfloat steps = ceil(boundariesInTheSystem / 3.0f);
	GLfloat stepper = 1.0f / steps;

	if (drawnBoundaries < boundariesInTheSystem / 3) {
		redVal += stepper;
	}
	else if (drawnBoundaries < 2 * boundariesInTheSystem / 3) {
		greenVal -= stepper;
	}
	else {
		blueVal += stepper;
	}

	// TODO the color thing doesn't quite work
	// some boundaries are black :S
	GLfloat specular[] = {0.5, 0.7, 0.1, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, specular);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	if (StraightBoundary *s = dynamic_cast<StraightBoundary*>(boundary)) {
		drawBoundary(s);
	}
	else if (PolygonBoundary *poly = dynamic_cast<PolygonBoundary*>(boundary)) {
		drawBoundary(poly);
	}
	else if (PlaneBoundary *p = dynamic_cast<PlaneBoundary*>(boundary)) {
		drawBoundary(p);
	}

	drawnBoundaries++;

}

void CollisionHandler::drawBoundary(StraightBoundary *s) {
	drawLine(s->r, s->r + s->a);
}

void CollisionHandler::drawBoundary(PlaneBoundary *p) {
	drawLine(p->point1, p->point2);
	drawLine(p->point1, p->point3);
}

void CollisionHandler::drawBoundary(PolygonBoundary *p) {
	drawLine(p->point1, p->point2);
	drawLine(p->point1, p->point3);
	drawLine(p->point2, p->point3);

}

// draw from origin
void CollisionHandler::drawLine(Vector3f end) {
	drawLine(Vector3f(0, 0, 0), end);
}

void CollisionHandler::drawLine(Vector3f start, Vector3f end) {
	glLineWidth(2);
    glBegin(GL_LINES);
		glVertex3f(start.x(), start.y(), start.z());
		glVertex3f(end.x(),   end.y(),   end.z());
    glEnd();
}

void CollisionHandler::collide() {
	std::vector<MovingObject>::iterator mItr;
	std::vector<CollidingObject *>::iterator sItr;

	for (mItr = movingObjects.begin(); mItr != movingObjects.end(); ++mItr) {
		CollidingObject *movingObject = mItr->object;
		collision_callback func = mItr->func;

		Boundary** mBoundaries = movingObject->getBoundaries();
		GLint mBoundaryAmount = movingObject->getBoundaryAmount();

		// stop indicates whether the callback wants us to test
		// with the other static objects
		bool stop = false;
		for (sItr = staticObjects.begin(); sItr != staticObjects.end() && !stop; ++sItr) {
			CollidingObject *staticObject = *sItr;
			Boundary** sBoundaries = staticObject->getBoundaries();
			GLint sBoundaryAmount = staticObject->getBoundaryAmount();

			CollisionInformation cI = collideBoundaries(
					mBoundaries,
					mBoundaryAmount,
					sBoundaries,
					sBoundaryAmount);
			if (cI.collision) {
				stop = !func(movingObject, staticObject, cI);
			}
		}
	}

	drawnBoundaries = 0;
	redVal = 0;
	greenVal = 0;
	blueVal = 0;

	// TODO refactor
	if (showBoundaries && this->collisionPosition) {
			// TODO the color thing doesn't quite work
		// some boundaries are black :S
		GLfloat specular[] = {1.5, 0.0, 0.0, 1.0};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, specular);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

		glPointSize( 6.0 );
		glBegin(GL_POINTS);
			glVertex3f(
					collisionPosition->x(),
					collisionPosition->y(),
					collisionPosition->z()
			);
		glEnd();
	}


	// TODO refactor
	if (showBoundaries && collidingPolygon) {
		GLfloat specular[] = {1, 0.0, 0.0, 1.0};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, specular);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 10);

		glBegin(GL_POLYGON);
			glVertex3f(
					collidingPolygon->point1.x(),
					collidingPolygon->point1.y(),
					collidingPolygon->point1.z()
			);
			glVertex3f(
					collidingPolygon->point2.x(),
					collidingPolygon->point2.y(),
					collidingPolygon->point2.z()
			);
			glVertex3f(
					collidingPolygon->point3.x(),
					collidingPolygon->point3.y(),
					collidingPolygon->point3.z()
			);
		glEnd();
	}
}

CollisionInformation CollisionHandler::collideBoundaries(
		Boundary **mBoundaries,
		GLint    mBoundaryAmount,
		Boundary **sBoundaries,
		GLint    sBoundaryAmount
	) {

	CollisionInformation cI;

	for (int i = 0; i < mBoundaryAmount; i++) {
		Boundary *mBoundary = mBoundaries[i];

		if (showBoundaries) {
			drawBoundary(mBoundary);
		}

		for (int j = 0; j < sBoundaryAmount; j++) {
			Boundary *sBoundary = sBoundaries[j];

			if (showBoundaries) {
				drawBoundary(sBoundary);
			}

			PolygonBoundary  *p = dynamic_cast<PolygonBoundary*>(sBoundary);
			StraightBoundary  *s = dynamic_cast<StraightBoundary*>(mBoundary);

			if (p && s) {
				cI = collideStraightWithPolygon(s, p);
				if (cI.collision) {
					return cI;
				}
			}

		}
	}

	return cI;
}

CollisionInformation CollisionHandler::collideStraightWithPolygon(
		StraightBoundary *s,
		PolygonBoundary  *p
) {

	CollisionInformation cI = CollisionInformation();

	// Get the intersection
	Vector3f normal = Linalg::calculateNormal(p->point1, p->point2, p->point3);

	GLfloat numerator             = normal.dot(p->point1 - s->r);
	GLfloat denominator           = normal.dot(s->a);

	Vector3f intersection = s->r + (s->a * (numerator / denominator));

	if (isnan(intersection.x()) || isnan(intersection.y()) || isnan(intersection.z())) {
		return cI;
	}

	Vector3f absoluteA = s->r + s->a;


	if (
		min(s->r.x(), absoluteA.x()) <= intersection.x()
		&&
		max(s->r.x(), absoluteA.x()) >= intersection.x()
		&&
		min(s->r.y(), absoluteA.y()) <= intersection.y()
		&&
		max(s->r.y(), absoluteA.y()) >= intersection.y()
	) {
		// test if it's inside the polygon
		Vector3f w = intersection - p->point1;
		Vector3f u = p->point2 - p->point1;
		Vector3f v = p->point3 - p->point1;

		GLfloat wv = w.dot(v);
		GLfloat uv = u.dot(v);
		GLfloat wu = w.dot(u);
		GLfloat uu = u.dot(u);
		GLfloat vv = v.dot(v);

		GLfloat denominator = uu * vv - pow(uv, 2);

		GLfloat alphaNominator = wu * vv - wv * uv;
		GLfloat betaNominator  = wv * uu - wu * uv;

		GLfloat alpha = alphaNominator / denominator;
		GLfloat beta  = betaNominator / denominator;

		if (alpha < -0.001 || alpha > 1.001 || beta < -0.001 || beta > 1.001) {
			return cI;
		}

		if (alpha + beta  > 1.001) {
			return cI;
		}

		if (	(
						alpha >= 0.0005 && alpha <= 0.9995
					&&	beta >= 0.0005 && beta <= 0.9995
					&&	alpha * beta <= 0.9995
				)
			||
				alpha < 0.0005
			||
				beta < 0.0005
			||
				alpha + beta > 0.9995
		) {

			cI.staticBoundary = new PolygonBoundary(*p);
			cI.movingBoundary = new StraightBoundary(*s);
			cI.collisionPoint = intersection;
			cI.collision      = true;
			cI.angle          = Linalg::straightStraightAngle(normal, intersection + s->a);

			if (this->collisionPosition == NULL) {
				this->collisionPosition = new Vector3f(intersection);
			}

			if (this->collidingPolygon == NULL) {
				this->collidingPolygon = new PolygonBoundary(*p);
			}

			if (COLLISION_DEBUG) {

				cout << "Collision of straight " << s->getName() << " with polygon '"
					 << p->getName() << "'" <<  endl;
				cout << "Polygon: ; " <<  *p << endl;
				cout << "Straight's coords: " << s->r << ", " << s->a << endl;
				cout << endl;
			}
		}
	}

	return cI;
}


void CollisionHandler::toggleBoundaries() {
	showBoundaries = !showBoundaries;
}


