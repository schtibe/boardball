#ifndef __PYCOLLADARECEIVER_HPP

#define __PYCOLLADARECEIVER_HPP

#include <GL/gl.h>
#include <iostream>

using std::string;

namespace PyCollada {

class Receiver {
	public:
		virtual void setVertices(
			GLfloat* data,
			GLuint   dataCount,
			GLuint   span
		) = 0;

		virtual void setNormals(
			GLfloat* data,
			GLuint   dataCount,
			GLuint   span
		) = 0;

		virtual void setTexels(
			GLfloat* data,
			GLuint   dataCount,
			GLuint  span
		) = 0;

		virtual void setTexture(
			string path
		) = 0;

		virtual void setAmbientColor(
			float *color
		) = 0;

		virtual void setSpecularColor(
			float *color
		) = 0;

		virtual void setTransformationMatrix(
			GLfloat* data
		) = 0;
};

}

#endif /* end of include guard: __PYCOLLADARECEIVER_HPP */
