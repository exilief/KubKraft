/*
 * =====================================================================================
 *
 *       Filename:  SettingsMenuState.cpp
 *
 *    Description:
 *
 *        Created:  29/06/2018 06:11:57
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include <algorithm>

#include "ApplicationStateStack.hpp"
#include "Config.hpp"
#include "Mouse.hpp"
#include "SettingsMenuState.hpp"
#include "World.hpp"

SettingsMenuState::SettingsMenuState(ApplicationState *parent) : ApplicationState(parent) {
	m_shader.createProgram();
	m_shader.addShader(GL_VERTEX_SHADER, "resources/shaders/basic.v.glsl");
	m_shader.addShader(GL_FRAGMENT_SHADER, "resources/shaders/basic.f.glsl");
	m_shader.linkProgram();

	m_background.setColor(Color{0, 0, 0, 127});
	m_background.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_menuWidget.setScale(GUI_SCALE, GUI_SCALE, 1);

	addMainButtons();
}

void SettingsMenuState::onEvent(const SDL_Event &event) {
	m_menuWidget.onEvent(event);

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
		m_stateStack->pop();
	}
}

void SettingsMenuState::update() {
}

void SettingsMenuState::addMainButtons() {
	m_menuWidget.addButton(0, 0, "Graphics...", [this] (TextButton &) {
		m_menuWidget.reset(1, 8);
		addGraphicsButtons();
	});

	m_menuWidget.addButton(0, 1, "Input...", [this] (TextButton &) {
		m_menuWidget.reset(1, 8);
		addInputButtons();
	});

	m_menuWidget.addButton(0, 7, "Done", [this] (TextButton &) { m_stateStack->pop(); });
}

void SettingsMenuState::addGraphicsButtons() {
	m_menuWidget.addButton(0, 0, "Render distance: " + std::to_string(World::renderDistance), [] (TextButton &button) {
		World::renderDistance = std::max(4, (World::renderDistance + 2) % 16);
		button.setText("Render distance: " + std::to_string(World::renderDistance));
		World::isReloadRequested = true;
	});

	m_menuWidget.addButton(0, 1, std::string("Smooth lighting: ") + (Config::isSmoothLightingEnabled ? "ON" : "OFF"), [] (TextButton &button) {
		Config::isSmoothLightingEnabled = !Config::isSmoothLightingEnabled;
		button.setText(std::string("Smooth lighting: ") + (Config::isSmoothLightingEnabled ? "ON" : "OFF"));
		World::isReloadRequested = true;
	});

	m_menuWidget.addButton(0, 2, "GUI Scale: " + std::to_string(GUI_SCALE), [] (TextButton &button) {
		GUI_SCALE = 1 + (GUI_SCALE + 1) % 3;
		button.setText("GUI Scale: " + std::to_string(GUI_SCALE));
		// FIXME: Fix decrease bug
		//        Reload menus with new scaling
	});

	m_menuWidget.addButton(0, 3, "Fullscreen: OFF", [] (TextButton &) {}).setEnabled(false);
	m_menuWidget.addButton(0, 4, "Use VSync: OFF", [] (TextButton &) {}).setEnabled(false);

	m_menuWidget.addButton(0, 7, "Done", [this] (TextButton &) {
		m_menuWidget.reset(1, 8);
		addMainButtons();
	});
}

void SettingsMenuState::addInputButtons() {
	m_menuWidget.addButton(0, 7, "Done", [this] (TextButton &) {
		m_menuWidget.reset(1, 8);
		addMainButtons();
	});
}

void SettingsMenuState::draw(RenderTarget &target, RenderStates states) const {
	if (m_parent)
		target.draw(*m_parent, states);

	states.transform *= getTransform();

	states.shader = &m_shader;
	states.vertexAttributes = VertexAttribute::Only2d;

	target.draw(m_background, states);
	target.draw(m_menuWidget, states);
}

