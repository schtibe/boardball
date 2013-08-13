#ifndef __GAME_HPP
#define __GAME_HPP

#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "engine/Engine.hpp"
#include "Board.hpp"

class Game {
	public:
		Game(string modelFile);
		void renderFunc(GLuint, Engine);

	private:
		Board *board;
		Board *secondBoard;
};


#endif