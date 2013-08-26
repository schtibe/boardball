
#include "Game.hpp"
#include "Board.hpp"

#include "py_collada/Parser.hpp"

using namespace PyCollada;

Game::Game(string modelFile) :
	board(new Board())
{
	recv_map receivers;
	receivers["board"] = board;
	
	Parser::parse(modelFile, receivers, "import.py");

	ball = new Ball();
}

void Game::renderFunc(GLuint time, Engine engine) {
	//engine.drawObject(*board);
	engine.drawObject(*ball);
}
