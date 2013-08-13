#ifndef __BOUNDARY_H

#define __BOUNDARY_H


#include <string>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>


#undef Success

#include <Eigen/Geometry>


using namespace std;
using namespace Eigen;

enum BoundaryType {
	point,
	straight,
	polygon,
	rectangle,
	plane
};

class Boundary {
	public:
		virtual BoundaryType getBoundaryType();

		virtual string getName();
	protected:
		string       name; // change back
		BoundaryType boundaryType;
};

class PointBoundary : public Boundary {
	public:
		PointBoundary(string, GLfloat, GLfloat, GLfloat);

		GLfloat x;
		GLfloat y;
		GLfloat z;
};

class StraightBoundary : public Boundary{
	public:
		StraightBoundary(string , Vector3f , Vector3f );
		Vector3f r;
		Vector3f a;

		friend ostream& operator<< (ostream &out, StraightBoundary &s);
};


class PlaneBoundary : public Boundary {
	public:
		PlaneBoundary();
		PlaneBoundary(string name);
		PlaneBoundary(string, Vector3f, Vector3f, Vector3f);

		Vector3f point1;
		Vector3f point2;
		Vector3f point3;
};

class PolygonBoundary : public Boundary{
	public:
		PolygonBoundary();
		PolygonBoundary(string name);

		Vector3f normal;

		Vector3f point1;
		Vector3f point2;
		Vector3f point3;


		friend ostream& operator<< (ostream &out, PolygonBoundary &p);

		bool operator== (PolygonBoundary &other);
};


#endif
