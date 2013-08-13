#ifndef __COLLIDING_OBJECT_H

#define __COLLIDING_OBJECT_H


#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include "Boundary.hpp"

#include <Eigen/Geometry>

using namespace Eigen;


using namespace std;

class CollisionInformation {
	public:
		bool collision = false;
		Vector3f collisionPoint;
		Boundary *movingBoundary;
		Boundary *staticBoundary;
		GLfloat angle;

		~CollisionInformation() {
			/*
			delete movingBoundary;
			delete staticBoundary;
			*/
		}
};

class CollidingObject {
	public:
		CollidingObject(GLint boundaryAmount, string identifier) :
			boundaryAmount(boundaryAmount),
			identifier(identifier) {}

		virtual Boundary** getBoundaries() = 0;
		virtual GLint getBoundaryAmount() {
			return boundaryAmount;
		}

		string getIdentifier() {
			return this->identifier;
		}

	protected:
		GLint boundaryAmount;
		string identifier;

};

#endif
