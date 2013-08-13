
#ifndef LINALG_H

#define LINALG_H

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glx.h>


#undef Success
#include <Eigen/Geometry>



using namespace Eigen;

class Linalg {
	public:
		static GLfloat planeStraightAngle(Vector3f, Vector3f);
		static GLfloat straightStraightAngle(Vector3f, Vector3f);
		static Vector3f calculateNormal(Vector3f, Vector3f, Vector3f);

		static Vector3f reflectVector(Vector3f vector, Vector3f normal);
};

#endif

