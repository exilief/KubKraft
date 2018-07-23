/*
 * =====================================================================================
 *
 *       Filename:  BlockCursor.cpp
 *
 *    Description:
 *
 *        Created:  14/06/2018 23:39:38
 *
 *         Author:  Quentin Bazin, <quent42340@gmail.com>
 *
 * =====================================================================================
 */
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "BlockCursor.hpp"
#include "Config.hpp"
#include "GameClock.hpp"
#include "Hotbar.hpp"
#include "Registry.hpp"
#include "Vertex.hpp"

void BlockCursor::updateVertexBuffer(const Block &block) {
	Vertex vertices[24] = {
		// Right
		{{1, 1, 1, -1}},
		{{1, 1, 0, -1}},
		{{1, 0, 0, -1}},
		{{1, 0, 1, -1}},

		// Top
		{{1, 1, 1, -1}},
		{{0, 1, 1, -1}},
		{{0, 1, 0, -1}},
		{{1, 1, 0, -1}},

		// Back
		{{1, 1, 1, -1}},
		{{1, 0, 1, -1}},
		{{0, 0, 1, -1}},
		{{0, 1, 1, -1}},

		// Left
		{{0, 1, 1, -1}},
		{{0, 1, 0, -1}},
		{{0, 0, 0, -1}},
		{{0, 0, 1, -1}},

		// Bottom
		{{1, 0, 1, -1}},
		{{0, 0, 1, -1}},
		{{0, 0, 0, -1}},
		{{1, 0, 0, -1}},

		// Front
		{{0, 0, 0, -1}},
		{{0, 1, 0, -1}},
		{{1, 1, 0, -1}},
		{{1, 0, 0, -1}},
	};

	for (u8 i = 0 ; i < 24 ; ++i) {
		vertices[i].coord3d[0] = vertices[i].coord3d[0] * block.boundingBox().width  + block.boundingBox().x;
		vertices[i].coord3d[1] = vertices[i].coord3d[1] * block.boundingBox().height + block.boundingBox().y;
		vertices[i].coord3d[2] = vertices[i].coord3d[2] * block.boundingBox().depth  + block.boundingBox().z;
	}

	VertexBuffer::bind(&m_vbo);
	m_vbo.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	VertexBuffer::bind(nullptr);
}

