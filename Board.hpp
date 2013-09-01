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

		void tiltUpwards(SDL_Event &ev);
		void tiltDownwards(SDL_Event &ev);
		void tiltLeftwards(SDL_Event &ev);
		void tiltRightwards(SDL_Event &ev);
		void tiltUpwardsRelease(SDL_Event &ev);
		void tiltDownwardsRelease(SDL_Event &ev);
		void tiltLeftwardsRelease(SDL_Event &ev);
		void tiltRightwardsRelease(SDL_Event &ev);

		void dumpData();

	private:
		void draw(GLuint time);
		void doPhysics(GLuint time);

		/**
		 * Physics
		 */
		GLuint lastUpdate = 0;
		const GLfloat TILT_INTERVAL = 0.1;
		const GLfloat TILT_FACTOR = 0.01;
		const GLfloat TILT_MAX = 10.0 / 180 * M_PI;
		GLfloat tiltX;
		GLfloat tiltY;
		bool doTiltDownwards = false;
		bool doTiltUpwards = false;
		bool doTiltLeftwards = false;
		bool doTiltRightwards = false;

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


		GLuint s_normal;
		GLuint s_texel;
		GLuint s_transformMtrx;
		GLuint s_tex0Sampler;
		GLuint s_tilt;


		GLfloat *transformationMatrix;

};

#endif

