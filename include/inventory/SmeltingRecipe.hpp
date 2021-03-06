/*
 * =====================================================================================
 *
 *       Filename:  SmeltingRecipe.hpp
 *
 *    Description:
 *
 *        Created:  30/06/2018 03:04:02
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#ifndef SMELTINGRECIPE_HPP_
#define SMELTINGRECIPE_HPP_

#include "Recipe.hpp"

class SmeltingRecipe : public Recipe {
	public:
		SmeltingRecipe(const ItemStack &input, const ItemStack &output);

		bool isMatching(const Inventory &inventory) const override;

	private:
		ItemStack m_input;
};

#endif // SMELTINGRECIPE_HPP_
