/*
 * =====================================================================================
 *
 *       Filename:  ItemBlock.cpp
 *
 *    Description:
 *
 *        Created:  25/06/2018 17:28:36
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#include "ItemBlock.hpp"
#include "Registry.hpp"

ItemBlock::ItemBlock(u32 id, u32 blockID, const std::string &name, const std::string &label) : Item(id, Registry::getInstance().getBlock(blockID).textureID(), name, label) {
	m_isBlock = true;
}

