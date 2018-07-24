/*
 * =====================================================================================
 *
 *       Filename:  GameState.cpp
 *
 *    Description:
 *
 *        Created:  15/12/2014 03:51:55
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGL.hpp"

#include "ApplicationStateStack.hpp"
#include "Config.hpp"
#include "GameClock.hpp"
#include "GameState.hpp"
#include "InventoryState.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "PauseMenuState.hpp"
#include "PlayerInventoryWidget.hpp"

GameState::GameState() {
	m_player.inventory().addStack(ItemType::Workbench, 1);
	m_player.inventory().addStack(ItemType::Dirt, 64);
	m_player.inventory().addStack(ItemType::Grass, 64);
	m_player.inventory().addStack(ItemType::Stone, 64);
	m_player.inventory().addStack(ItemType::Glass, 64);
	m_player.inventory().addStack(ItemType::Glowstone, 64);
	m_player.inventory().addStack(ItemType::Furnace, 1);
	m_player.inventory().addStack(ItemType::PlankSlab, 64);
	m_player.inventory().addStack(ItemType::StonePickaxe, 1);

	m_player.inventory().addStack(ItemType::Wood, 64);
	m_player.inventory().addStack(ItemType::Planks, 64);
	m_player.inventory().addStack(ItemType::Stick, 64);
	m_player.inventory().addStack(ItemType::Cobblestone, 64);
	m_player.inventory().addStack(ItemType::StoneAxe, 1);
	m_player.inventory().addStack(ItemType::StoneHoe, 1);
	m_player.inventory().addStack(ItemType::StoneShovel, 1);
	m_player.inventory().addStack(ItemType::StoneSword, 1);
	m_player.inventory().addStack(ItemType::IronOre, 64);
	m_player.inventory().addStack(ItemType::Coal, 64);

	m_projectionMatrix = glm::perspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, DIST_NEAR, DIST_FAR);

	initShaders();
}

void GameState::onEvent(const S_Event &event) {
#ifdef USE_SFML
	static sf::Vector2i lastPosition;
#endif // USE_SFML

	if (event.type == S_EventType::MouseMoved) {
		if(SCREEN_WIDTH / 2 != S_Event_getMouseMove(event).x || SCREEN_HEIGHT / 2 != S_Event_getMouseMove(event).y) {
#ifdef USE_SDL
			m_camera.turnH(event.motion.xrel * 0.06);
			m_camera.turnV(-event.motion.yrel * 0.06);
#elif defined USE_SFML
			m_camera.turnH((event.mouseMove.x-lastPosition.x) * 0.06);
			m_camera.turnV(-(event.mouseMove.y-lastPosition.y) * 0.06);
#endif // USE_SDL, USE_SFML

			Mouse::resetToWindowCenter();
		#ifdef USE_SFML
			const Vector2i pos0 = Mouse::getPosition();
			lastPosition = {pos0.x, pos0.y};
		#endif // USE_SFML

			m_camera.updateViewMatrix();
		}
	}
	else if (event.type == S_EventType::KeyPressed && S_Event_getKeyCode(event) == Keyboard::getKey(Keyboard::Escape) && &m_stateStack->top() == this) {
		m_stateStack->push<PauseMenuState>(this);
	}
#ifdef USE_SDL
	else if (event.type == SDL_WINDOWEVENT) {
		if (event.window.event == SDL_WINDOWEVENT_LEAVE) {
#elif defined USE_SFML
		if (event.type == sf::Event::LostFocus) {
#endif // USE_SDL, USE_SFML
			Mouse::setCursorGrabbed(false);
			Mouse::setCursorVisible(true);
		}
#ifdef USE_SDL
		else if (event.window.event == SDL_WINDOWEVENT_ENTER) {
#elif defined USE_SFML
		else if (event.type == sf::Event::GainedFocus) {
#endif // USE_SDL, USE_SFML
			Mouse::setCursorGrabbed(true);
			Mouse::setCursorVisible(false);
		}
#ifdef USE_SDL
	}
#endif // USE_SDL

	m_hud.onEvent(event);
}

void GameState::update() {
	Shader::bind(&m_shader);
	m_shader.setUniform("u_time", GameClock::getTicks());
	Shader::bind(nullptr);

	m_world.update(m_player);

	if (&m_stateStack->top() == this) {
		m_camera.processInputs();

		if (Keyboard::isKeyPressedOnce(Keyboard::E)) {
			auto &inventoryState = m_stateStack->push<InventoryState>(this);
			inventoryState.setupWidget<PlayerInventoryWidget>(m_player.inventory());
		}
	}

	m_viewMatrix = m_camera.updatePosition(m_world);

	m_hud.update();
}

void GameState::initShaders() {
	m_shader.createProgram();

	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/game.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/color.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/light.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/fog.f.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/game.f.glsl");

	m_shader.linkProgram();
}

void GameState::draw(RenderTarget &target, RenderStates states) const {
	states.shader = &m_shader;
	states.projectionMatrix = &m_projectionMatrix;
	states.viewMatrix = &m_viewMatrix;

	target.draw(m_world, states);
	target.draw(m_hud, states);
}

