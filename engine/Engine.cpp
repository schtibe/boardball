#include "Engine.hpp"

#include <iostream>
#include <boost/bind.hpp>
#include <GL/glu.h>
#include <SDL/SDL.h>


/*
Engine& Engine::inst() {
	static Engine instance;
	return instance;
}
*/



Engine::Engine(string name, GLuint scrW, GLuint scrH) :
	cam(Camera(scrW, scrH)),
	running(false),
	scrW(scrW),
	scrH(scrH),
	evHandler(EventHandler()),
	collisionHandler(CollisionHandler())
{
	initSDL(name);
	initOpenGL();
	glewInit();
	initEvents();
}

EventHandler& Engine::getEventHandler() {
	return evHandler;
}

CollisionHandler& Engine::getCollisionHandler() {
	return collisionHandler;
}

GLboolean Engine::isRunning() {
	return running;
}

void Engine::registerRenderFunc(render_func renderFunc) {
	this->renderFunc = renderFunc;
}

void Engine::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	currentTime = SDL_GetTicks();

	cam.look();

	renderOrigin();

	evHandler.handle();
	renderFunc(currentTime, *this);

	fps(currentTime);

	SDL_GL_SwapBuffers();

}

void Engine::drawObject(DrawObject &obj) {
	if (!debugBoundaries) {
		obj.draw();
	}

}

void Engine::fps(GLuint time) {
	frameCounter++;

	timer += time - lastDraw;

	if (timer > 1000) {
		cout << frameCounter << " frames per second" << endl;
		timer = 0;
		frameCounter = 0;
	}
	lastDraw = time;
}

void Engine::doPhysics(DrawObject &obj) {
	obj.doPhysics(currentTime);
}

void Engine::run() {
	running = true;

	while (running) {
		render();
	}

	// cleanup
	SDL_Quit();
}

/**
 * Initialize SDL
 *
 * @TODO Probably make exceptions instead of return values
 */
void Engine::initSDL(string name) {
	SDL_WM_SetCaption(name.c_str(), name.c_str());

	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		//cout << "Can't init SDL: " << SDL_GetError() << endl;
		//return -1;
	}
	atexit(SDL_Quit);
	
	screen = SDL_SetVideoMode(scrW, scrH, clrDepth, SDL_OPENGL | SDL_RESIZABLE);

	if (screen == NULL) {
		//cout << "Can't set video mode: " << SDL_GetError() << endl;
		//return -1;
	}

	SDL_EnableUNICODE(true);

	//return 1;
}


/**
 * Initialize OpenGL
 */
void Engine::initOpenGL() {
	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
	cout << "Version: " << glGetString(GL_VERSION) << endl;
	//cout << "Extensions: " << glGetString(GL_EXTENSIONS) << endl;

	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glViewport(0,0,scrW, scrH);
	glMatrixMode(GL_PROJECTION);

	gluPerspective(45.0f,(GLfloat)scrW/(GLfloat)scrH, 1.0f, 150.0f);
	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);


	cam.positionCamera(0, 0, 20,   0, 0, 0,   0, 1, 0);
}



void Engine::toggleBoundaries(SDL_Event &event) {
	debugBoundaries = !debugBoundaries;

	getCollisionHandler().toggleBoundaries();
}

void Engine::initEvents() {
	evHandler.registerVideoResizeCallback (boost::bind(&Engine::videoResize, this, _1));
	evHandler.registerQuitCallback        (boost::bind(&Engine::quit, this, _1));

	evHandler.registerKey(SDLK_ESCAPE, boost::bind(&Engine::quit, this, _1));
	evHandler.registerKeyOnce(SDLK_b  , boost::bind(&Engine::toggleBoundaries, this, _1));
}

void Engine::toggleDebugCamera() {
	
	if (debugCameraActive) {
		evHandler.unregisterKey(SDLK_w);
		evHandler.unregisterKey(SDLK_a);
		evHandler.unregisterKey(SDLK_s);
		evHandler.unregisterKey(SDLK_d);

		evHandler.unregisterMouseMotion();
	} 
	else {
		evHandler.registerMouseMotionCallback (
				boost::bind(&Engine::mouseMotion, this, _1)
		);

		evHandler.registerKey(
				SDLK_w,
				boost::bind(&Engine::cameraForwardMovement, this, _1)
		);

		evHandler.registerKey(
				SDLK_s,
				boost::bind(&Engine::cameraBackwardMovement, this, _1)
		);

		evHandler.registerKey(
				SDLK_a,
				boost::bind(&Engine::cameraLeftMovement, this, _1)
		);

		evHandler.registerKey(
				SDLK_d,
				boost::bind(&Engine::cameraRightMovement, this, _1)
		);
	}

	debugCameraActive = !debugCameraActive;
}

void Engine::cameraForwardMovement(SDL_Event &event) {
	cam.moveCamera(cameraSpeed);
}

void Engine::cameraBackwardMovement(SDL_Event &event) {
	cam.moveCamera(-cameraSpeed);
}

void Engine::cameraLeftMovement(SDL_Event &event) {
	cam.strafeCamera(-cameraSpeed);
}

void Engine::cameraRightMovement(SDL_Event &event) {
	cam.strafeCamera(cameraSpeed);
}

/**
 * Resize event
 */
void Engine::videoResize(SDL_Event &event) {
	screen = SDL_SetVideoMode(
			event.resize.w,
			event.resize.h,
			clrDepth,
			SDL_OPENGL | SDL_RESIZABLE
	);
	scrH = event.resize.h;
	scrW = event.resize.w;

	cam.windowResize(scrH, scrW);
}

/**
 * Mouse motion event
 */
void Engine::mouseMotion(SDL_Event &event) {
	Sint16 mouseX = event.motion.x;
	Sint16 mouseY = event.motion.y;
	cam.update(mouseX, mouseY);
}

void Engine::quit(SDL_Event &event) {
	running = false;
}

void Engine::handleErrors() {
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		cerr << "OpenGL Error: " << errString << endl;
	}
}

void Engine::renderOrigin() {
	/*
	glPushMatrix();
	glLineWidth(4);

	GLfloat red[]   = {1.0, 0.0, 0.0, 1.0};
	GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat blue[]  = {0.0, 0.0, 1.0, 1.0};

	glMaterialfv(GL_FRONT, GL_DIFFUSE,  red);
	glMaterialfv(GL_FRONT, GL_AMBIENT,  red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, red);

	glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();

	glMaterialfv(GL_FRONT, GL_DIFFUSE,  green);
	glMaterialfv(GL_FRONT, GL_AMBIENT,  green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, green);

	glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();

	glMaterialfv(GL_FRONT, GL_DIFFUSE,  blue);
	glMaterialfv(GL_FRONT, GL_AMBIENT,  blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, blue);

	glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();

	glPopMatrix();
	*/
}
