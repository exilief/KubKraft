/*
 * =====================================================================================
 *
 *       Filename:  CoreApplication.cpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 02:00:18
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <ctime>

#include "CoreApplication.hpp"
#include "Exception.hpp"
#include "GamePad.hpp"
#include "Mouse.hpp"

CoreApplication::CoreApplication(int, char **) {
	// if (argc > 1 && argv[1] == std::string("--no-sound"))
	// 	AudioPlayer::setMuteState(true);
}

void CoreApplication::init() {
	std::srand(std::time(nullptr));

	Mouse::setWindow(&m_window);

	GamePad::init(m_keyboardHandler);

	ApplicationStateStack::setInstance(m_stateStack);
	ResourceHandler::setInstance(m_resourceHandler);
}

int CoreApplication::run() {
	// try {
		m_sdlLoader.load();

		init();
		mainLoop();
	// }
	// catch(const Exception &e) {
	// 	std::cerr << "Fatal error " << e.what() << std::endl;
	// 	return 1;
	// }
	// catch(const std::exception &e) {
	// 	std::cerr << "Exception caught: " << e.what() << std::endl;
	// 	return 1;
	// }
	// catch(...) {
	// 	std::cerr << "Fatal error: Unknown error." << std::endl;
	// 	return 1;
	// }

	return 0;
}

void CoreApplication::createWindow(u16 screenWidth, u16 screenHeight, const char *windowTitle) {
	m_window.open(windowTitle, screenWidth, screenHeight);
}

void CoreApplication::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
 		if (event.type == SDL_QUIT) {
			m_window.close();
		}

		if (!m_stateStack.empty())
			m_stateStack.top().onEvent(event);
	}
}

void CoreApplication::mainLoop() {
	while(m_window.isOpen() && m_stateStack.size()) {
		handleEvents();

		m_clock.updateGame([&] {
			if (!m_stateStack.empty())
				m_stateStack.top().update();

			m_stateStack.clearDeletedStates();
		});

		m_clock.drawGame([&] {
			m_window.clear();

			if(!m_stateStack.empty())
				m_window.draw(m_stateStack.top());

			m_window.display();
		});
	}
}

