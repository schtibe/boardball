#ifndef __DRAW_OBJECT_HPP

#define __DRAW_OBJECT_HPP

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>

class DrawObject {
	private:
		virtual void draw(GLuint time) = 0;
		virtual void doPhysics(GLuint time) = 0;

	friend class Engine;
};


#endif
