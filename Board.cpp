#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#undef Success

#include <Eigen/Geometry>

#include "Board.hpp"

#include "engine/Shader.hpp"

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

using namespace std;
using namespace Eigen;

Board::Board() :
	tiltX(0),
	tiltY(0),
	vertexBuffers(new GLuint),
	normalBuffers(new GLuint),
	textureBuffers(new GLuint)
{
	// Create and compile our GLSL program from the shaders
	programID = Shader::loadShaders(
			"board.vert",
			"board.frag"
	);

	s_normal        = glGetAttribLocation(programID, "vertex_normal");
	s_texel         = glGetAttribLocation(programID,  "texel");

	s_transformMtrx = glGetUniformLocation(programID, "transformation");
	s_tex0Sampler   = glGetUniformLocation(programID, "tex0");
	s_tilt          = glGetUniformLocation(programID, "tilt");
}

Board::~Board() {
	delete vertexData;

	glDeleteBuffers(1, vertexBuffers);
	glDeleteBuffers(1, normalBuffers);

	delete vertexBuffers;
	delete normalBuffers;
	delete transformationMatrix;
}

void Board::draw(GLuint time) {
	glUseProgram(programID);

	glUniform2f(s_tilt, tiltX, tiltY);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, specularColor);
	glMateriali(GL_FRONT, GL_SHININESS, 10);

	glUniformMatrix4fv(s_transformMtrx, 1, GL_TRUE, transformationMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(s_tex0Sampler, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);

	glVertexAttribPointer(
			0,
			vertexSpan,
			GL_FLOAT,
			GL_FALSE,
			0,
			BUFFER_OFFSET(0)
	);

	glEnableVertexAttribArray(s_normal);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffers[0]);

	glVertexAttribPointer(
			s_normal,
			normalSpan,
			GL_FLOAT,
			GL_FALSE,
			0,
			BUFFER_OFFSET(0)
	);

	glEnableVertexAttribArray(s_texel);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffers[0]);
	glVertexAttribPointer(
			s_texel,
			texelSpan,
			GL_FLOAT,
			GL_TRUE,
			0,
			BUFFER_OFFSET(0)
	);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(s_normal);
	glDisableVertexAttribArray(s_texel);
}

void Board::doPhysics(GLuint time) {
	if (time - lastUpdate >= TILT_INTERVAL) {
		if (doTiltUpwards) {
			if (tiltX + TILT_FACTOR < TILT_MAX) {
				tiltX += TILT_FACTOR;
			}
		}

		if (doTiltDownwards) {
			if (tiltX - TILT_FACTOR > -TILT_MAX) {
				tiltX -= TILT_FACTOR;
			}
		}

		if (this->doTiltLeftwards) {
			if (tiltY - TILT_FACTOR > -TILT_MAX) {
				tiltY -= TILT_FACTOR;
			}
		}

		if (this->doTiltRightwards) {
			if (tiltY + TILT_FACTOR < TILT_MAX) {
				tiltY += TILT_FACTOR;
			}
		}

		lastUpdate = time;
	}
}

/**
 * Register geometry data
 */
void Board::setVertices(
		GLfloat* data,
		GLuint   vertexCount,
		GLuint   span
) {
	this->vertexData      = data;
	this->vertexCount     = vertexCount;
	this->vertexSpan      = span;

	glGenBuffers(1, vertexBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[0]);
	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * vertexCount * vertexSpan,
			vertexData,
			GL_STATIC_DRAW
	);
}

void Board::setNormals(
		GLfloat* data,
		GLuint   dataCount,
		GLuint   span
) {

	this->normalData  = data;
	this->normalCount = dataCount;
	this->normalSpan  = span;

	glGenBuffers(1, normalBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffers[0]);
	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * normalCount * normalSpan,
			normalData,
			GL_STATIC_DRAW
	);
}

void Board::setTexels(
		GLfloat* data,
		GLuint   dataCount,
		GLuint   span
) {
	this->texelData = data;
	this->texelCount  = dataCount;
	this->texelSpan   = span;

	glGenBuffers(1, textureBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, textureBuffers[0]);
	glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * texelCount * texelSpan,
			texelData,
			GL_STATIC_DRAW
	);

	// it's in openGL memory now, we don't need this anymore
	delete this->texelData;
}

void Board::setTexture(
		string path
) {
	GLuint storage;

	SDL_Surface *surface;
	SDL_RWops *rwop = SDL_RWFromFile(path.c_str(), "rb");

	if (IMG_isBMP(rwop)) {
		surface = IMG_LoadBMP_RW(rwop);
	}
	else if (IMG_isJPG(rwop)) {
		surface = IMG_LoadJPG_RW(rwop);
	}

	if (!surface) {
		throw "Could not load texture " + path;
	}

	glGenTextures(1, &storage);
	glBindTexture(GL_TEXTURE_2D, storage);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		surface->w,
		surface->h,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		surface->pixels
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	delete surface;

	delete rwop;

	textures.push_back(storage);
}

void Board::setTransformationMatrix(GLfloat *data) {
	this->transformationMatrix = data;
}

void Board::setAmbientColor(float *color) {
	this->ambientColor = color;
}

void Board::setSpecularColor(float *color) {
	this->specularColor = color;
}

void Board::tiltUpwards(SDL_Event &ev) {
	doTiltUpwards = true;
}

void Board::tiltDownwards(SDL_Event &ev) {
	doTiltDownwards = true;
}

void Board::tiltLeftwards(SDL_Event &ev) {
	doTiltLeftwards = true;
}

void Board::tiltRightwards(SDL_Event &ev) {
	doTiltRightwards = true;
}

void Board::tiltUpwardsRelease(SDL_Event &ev) {
	doTiltUpwards = false;
}

void Board::tiltDownwardsRelease(SDL_Event &ev) {
	doTiltDownwards = false;
}

void Board::tiltLeftwardsRelease(SDL_Event &ev) {
	doTiltLeftwards = false;
}

void Board::tiltRightwardsRelease(SDL_Event &ev) {
	doTiltRightwards = false;
}

void Board::dumpData() {
	cout << "#############################" << endl;

	for (unsigned int i = 0; i < vertexCount; i++) {
		int pos = i * 3;
		cout << vertexData[pos] << ", " << vertexData[pos + 1]
			<< ", " << vertexData[pos + 2] << endl;
		cout << normalData[pos] << ", " << normalData[pos + 1]
			<< ", " << normalData[i + 2] << endl;

		int texPos = i * 2;
		cout << texelData[texPos] << ", " << texelData[texPos + 1] << endl;
		cout << endl;
	}

	cout << "-----------------------------" << endl;
}
