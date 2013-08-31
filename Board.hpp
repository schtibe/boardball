#ifndef __BOARD_HPP_
#define __BOARD_HPP_ 

#include <SDL/SDL.h>
#include <boost/container/vector.hpp>

#include "engine/DrawObject.hpp"
#include "py_collada/Receiver.hpp"

using namespace PyCollada;

class Board : public DrawObject, public Receiver {
	public:
		Board();
		~Board();

		void setVertices(
			GLfloat* data,
			GLuint   vertexCount,
			GLuint   span
		);

		void setNormals(
			GLfloat* data,
			GLuint   dataCount,
			GLuint   span
		);

		void setTexels(
			GLfloat* data,
			GLuint   dataCount,
			GLuint   span
		);

		void setTexture(
			string path
		);

		void setAmbientColor(float *color);
		void setSpecularColor(float *color);

		void setTransformationMatrix(GLfloat *data);

		void dumpData();

	private:
		void draw();
		void doPhysics(GLuint time);

		/**
		 * Vertex data
		 */
		unsigned int vertexCount;
		unsigned int vertexSpan;
		GLfloat *vertexData;

		/**
		 * Normal data
		 */
		unsigned int normalCount;
		unsigned int normalSpan;
		GLfloat *normalData;

		/**
		 * Texture data
		 */
		unsigned int texelCount;
		unsigned int texelSpan;
		GLfloat *texelData;

		/**
		 * Color data
		 */
		float *specularColor;
		float *ambientColor;

		/**
		 * Buffers
		 */
		GLuint *vertexBuffers;
		GLuint *normalBuffers;
		GLuint *textureBuffers;
		boost::container::vector<GLuint> textures;

		GLuint programID;


		GLuint normalID;
		GLuint texelID;
		GLuint transformMtrx;
		GLuint tex0Sampler;


		GLfloat *transformationMatrix;

};

#endif

