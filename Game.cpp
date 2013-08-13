
#include "Game.hpp"
#include "Board.hpp"

#include "py_collada/Parser.hpp"

using namespace PyCollada;

Game::Game(string modelFile) :
	board(new Board()),
	secondBoard(new Board())
{
	recv_map receivers;
	receivers["board"] = board;
	
	Parser::parse(modelFile, receivers);


	/*
	DataPreprocessor boardPreprocessor(*parser, string("board"), *board);
	DataPreprocessor secondBoardPreprocessor(*parser, string("zweit"), *secondBoard);

	try {
		parser->parseFile(modelFile);
	} catch (ParseException& e) {
		cout << "Caught exception" << endl;
		cout << e.what() << endl;

		throw e; // make the game end
	}
	try {
		boardPreprocessor.finish();
		secondBoardPreprocessor.finish();
	} catch (PreprocessException &e) {
		cout << "Caught preprocess exception" <<endl;
		cout << e.what() << endl;

		throw e;
	}
		*/

	//board->dumpData();
}

void Game::renderFunc(GLuint time, Engine engine) {
	engine.drawObject(*board);
	//engine.drawObject(*secondBoard);
}
