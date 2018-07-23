/*
 * =====================================================================================
 *
 *       Filename:  PlayerInventoryWidget.cpp
 *
 *    Description:
 *
 *        Created:  27/06/2018 05:34:04
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "PlayerInventoryWidget.hpp"

PlayerInventoryWidget::PlayerInventoryWidget(Inventory &playerInventory, Widget *parent)
	: Widget(176, 166, parent), m_playerInventory(playerInventory)
{
	m_background.load("texture-inventory");
	m_background.setClipRect(0, 0, 176, 166);

	m_playerInventoryWidget.init(m_playerInventory, 9, 9 * 3);
	m_playerInventoryWidget.setPosition(7, 83, 0);

	m_hotbarInventoryWidget.init(m_playerInventory, 0, 9);
	m_hotbarInventoryWidget.setPosition(7, 141, 0);

	m_craftingWidget.craftingInventoryWidget().setPosition(97, 17, 0);
	m_craftingWidget.craftingResultInventoryWidget().setPosition(153, 27, 0);
}

void PlayerInventoryWidget::onEvent(const S_Event &event) {
	m_craftingWidget.onMouseEvent(event, m_mouseItemWidget);

	m_playerInventoryWidget.onMouseEvent(event, m_mouseItemWidget);
	m_hotbarInventoryWidget.onMouseEvent(event, m_mouseItemWidget);

	m_mouseItemWidget.onEvent(event);
}

void PlayerInventoryWidget::update() {
	m_craftingWidget.update();

	const ItemWidget *currentItemWidget = nullptr;
	if ((currentItemWidget = m_playerInventoryWidget.currentItemWidget())
	 || (currentItemWidget = m_hotbarInventoryWidget.currentItemWidget())
	 || (currentItemWidget = m_craftingWidget.currentItemWidget()))
		m_mouseItemWidget.update(currentItemWidget);
	else
		m_mouseItemWidget.update(nullptr);
}

void PlayerInventoryWidget::draw(RenderTarget &target, RenderStates states) const {
	applyTransform(states);

	target.draw(m_background, states);

	target.draw(m_craftingWidget, states);

	target.draw(m_playerInventoryWidget, states);
	target.draw(m_hotbarInventoryWidget, states);

	target.draw(m_mouseItemWidget, states);
}

