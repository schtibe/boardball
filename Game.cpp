
#include "Game.hpp"
#include "Board.hpp"

#include "py_collada/Parser.hpp"

using namespace PyCollada;

Game::Game(string modelFile) :
	board(new Board())
{
	lighting();
	recv_map receivers;
	receivers["board"] = board;
	
	Parser::parse(modelFile, receivers, "import.py");

	ball = new Ball(0, 0, 2);
}

void Game::renderFunc(GLuint time, Engine engine) {
	engine.drawObject(*board);
	engine.drawObject(*ball);
}

void Game::lighting() {
	GLfloat ambientLight[]  = {1.5f, 0.5f, 0.5f, 1.0f};
	GLfloat diffuseLight[]  = {0.5f, 0.5f, 0.5f, 0.0f};
	GLfloat specularLight[] = {0.2f, 0.2f, 0.2f, 1.0f};

	GLfloat lightPosition[] = {0.0f, 0.0f, 10.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}
