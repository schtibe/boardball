#include "Engine.hpp"
#include "EventHandler.hpp"

// NOTE at the moment it is possible to
// register several functions for the same key. Consider
// restricting this

/*
 * http://www.libsdl.org/docs/html/sdlevent.html
 */

/**
 * Construct
 */
EventHandler::EventHandler() {
	std::fill_n(keysHeld, sdlKeyAmount, false);
}


/**
 * Handle the events
 */
void EventHandler::handle() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_VIDEORESIZE:
				if (videoResize) {
					videoResize(event);
				}
				break;
			case SDL_QUIT:
				quit(event);
				break;
			case SDL_KEYDOWN:
				keyDown(event.key.keysym.sym);
				keysHeld[event.key.keysym.sym] = true;
				break;
			case SDL_KEYUP:
				keyUp(event.key.keysym.sym);
				keysHeld[event.key.keysym.sym] = false;
				break;
			case SDL_MOUSEMOTION:
				if (mouseMotion) {
					mouseMotion(event);
				}
				break;
		}
	}

	cycle();
}

void EventHandler::registerMouseMotionCallback(callback func) {
	this->mouseMotion = func;
}

void EventHandler::unregisterMouseMotion() {
	this->mouseMotion = NULL;
}

/**
 * Register the function for the video resize
 */
void EventHandler::registerVideoResizeCallback(callback func) {
	videoResize = func;
}

void EventHandler::registerQuitCallback(callback func) {
	quit = func;
}

/**
 * Trigger the func on a key once
 */
void EventHandler::keyDown(SDLKey &key) {
	if (!keysHeld[key]) {
		if (keyFuncOnce[key]) {
			keyFuncOnce[key](event);
		}
	}
}

/**
 * When a key is released, call the release function
 */
void EventHandler::keyUp(SDLKey &key) {
	if (keyReleaseFunc[key]) {
		keyReleaseFunc[key](event);
	}
}

/**
 * When a key is held, call the associated function (if set)
 */
void EventHandler::cycle() {
	for (GLuint i = 0; i < sdlKeyAmount; i++) {
		if (keysHeld[i]) {
			if (keyFunc[i]) {
				keyFunc[i](event);
			}
		}
	}
}


/**
 * Register a key to the list
 */
void EventHandler::registerKey(SDLKey key, callback func) {
	keyFunc[key] = func;
}

/**
 * Register function to a key when it's released
 */
void EventHandler::registerKeyRelease(SDLKey key, callback func) {
	keyReleaseFunc[key] = func;
}

/**
 * Register a function to be spawned only once when pressing the key
 */
void EventHandler::registerKeyOnce(SDLKey key, callback func) {
	keyFuncOnce[key] = func;
}

/**
 * Unregister the function on a key
 *
 * For now unregister all the possible variations
 * of the callbacks.
 */
void EventHandler::unregisterKey(SDLKey key) {
	keyFunc[key]        = NULL;
	keyReleaseFunc[key] = NULL;
	keyFuncOnce[key]    = NULL;
}
