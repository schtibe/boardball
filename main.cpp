
#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <boost/bind.hpp>

#include "Game.hpp"
#include "engine/Engine.hpp"



int main(int argc, char**argv) {
	string model;

	if (argc < 2) {
		model = "board.dae";
	}
	else {
		model = argv[1];
	}

	cout << model << endl;
	
	Engine engine("colladaparser", 1024, 768);

	Game *game = new Game(model);

	
	engine.registerRenderFunc(
			boost::bind(&Game::renderFunc, game, _1, _2)
	);


	engine.toggleDebugCamera();
	engine.run();
}