void BlockCursor::updateAnimationVertexBuffer(const Block &block, int animationPos) {
	Vertex vertices[24] = {
		// Right
		{{1, 1, 1, -1}},
		{{1, 1, 0, -1}},
		{{1, 0, 0, -1}},
		{{1, 0, 1, -1}},

		// Top
		{{1, 1, 1, -1}},
		{{0, 1, 1, -1}},
		{{0, 1, 0, -1}},
		{{1, 1, 0, -1}},

		// Back
		{{1, 1, 1, -1}},
		{{1, 0, 1, -1}},
		{{0, 0, 1, -1}},
		{{0, 1, 1, -1}},

		// Left
		{{0, 1, 1, -1}},
		{{0, 1, 0, -1}},
		{{0, 0, 0, -1}},
		{{0, 0, 1, -1}},

		// Bottom
		{{1, 0, 1, -1}},
		{{0, 0, 1, -1}},
		{{0, 0, 0, -1}},
		{{1, 0, 0, -1}},

		// Front
		{{0, 0, 0, -1}},
		{{0, 1, 0, -1}},
		{{1, 1, 0, -1}},
		{{1, 0, 0, -1}},
	};

	for (u8 i = 0 ; i < 24 ; ++i) {
		vertices[i].coord3d[0] = vertices[i].coord3d[0] * block.boundingBox().width  + block.boundingBox().x;
		vertices[i].coord3d[1] = vertices[i].coord3d[1] * block.boundingBox().height + block.boundingBox().y;
		vertices[i].coord3d[2] = vertices[i].coord3d[2] * block.boundingBox().depth  + block.boundingBox().z;
	}

	GLfloat color[4] = {1, 1, 1, 0.5};
	for (int i = 0 ; i < 24 ; ++i)
		memcpy(vertices[i].color, color, 4 * sizeof(GLfloat));

	if (animationPos != -1) {
		glm::vec4 blockTexCoords{0.1f * animationPos, 0.0, 0.1f + 0.1f * animationPos, 1.0};
		float faceTexCoords[2 * 4] = {
			blockTexCoords.x, blockTexCoords.w,
			blockTexCoords.z, blockTexCoords.w,
			blockTexCoords.z, blockTexCoords.y,
			blockTexCoords.x, blockTexCoords.y
		};

		for (u8 i = 0 ; i < 6 ; ++i) {
			for(u8 j = 0 ; j < 4 ; j++) {
				vertices[j + i * 4].texCoord[0] = faceTexCoords[j * 2];
				vertices[j + i * 4].texCoord[1] = faceTexCoords[j * 2 + 1];
			}
		}
	}

	VertexBuffer::bind(&m_animationVBO);
	m_animationVBO.setData(sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	VertexBuffer::bind(nullptr);
}

void BlockCursor::onEvent(const S_Event &event, const Hotbar &hotbar) {
#ifdef USE_SDL
	if (event.type == SDL_MOUSEBUTTONDOWN && m_selectedBlock.w != -1) {
		if (event.button.button == SDL_BUTTON_LEFT) {
#elif defined USE_SFML
    if (event.type == sf::Event::MouseButtonPressed && m_selectedBlock.w != -1) {
        if (event.mouseButton.button == sf::Mouse::Left) {
#endif // USE_SDL, USE_SFML
			m_animationStart = GameClock::getTicks();
		}
#ifdef USE_SDL
		else if (event.button.button == SDL_BUTTON_RIGHT) {
#elif defined USE_SFML
        else if (event.mouseButton.button == sf::Mouse::Right) {
#endif // USE_SDL, USE_SFML
			u32 blockId = m_world.getBlock(m_selectedBlock.x, m_selectedBlock.y, m_selectedBlock.z);
			const Block &block = Registry::getInstance().getBlock(blockId);
			const Item &item = Registry::getInstance().getItem(hotbar.currentItem());

			if (block.id() && !block.onBlockActivated(m_selectedBlock, m_player, m_world)
			&& hotbar.currentItem() && item.isBlock()) {
				int face = m_selectedBlock.w;

				int x = m_selectedBlock.x;
				int y = m_selectedBlock.y;
				int z = m_selectedBlock.z;

				if(face == 0) x++;
				if(face == 3) x--;
				if(face == 1) y++;
				if(face == 4) y--;
				if(face == 2) z++;
				if(face == 5) z--;

				m_world.setBlock(x, y, z, hotbar.currentItem());

				const ItemStack &currentStack = m_player.inventory().getStack(hotbar.cursorPos(), 0);
				m_player.inventory().setStack(hotbar.cursorPos(), 0, currentStack.amount() > 1 ? currentStack.item().id() : 0, currentStack.amount() - 1);
			}
		}
	}
#ifdef USE_SDL
	else if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT) {
#elif defined USE_SFML
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
#endif // USE_SDL, USE_SFML
			m_animationStart = 0;
		}
	}
}

void BlockCursor::update(const Hotbar &hotbar, bool useDepthBuffer) {
	bool selectedBlockChanged = false;
	glm::vec4 selectedBlock = findSelectedBlock(useDepthBuffer);
	if (selectedBlock.x != m_selectedBlock.x || selectedBlock.y != m_selectedBlock.y || selectedBlock.z != m_selectedBlock.z)
		selectedBlockChanged = true;

	m_selectedBlock = selectedBlock;

	m_currentBlock = &Registry::getInstance().getBlock(m_world.getBlock(m_selectedBlock.x, m_selectedBlock.y, m_selectedBlock.z));
	// if (block.boundingBox().intersects(FloatBox{m_selectedBlock.x, m_selectedBlock.y, m_selectedBlock.z, 1, 1, 1})) {
	// 	selectedBlockChanged = false;
	// 	m_selectedBlock.w = -1;
	// }

	if (selectedBlockChanged)
		m_animationStart = (m_animationStart) ? GameClock::getTicks() : 0;

	const ItemStack &currentStack = m_player.inventory().getStack(hotbar.cursorPos(), 0);
	float timeToBreak = m_currentBlock->timeToBreak(currentStack.item().harvestCapability(), currentStack.item().miningSpeed());
	if (m_animationStart && GameClock::getTicks() > m_animationStart + timeToBreak * 1000) {
		ItemStack itemDrop = m_currentBlock->getItemDrop();
		m_player.inventory().addStack(itemDrop.item().id(), itemDrop.amount());
		m_world.setBlock(m_selectedBlock.x, m_selectedBlock.y, m_selectedBlock.z, 0);
		m_animationStart = GameClock::getTicks();
	}

	if (m_selectedBlock.w != -1)
		updateVertexBuffer(*m_currentBlock);
	else
		m_currentBlock = nullptr;

	if (m_animationStart && m_currentBlock)
		updateAnimationVertexBuffer(*m_currentBlock, (GameClock::getTicks() - m_animationStart) / (timeToBreak * 100));
}

#include "ResourceHandler.hpp"

void BlockCursor::draw(RenderTarget &target, RenderStates states) const {
	if (m_selectedBlock.w == -1) return;

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_CULL_FACE);

	glm::mat4 modelMatrix = glm::translate(glm::mat4{1}, glm::vec3{m_selectedBlock});
	states.modelMatrix = &modelMatrix;

	target.draw(m_vbo, GL_LINES, 0, 24, states);

	if (m_animationStart > 0) {
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		states.texture = &ResourceHandler::getInstance().get<Texture>("texture-block_destroy");
		target.draw(m_animationVBO, GL_QUADS, 0, 24, states);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_POLYGON_OFFSET_FILL);
}

// Not really GLSL fract(), but the absolute distance to the nearest integer value
// FIXME: fract also exists in glm, check if its the same thing
float BlockCursor::fract(float value) const {
	float f = value - floorf(value);
	if(f > 0.5) return 1 - f;
	else return f;
}

glm::vec4 BlockCursor::findSelectedBlock(bool useDepthBuffer) const {
	int mx, my, mz;
	int face = -1;

	glm::vec3 lookAt{m_camera.pointTargetedX() - m_camera.x(),
	                 m_camera.pointTargetedY() - m_camera.y(),
	                 m_camera.pointTargetedZ() - m_camera.z()};

	glm::vec3 position{m_camera.x(),
	                   m_camera.y(),
	                   m_camera.z()};

	// FIXME: Add max distance
	if(useDepthBuffer) {
		// At which voxel are we looking? First, find out coords of the center pixel
		float depth;
		glReadPixels(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

		glm::vec4 viewport = glm::vec4(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glm::vec3 winCoord = glm::vec3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, depth);
		glm::vec3 objCoord = glm::unProject(winCoord, m_viewMatrix, m_projectionMatrix, viewport);

		// Find out which block it belongs to
		mx = objCoord.x;
		my = objCoord.y;
		mz = objCoord.z;

		if(objCoord.x < 0) mx--;
		if(objCoord.y < 0) my--;
		if(objCoord.z < 0) mz--;

		// Find out which face of the block we are looking at
		if(fract(objCoord.x) < fract(objCoord.y)) {
			if(fract(objCoord.x) < fract(objCoord.z)) {
				face = 0; // X
			} else {
				face = 2; // Z
			}
		} else {
			if(fract(objCoord.y) < fract(objCoord.z)) {
				face = 1; // Y
			} else {
				face = 2; // Z
			}
		}

		if(face == 0 && lookAt.x > 0) face += 3;
		if(face == 1 && lookAt.y > 0) face += 3;
		if(face == 2 && lookAt.z > 0) face += 3;
	} else {
		// Very naive ray casting algorithm to find out which block we are looking at
		glm::vec3 testPos = position;
		glm::vec3 prevPos = position;

		for(int i = 0 ; i < 100 ; i++) {
			// Advance from our current position to the direction we are looking at, in small steps
			prevPos = testPos;
			testPos += lookAt * 0.1f;

			mx = floorf(testPos.x);
			my = floorf(testPos.y);
			mz = floorf(testPos.z);

			// If we find a block that is not air, we are done
			u32 block = m_world.getBlock(mx, my, mz);
			if(block && block != BlockType::Water) break;
		}

		// Find out which face of the block we are looking at
		int px = floorf(prevPos.x);
		int py = floorf(prevPos.y);
		int pz = floorf(prevPos.z);

		if(px > mx) face = 0;
		else if(px < mx) face = 3;
		else if(py > my) face = 1;
		else if(py < my) face = 4;
		else if(pz > mz) face = 2;
		else if(pz < mz) face = 5;
	}

	// If we are looking at air, disable the cursor
	u32 block = m_world.getBlock(mx, my, mz);
	if(!block || block == BlockType::Water) {
		face = -1;
	}

	return {mx, my, mz, face};
}

