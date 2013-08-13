#ifndef _CAMERA_H
#define _CAMERA_H

// This was created to allow you to use this camera code in your projects
// without having to cut and paste code.  This file and camera.cpp should be
// added to your project.
#include <cmath>
#include <SDL/SDL.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#undef Success

#include <Eigen/Geometry>

using namespace Eigen;

// This is our camera class
class Camera {
	public:
		Camera();
		Camera(GLint screenWidth, GLint screenHeight);

		Vector3f getPosition();
		Vector3f getView();
		Vector3f getUpVector();
		Vector3f getStrafe();

		void positionCamera(
				GLfloat positionX,
				GLfloat positionY,
				GLfloat positionZ,
				GLfloat viewX,
				GLfloat viewY,
				GLfloat viewZ,
				GLfloat upVectorX,
				GLfloat upVectorY,
				GLfloat upVectorZ
		);

		void positionCamera(
				Vector3f position,
				Vector3f view,
				Vector3f upVector
		);

		void rotateView(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
		void setViewByMouse(GLint mouseX, GLint mouseY);

		void strafeCamera(GLfloat speed);
		void moveCamera(GLfloat speed);
		void update(GLint mouseX, GLint mouseY);
		void look();

		void windowResize(GLint screenWidth, GLint screenHeight);

	private:
		GLint screenWidth, screenHeight;

		/**
		 * The camera's position
		 */
		Vector3f position;

		/**
		 * The camera's view
		 */
		Vector3f view;

		/**
		 * The camera's up vector
		 */
		Vector3f upVector;

		/**
		 * The camera's strafe vector
		 */
		Vector3f strafe;
};


#endif

