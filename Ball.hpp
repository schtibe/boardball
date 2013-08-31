#ifndef BALL_HPP

#define BALL_HPP

#include "engine/DrawObject.hpp"
#include "boost/container/vector.hpp"

using boost::container::vector;

class Ball : public DrawObject {
	public:
		Ball();
		Ball(GLfloat xPos, GLfloat yPos, GLfloat zPos);

		void draw();
		void doPhysics(GLuint time);

	private:
		void initBallGeometry();
		void subdivide(float *v1, float *v2, float *v3, long depth);
		void normalize(float v[3]);
		void init();
		GLfloat xPos, yPos, zPos;

		GLuint *vertexBuffers;
		GLuint vertexCount;

		boost::container::vector<GLfloat> ballVertices;

		GLuint shader;

		GLuint s_ballPos;
};


#endif /* end of include guard: BALL_HPP */
