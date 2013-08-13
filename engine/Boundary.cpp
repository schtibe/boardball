#include <algorithm>

#include "Boundary.hpp"

BoundaryType Boundary::getBoundaryType() {
	return this->boundaryType;
}

string Boundary::getName() {
	return this->name;
}





PointBoundary::PointBoundary(string name, GLfloat xPos, GLfloat yPos, GLfloat zPos) :
	x(xPos),
	y(yPos),
	z(zPos)
{
	boundaryType = point;
	this->name         = name;
}





StraightBoundary::StraightBoundary(string name, Vector3f r, Vector3f a) :
	r(r),
	a(a)
{
	boundaryType = straight;
	this->name   = name;
}


ostream& operator<< (ostream &out, StraightBoundary &p) {
	out << "Straight boundary \"" << p.getName() << "\"; "
		<< "r: " << p.r << ", "
		<< "a: " << p.a;

	return out;
}


PlaneBoundary::PlaneBoundary() :
	point1(Vector3f(0, 0, 0)),
	point2(Vector3f(0, 0, 0)),
	point3(Vector3f(0, 0, 0))
{
	name = "plane boundary";
	boundaryType = plane;
}


PlaneBoundary::PlaneBoundary(string name) :
	point1(Vector3f(0, 0, 0)),
	point2(Vector3f(0, 0, 0)),
	point3(Vector3f(0, 0, 0))
{
	this->name   = name;
	boundaryType = plane;
}


PlaneBoundary::PlaneBoundary(
		string name,
		Vector3f point1,
		Vector3f point2,
		Vector3f point3

	):
	point1(point1),
	point2(point2),
	point3(point3)
{
	this->name   = name;
	boundaryType = plane;
}


PolygonBoundary::PolygonBoundary() {
	name = "plane boundary";
	boundaryType = polygon;
}


PolygonBoundary::PolygonBoundary(string name) {
	this->name = name;
	boundaryType = polygon;
}

bool PolygonBoundary::operator==(PolygonBoundary &other) {
	return
		this->point1 == other.point1
		&&
		this->point2 == other.point2
		&&
		this->point3 == other.point3;
}

ostream& operator<< (ostream &out, PolygonBoundary &p) {
	out << "Polygon \"" << p.name  << "\"; "
		<< "point1: " << p.point1 << ", "
		<< "point2: " << p.point2 << ", "
		<< "point3: " << p.point3 << ", "
		<< "normal: " << p.normal;

	return out;
}

