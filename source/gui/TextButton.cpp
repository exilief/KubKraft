/*
 * =====================================================================================
 *
 *       Filename:  TextButton.cpp
 *
 *    Description:
 *
 *        Created:  28/06/2018 10:18:03
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "TextButton.hpp"

TextButton::TextButton(Widget *parent) : Widget(200, 20, parent) {
	m_background.setClipRect(0, 66, 200, 20);
	m_hoverBackground.setClipRect(0, 86, 200, 20);
	m_disabledBackground.setClipRect(0, 46, 200, 20);
}

TextButton::TextButton(const Callback &callback, Widget *parent) : TextButton(parent) {
	m_callback = callback;
}

void TextButton::onEvent(const sf::Event &event) {
	if (event.type == sf::Event::MouseMoved) {
		m_isHovered = isPointInWidget(event.mouseMove.x, event.mouseMove.y);
	}
	else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && m_isHovered && m_callback) {
		m_callback(*this);
	}
}

void TextButton::setText(const std::string &text) {
	m_text.setText(text);
	m_text.setPosition(m_width / 2 - m_text.getSize().x / 2, m_height / 2 - m_text.getSize().y / 2, 0);
}

void TextButton::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	if (!m_isEnabled)
		target.draw(m_disabledBackground, states);
	else if (m_isHovered)
		target.draw(m_hoverBackground, states);
	else
		target.draw(m_background, states);
	target.draw(m_text, states);
}

