#include "Camera.hpp"

#include <iostream>

using namespace std;

Camera::Camera() {

}

/**
 * Constructor
 *
 * @todo overload this and let it call the position
 * camera method
 */
Camera::Camera(GLint screenWidth, GLint screenHeight)
		// init
		: screenWidth    (screenWidth),
			screenHeight (screenHeight),
			position     (Vector3f(0.0, 0.0, 1.0)),
			view         (Vector3f(0.0, 1.0, 0.0)),
			upVector     (Vector3f(0.0, 1.0, 0.0))
{
}

/**
 * Return the position vector
 */
Vector3f Camera::getPosition() {
	return position;
}

/**
 * Return the view vector
 */
Vector3f Camera::getView() {
	return view;
}

/**
 * Return the up vector
 */
Vector3f Camera::getUpVector() {
	return upVector;
}

/**
 * Return the strafe vector
 */
Vector3f Camera::getStrafe() {
	return strafe;
}

/**
 * This changes the position, view, and up vector of the camera.
 * This is primarily used for initialization
 */
void Camera::positionCamera(
		GLfloat positionX,
		GLfloat positionY,
		GLfloat positionZ,
		GLfloat viewX,
		GLfloat viewY,
		GLfloat viewZ,
		GLfloat upVectorX,
		GLfloat upVectorY,
		GLfloat upVectorZ
	)
{
	this->position = Vector3f(positionX, positionY, positionZ);
	this->view     = Vector3f(viewX, viewY, viewZ);
	this->upVector = Vector3f(upVectorX, upVectorY, upVectorZ);

	Vector3f direction = view - position;
	strafe = direction.cross(upVector);
	strafe.normalize();
}

/**
 * Change the position, view and up vector of the camera
 */
void Camera::positionCamera(
		Vector3f position,
		Vector3f view,
		Vector3f upVector
) {
	this->position = position;
	this->view     = view;
	this->upVector = upVector;
}


/**
 * Rotate the camera's view around the position
 */
void Camera::rotateView(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
	Vector3f newView;

	// the new direction we are facing
	Vector3f dir = view - position;

	GLfloat cosTheta = (GLfloat)cos(angle);
	GLfloat sinTheta = (GLfloat)sin(angle);

	newView.x()  = (cosTheta + (1 - cosTheta) * x * x) * dir.x();
	newView.x() += ((1 - cosTheta) * x * y - z * sinTheta) * dir.y();
	newView.x() += ((1 - cosTheta) * x * z + y * sinTheta) * dir.z();

	newView.y()  = ((1 - cosTheta) * x * y + z * sinTheta) * dir.x();
	newView.y() += (cosTheta + (1 - cosTheta) * y * y) * dir.y();
	newView.y() += ((1 - cosTheta) * y * z - x * sinTheta) * dir.z();

	newView.z()  = ((1 - cosTheta) * x * z - y * sinTheta) * dir.x();
	newView.z() += ((1 - cosTheta) * y * z + x * sinTheta) * dir.y();
	newView.z() += (cosTheta + (1 - cosTheta) * z * z) * dir.z();

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	view = position + newView;
}

/**
 * Move the camera by the mouse movements
 */
void Camera::setViewByMouse(GLint mouseX, GLint mouseY) {
	GLint middleX = this->screenWidth  >> 1;
	GLint middleY = this->screenHeight >> 1;

	GLfloat angleY = 0.0f;							// This is the direction for looking up or down
	GLfloat angleZ = 0.0f;							// This will be the value we need to rotate around the Y axis (Left and Right)
	static GLfloat currentRotX = 0.0f;

	// nothing has changed, so don't update
	if ((mouseX == middleX) && (mouseY == middleY)) {
		return;
	}

	// Get the direction the mouse moved in, but bring the number down to a reasonable amount
	angleY = (GLfloat)( (middleX - mouseX) ) / 2000.0f;
	angleZ = (GLfloat)( (middleY - mouseY) ) / 2000.0f;

	// Here we keep track of the current rotation (for up and down) so that
	// we can restrict the camera from doing a full 360 loop.
	currentRotX -= angleZ;

	if(currentRotX > M_PI) {
		// If the current rotation (in radians) is greater than 1.0, we want to cap it.
		currentRotX = M_PI;
	} else if(currentRotX < -M_PI)
		// Check if the rotation is below -1.0, if so we want to make sure it doesn't continue
		currentRotX = -M_PI;
	else {
		// Otherwise, we can rotate the view around our position
		// To find the axis we need to rotate around for up and down
		// movements, we need to get a perpendicular vector from the
		// camera's view vector and up vector.  This will be the axis.
		Vector3f vAxis = (view - position).cross(upVector);
		vAxis.normalize();

		// Rotate around our perpendicular axis and along the y-axis
		rotateView(angleZ, vAxis.x(), vAxis.y(), vAxis.z());
		rotateView(angleY, 0, 1, 0);
	}

	// Set the mouse position to the middle of our window
	SDL_WarpMouse(middleX, middleY);
}

/**
 * Strafe camera left or right
 */
void Camera::strafeCamera(GLfloat speed) {
	// Add the strafe vector to our position
	position.x() += strafe.x() * speed;
	position.y() += strafe.y() * speed;
	position.z() += strafe.z() * speed;

	// Add the strafe vector to our view
	view.x() += strafe.x() * speed;
	view.y() += strafe.y() * speed;
	view.z() += strafe.z() * speed;
}

/**
 * Move camera
 */
void Camera::moveCamera(GLfloat speed) {
	// get the current direction we are looking
	Vector3f vector = view - position;

	// normalize to not move faster than the strafe
	vector.normalize();

	// update position
	position.x() += vector.x() * speed;
	position.y() += vector.y() * speed;
	position.z() += vector.z() * speed;

	// update view
	view.x() += vector.x() * speed;
	view.y() += vector.y() * speed;
	view.z() += vector.z() * speed;
}

/**
 * This updates the camera's view and other data (Should be called each frame)
 */
void Camera::update(GLint mouseX, GLint mouseY) {
	Vector3f direction = view - position;
	strafe = direction.cross(upVector);
	strafe.normalize();

	// Move the camera's view by the mouse
	setViewByMouse(mouseX, mouseY);
}

/**
 * Update the view
 *
 * Tell opengl where to look
 */
void Camera::look() {
	gluLookAt(
		position.x(),
		position.y(),
		position.z(),
		view.x(),
		view.y(),
		view.z(),
		upVector.x(),
		upVector.y(),
		upVector.z()
	);
}

/**
 * Pass the resize
 */
void Camera::windowResize(GLint screenWidth, GLint screenHeight) {
	this->screenWidth  = screenWidth;
	this->screenHeight = screenHeight;
}
