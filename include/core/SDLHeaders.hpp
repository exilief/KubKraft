/*
 * =====================================================================================
 *
 *       Filename:  SDLHeaders.hpp
 *
 *    Description:
 *
 *        Created:  20/12/2014 01:18:31
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SDLHEADERS_HPP_
#define SDLHEADERS_HPP_

///NOTE: Using generic names 'S_...' for classes existing in SDL and SFML. Replace with better names?

#ifdef USE_SDL

	#ifdef __ANDROID__
		#include "SDL.h"
		#include "SDL_image.h"
		#include "SDL_mixer.h"
	#else
		#include <SDL2/SDL.h>
		#include <SDL2/SDL_image.h>
		#include <SDL2/SDL_mixer.h>
	#endif

	using S_Window = SDL_Window;
	using S_Event = SDL_Event;
	using S_KeyCode = SDL_Keycode;  //= u32;

//	using S_EventType = SDL_EventType;
	namespace S_EventType {
		enum Type {
			MouseMoved  		= SDL_EventType::SDL_MOUSEMOTION,
			MouseButtonPressed	= SDL_MOUSEBUTTONDOWN,
			MouseButtonReleased	= SDL_MOUSEBUTTONUP,
			MouseWheelScrolled	= SDL_MOUSEWHEEL,
			KeyPressed  		= SDL_KEYDOWN,
			KeyReleased 		= SDL_KEYUP,
		};
	}
	inline const SDL_MouseButtonEvent& S_Event_getMouseButton(const S_Event &e) { return e.button; }  //Replace!
	inline const SDL_MouseMotionEvent& S_Event_getMouseMove(const S_Event &e) { return e.motion; }
	inline int S_Event_getMouseScrollY(const S_Event &e) { return e.wheel.y; }
	inline S_KeyCode S_Event_getKeyCode(const S_Event &e) { return e.key.keysym.sym; }

	namespace S_MouseButton {
		enum Button {
			Left =	 SDL_BUTTON_LEFT,
			Right =	 SDL_BUTTON_RIGHT,
			Middle = SDL_BUTTON_MIDDLE
		};
	}

#elif defined USE_SFML

	#include <SFML/Graphics.hpp>

	using S_Window = sf::RenderWindow;
	using S_Event = sf::Event;
	using S_KeyCode = sf::Keyboard::Key;

//	using S_EventType = sf::Event::EventType;
	namespace S_EventType {
		enum Type {
			MouseMoved  		= sf::Event::EventType::MouseMoved,
			MouseButtonPressed	= sf::Event::MouseButtonPressed,
			MouseButtonReleased	= sf::Event::MouseButtonReleased,
			MouseWheelScrolled	= sf::Event::MouseWheelScrolled,
			KeyPressed  		= sf::Event::KeyPressed,
			KeyReleased 		= sf::Event::KeyReleased,
		};
	}
	inline const sf::Event::MouseButtonEvent& S_Event_getMouseButton(const S_Event &e) { return e.mouseButton; }  //Replace!
	inline const sf::Event::MouseMoveEvent& S_Event_getMouseMove(const S_Event &e) { return e.mouseMove; }
	inline float S_Event_getMouseScrollY(const S_Event &e) { return e.mouseWheelScroll.delta; }
	inline S_KeyCode S_Event_getKeyCode(const S_Event &e) { return e.key.code; }

	namespace S_MouseButton {
		enum Button {
			Left =	 sf::Mouse::Button::Left,
			Right =	 sf::Mouse::Right,
			Middle = sf::Mouse::Middle
		};
	}

#endif // USE_SDL, USE_SFML

#endif // SDLHEADERS_HPP_
