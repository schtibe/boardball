#ifndef __COLLISION_HANDLER_H

#define __COLLISION_HANDLER_H

#include <vector>
#include <boost/function.hpp>

#include "CollidingObject.hpp"

#define COLLISION_DEBUG true

typedef boost::function<bool (CollidingObject *,
					CollidingObject *,
					CollisionInformation
)> collision_callback;

#define COLLISION_CALLBACK \
				bool (*func)( \
					CollidingObject *, \
					CollidingObject *, \
					CollisionInformation \
					)

// TODO if one would want to take this
// one step further, one would consider
// that moving objects may collide with moving
// objects
// Would another possibility be to tell
// the colliding object to implement some
// collide method... I don't think so,
// like that nothing external can be done


struct MovingObject {
	CollidingObject *object;
	collision_callback func;
};


class CollisionHandler {
	public:
		void collide();

		void registerStaticObject(CollidingObject *);
		void registerMovingObject(
				CollidingObject *,
				collision_callback
		);

		CollisionInformation collideBoundaries(Boundary **, GLint, Boundary **, GLint);
		//CollisionInformation collidePointWithPlane(PointBoundary*, PlaneBoundary*);
		//CollisionInformation collideStraightWithPlane(StraightBoundary*, PlaneBoundary*);
		CollisionInformation collideStraightWithPolygon(StraightBoundary*, PolygonBoundary*);

		void toggleBoundaries();

	private:
		std::vector<CollidingObject*> staticObjects;
		std::vector<MovingObject> movingObjects;

		bool showBoundaries;

		void drawBoundary(Boundary *);
		void drawBoundary(StraightBoundary *);
		void drawBoundary(PlaneBoundary *);
		void drawBoundary(PolygonBoundary *);

		void drawLine(Vector3f);
		void drawLine(Vector3f, Vector3f);

		GLuint boundariesInTheSystem = 0;
		GLuint drawnBoundaries = 0;
		GLfloat redVal   = 0;
		GLfloat greenVal = 0;
		GLfloat blueVal  = 0;

		Vector3f *collisionPosition = NULL;
		PolygonBoundary *collidingPolygon  = NULL;
};

#endif
