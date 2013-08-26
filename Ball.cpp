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

	vertexCount = 60;
	//GLfloat ballVertices[vertexCount * 3];

	for (unsigned int i = 0; i < vertexCount; i++) {
		ballVertices[(3 * i) + 0] = verts[indices[i]][0];
		ballVertices[(3 * i) + 1] = verts[indices[i]][1];
		ballVertices[(3 * i) + 2] = verts[indices[i]][2];
	}

	for (unsigned int i = 0; i < vertexCount * 3; i++) {
		cout << ballVertices[i];

		if (i % 3 == 2) {
			cout << endl;
		}
		else {
			cout << ", ";
		}
	}

	glGenBuffers(1, vertexBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);

	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(ballVertices),
			ballVertices,
			GL_STATIC_DRAW
	);

}

void Ball::draw() {
	//glUseProgram(shader);

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

	//glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	
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

	glDisableVertexAttribArray(0);

}

void Ball::doPhysics(GLuint time) {

}
