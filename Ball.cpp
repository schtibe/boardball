#include <Eigen/Geometry>

#include "Ball.hpp"
#include "engine/Shader.hpp"

#include <iostream>

using namespace std;

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

Ball::Ball() : 
	xPos(0),
	yPos(0),
	zPos(0) 
{
	init ();
}

Ball::Ball(GLfloat xPos, GLfloat yPos, GLfloat zPos) : 
	xPos(xPos),
	yPos(yPos),
	zPos(zPos)
{
	init();
}

void Ball::init() {
	shader = Shader::loadShaders(
			"ball.vert",
			"ball.frag"
	);

	s_ballPos = glGetUniformLocation(shader, "ballPosition");

	initBallGeometry();
}

void Ball::initBallGeometry() {

	vertexBuffers = new GLuint;

	const float X = .525731112119133606;
	const float Z = .850650808352039932;

	GLfloat verts[12][3] = {
		{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
		{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
		{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
	};

	GLuint indices[] = {
		1,4,0,     4,9,0,    4,5,9,    8,5,4,    1,8,4,
		1,10,8,    10,3,8,   8,3,5,    3,2,5,    3,7,2,
		3,10,7,    10,6,7,   6,11,7,   6,0,11,   6,1,0,
		10,1,6,    11,0,9,   2,11,9,   5,2,9,    11,2,7
	};

	GLuint indexCount = 60;
	//GLfloat ballVertices[vertexCount * 3];

	long depth = 3;
	for (unsigned int i = 0; i < indexCount; i+=3) {
		float v1[3] = {
			verts[indices[i]][0],
			verts[indices[i]][1],
			verts[indices[i]][2]
		};
		float v2[3] = {
			verts[indices[i + 1]][0],
			verts[indices[i + 1]][1],
			verts[indices[i + 1]][2]
		};
		float v3[3] = {
			verts[indices[i + 2]][0],
			verts[indices[i + 2]][1],
			verts[indices[i + 2]][2]
		};
		subdivide(v1, v2, v3, depth);
	}
	vertexCount = ballVertices.size() / 3;

	/*
	for (unsigned int i = 0; i < vertexCount * 3; i++) {
		cout << ballVertices[i];

		if (i % 3 == 2) {
			cout << endl;
		}
		else {
			cout << ", ";
		}
	}
	*/

	glGenBuffers(1, vertexBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);

	glBufferData(
			GL_ARRAY_BUFFER,
			ballVertices.size() * sizeof(GLfloat),
			ballVertices.data(),
			GL_STATIC_DRAW
	);
}

void Ball::subdivide(float *v1, float *v2, float *v3, long depth) {
	GLfloat v12[3], v23[3], v31[3];

	if (depth == 0) {
		ballVertices.push_back(v1[0]);
		ballVertices.push_back(v1[1]);
		ballVertices.push_back(v1[2]);
		ballVertices.push_back(v2[0]);
		ballVertices.push_back(v2[1]);
		ballVertices.push_back(v2[2]);
		ballVertices.push_back(v3[0]);
		ballVertices.push_back(v3[1]);
		ballVertices.push_back(v3[2]);
		return;
	}

	for (GLuint i = 0; i < 3; i++) {
		v12[i] = (v1[i] + v2[i]) / 2.0;
		v23[i] = (v2[i] + v3[i]) / 2.0;
		v31[i] = (v3[i] + v1[i]) / 2.0;
	}

	normalize(v12);
	normalize(v23);
	normalize(v31);

	subdivide(v1, v12, v31, depth - 1);
	subdivide(v2, v23, v12, depth - 1);
	subdivide(v3, v31, v23, depth - 1);
	subdivide(v12, v23, v31, depth - 1);
}

void Ball::normalize(float v[3]) {
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (d == 0.0) {
		throw "Zero length vector";
	}

	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}

void Ball::draw() {
	glUseProgram(shader);

	GLfloat ballPos[3] = {xPos, yPos, zPos};
	glUniformMatrix3fv(s_ballPos, 1, GL_TRUE, ballPos);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);

	glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			BUFFER_OFFSET(0)
	);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
	/*
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i < 60; i++) {
		glNormal3f(
				ballVertices[(i * 3) + 0],
				ballVertices[(i * 3) + 1],
				ballVertices[(i * 3) + 2]
		);

		glVertex3f(
				ballVertices[(i * 3) + 0],
				ballVertices[(i * 3) + 1],
				ballVertices[(i * 3) + 2]
		);
	}
	glEnd();
	*/

	glDisableVertexAttribArray(0);

}

void Ball::doPhysics(GLuint time) {

}
