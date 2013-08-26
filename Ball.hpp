#ifndef BALL_HPP

#define BALL_HPP

#include "engine/DrawObject.hpp"

class Ball : public DrawObject {
	public:
		Ball();
		Ball(GLfloat xPos, GLfloat yPos, GLfloat zPos);

		void draw();
		void doPhysics(GLuint time);

	private:
		void initBallGeometry();
		void init();
		GLfloat xPos, yPos, zPos;

		GLuint *vertexBuffers;
		GLuint vertexCount;

		GLfloat ballVertices[60 * 3];

		GLuint shader;

		GLuint s_ballPos;
};


#endif /* end of include guard: BALL_HPP */
