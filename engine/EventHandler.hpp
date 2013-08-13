#ifndef __EVENT_HANDLER_H
#define __EVENT_HANDLER_H


#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <SDL/SDL.h>
#include <boost/function.hpp>

typedef boost::function<void (SDL_Event&)> callback;

class EventHandler {
	public:
		EventHandler();

		void handle();

		void registerVideoResizeCallback(callback);
		void registerMouseMotionCallback(callback);
		void registerQuitCallback(callback);


		void registerKey(SDLKey key        , callback);
		void registerKeyOnce(SDLKey key    , callback);
		void registerKeyRelease(SDLKey key , callback);

		void unregisterKey(SDLKey key);

		void unregisterMouseMotion();


	private:
		static const GLuint sdlKeyAmount = 323;

		SDL_Event event;

		void actualHandle(SDL_Event &);
		void handleKeys(SDL_Event &event);
		void cycle();
		void keyDown(SDLKey &);
		void keyUp(SDLKey &);

		bool keysHeld[sdlKeyAmount];

		callback videoResize;
		callback mouseMotion;
		callback quit;

		callback keyFunc[sdlKeyAmount];
		callback keyReleaseFunc[sdlKeyAmount];
		callback keyFuncOnce[sdlKeyAmount];


};



#endif
