/*
 * =====================================================================================
 *
 *       Filename:  Hotbar.cpp
 *
 *    Description:
 *
 *        Created:  20/06/2018 05:40:47
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "Config.hpp"
#include "Hotbar.hpp"

Hotbar::Hotbar(Inventory &inventory, Widget *parent) : Widget(182, 22, parent), m_inventory(inventory) {
	m_background.load("texture-widgets");
	m_background.setClipRect(0, 0, 182, 22);
	m_background.setPosition(0, 0, 0);

	m_cursor.load("texture-widgets");
	m_cursor.setClipRect(0, 22, 24, 24);
	m_cursor.setPosition(-1, -1, 0);

	for (u16 i = 0 ; i < 9 ; ++i) {
		ItemWidget &widget = m_items.emplace_back(m_inventory, i, 0);
		widget.setPosition(5 + 20 * i - 3, 2, 0);
	}
}

void Hotbar::onEvent(const S_Event &event) {
	if (event.type == S_EventType(S_EventTypeID::MouseWheelScrolled)) {
#ifdef USE_SDL
		if (event.wheel.y == -1)
#elif defined USE_SFML
		if (int(event.mouseWheelScroll.delta) == -1)
#endif // USE_SDL, USE_SFML
			m_cursorPos = (m_cursorPos + 1) % 9;
#ifdef USE_SDL
		else if (event.wheel.y == 1)
#elif defined USE_SFML
		else if (int(event.mouseWheelScroll.delta) == 1)
#endif // USE_SDL, USE_SFML
			m_cursorPos = (m_cursorPos == 0) ? 8 : m_cursorPos - 1;

		m_cursor.setPosition(-1 + 20 * m_cursorPos, -1, 0);
	}
}

void Hotbar::update() {
	for (u16 i = 0 ; i < 9 ; ++i) {
		m_items[i].setStack(m_inventory.getStack(i, 0).item().id(), m_inventory.getStack(i, 0).amount());
	}
}

void Hotbar::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_background, states);

	for (auto &it : m_items)
		target.draw(it, states);

	target.draw(m_cursor, states);
}

