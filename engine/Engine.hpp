#ifndef __ENGINE_HPP_
#define __ENGINE_HPP_

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <string>


#include "EventHandler.hpp"
#include "CollisionHandler.hpp"
#include "Camera.hpp"
#include "DrawObject.hpp"


using namespace std;



class SDLException : public std::exception {

};

class Engine;

typedef boost::function<void (GLuint, Engine)> render_func;

class Engine {
	public:
		//static Engine& inst();
		Engine(string name, GLuint scrW, GLuint scrH);

		EventHandler& getEventHandler();
		CollisionHandler& getCollisionHandler();

		GLboolean isRunning();
		void registerRenderFunc(render_func);
		void run();

		void init(GLuint scrW, GLuint srcH);
		Camera cam;

		void toggleDebugCamera();

		void drawObject(DrawObject &obj);
		void doPhysics(DrawObject &obj);

	private:

		//static Engine *instance;

		GLboolean running;
		GLuint scrW      = 1280;
		GLuint scrH      = 1024;
		GLuint clrDepth  = 24;

		GLuint currentTime = 0;

		EventHandler evHandler;
		CollisionHandler collisionHandler;

		void initSDL(string name);
		void initOpenGL();
		void initEvents();
		void lighting();

		void render();

		// events
		void videoResize(SDL_Event &event);
		void mouseMotion(SDL_Event &event);
		void quit(SDL_Event &event);
		void cameraForwardMovement(SDL_Event &event);
		void cameraBackwardMovement(SDL_Event &event);
		void cameraLeftMovement(SDL_Event &event);
		void cameraRightMovement(SDL_Event &event);

		SDL_Surface *screen;

		void handleErrors();

		bool debugCameraActive       = false;
		GLfloat cameraSpeed          = 0.5;

		bool debugBoundaries         = false;
		void toggleBoundaries(SDL_Event&);

		void renderOrigin();

		render_func renderFunc;

		GLuint frameCounter = 0;
		GLuint lastDraw;
		GLuint timer;
		void fps(GLuint time);

		//Engine(Engine const& copy);            // Not Implemented
		//Engine& operator=(Engine const& copy); // Not Implemented
};

#endif
