
#include "Game.hpp"
#include "Board.hpp"

#include "py_collada/Parser.hpp"

using namespace PyCollada;

Game::Game(string modelFile, Engine &engine) :
	board(new Board()),
	engine(engine)
{
	lighting();
	events();
	recv_map receivers;
	receivers["board"] = board;
	
	Parser::parse(modelFile, receivers, "import.py");

	ball = new Ball(0, 0, 2);
}

void Game::renderFunc(GLuint time, Engine engine) {
	engine.doPhysics(*board);
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

void Game::events() {
	EventHandler &ev = engine.getEventHandler();
	ev.registerKey(SDLK_UP, boost::bind(&Board::tiltUpwards, board, _1));
	ev.registerKey(SDLK_DOWN, boost::bind(&Board::tiltDownwards, board, _1));
	ev.registerKey(SDLK_LEFT, boost::bind(&Board::tiltLeftwards, board, _1));
	ev.registerKey(SDLK_RIGHT, boost::bind(&Board::tiltRightwards, board, _1));

	ev.registerKeyRelease(SDLK_UP, boost::bind(&Board::tiltUpwardsRelease, board, _1));
	ev.registerKeyRelease(SDLK_DOWN, boost::bind(&Board::tiltDownwardsRelease, board, _1));
	ev.registerKeyRelease(SDLK_LEFT, boost::bind(&Board::tiltLeftwardsRelease, board, _1));
	ev.registerKeyRelease(SDLK_RIGHT, boost::bind(&Board::tiltRightwardsRelease, board, _1));
}
