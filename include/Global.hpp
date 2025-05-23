//
// Created by loedd on 2025/3/7.
//

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#pragma once

#include <vector>

constexpr float BLOCK_SIZE = 48.0f;
constexpr float BLOCK_MAGNIFICATION = 3.0f;
constexpr float SMALL_TUBE_X_OFFSET = -310.0f;
constexpr float MID_TUBE_X_OFFSET = -313.0f;
constexpr float LARGE_TUBE_X_OFFSET = -313.0f;
constexpr float SMALL_TUBE_Y_OFFSET = -350.0f;
constexpr float MID_TUBE_Y_OFFSET = -375.0f;
constexpr float LARGE_TUBE_Y_OFFSET = -400.0f;
constexpr float SMALL_TUBE_MAGNIFICATION = 3.0f;
constexpr float MID_TUBE_MAGNIFICATION = 3.0f;
constexpr float LARGE_TUBE_MAGNIFICATION = 3.0f;
constexpr float MARIO_MAGNIFICATION = 2.5f;
constexpr float GOOMBA_MAGNIFICATION = 2.5f;
constexpr float ENEMY_MAGNIFICATION = 2.5f;
constexpr float KOOPA_MAGNIFICATION = 2.5f;
constexpr float FLOWER_MAGNIFICATION = 2.5f;
constexpr float PROP_MAGNIFICATION = 2.5f;
constexpr float FIREBALL_MAGNIFICATION = 3.0f;
constexpr float BACKGROUND_X_OFFSET = -335.0f;
constexpr float BACKGROUND_Y_OFFSET = -325.0f;
//constexpr int WINDOW_WIDTH = 720;
//constexpr int WINDOW_HEIGHT = 720;
// Start: 400, Level1_1: 400, ..., Level1_3: 300
const std::vector<int> LEVEL_TIME = {400, 400, 400, 300};

/**constexpr float BRICK_PARTICLE_SPEED = 4;
//A coin jumping out of the question block.
constexpr float COIN_JUMP_SPEED = -6;
constexpr float GOOMBA_SPEED = 1;
constexpr float GRAVITY = 0.25f;
constexpr float KOOPA_SHELL_SPEED = 4;
constexpr float KOOPA_SPEED = 1;
constexpr float MARIO_ACCELERATION = 0.25f;
constexpr float MARIO_JUMP_SPEED = -4;
constexpr float MARIO_WALK_SPEED = 2;
constexpr float MAX_VERTICAL_SPEED = 8;
constexpr float MUSHROOM_SPEED = 1;

constexpr unsigned char CELL_SIZE = 16;
//We use this constant when Mario is growing or when he's invincible.
constexpr unsigned char MARIO_BLINKING = 4;
//Lemme explain:
//1) Mario gets hit by an enemy.
//2) Mario looks at you like "Oh no! Me lost! Mama mia!" and pauses for this amount.
//3) Mario bounces off the screen.
constexpr unsigned char MARIO_DEATH_DURATION = 32;
constexpr unsigned char MARIO_GROWTH_DURATION = 64;
constexpr unsigned char MARIO_INVINCIBILITY_DURATION = 128;
//The longer we press the jump button, the higher Mario jumps.
constexpr unsigned char MARIO_JUMP_TIMER = 8;
constexpr unsigned char SCREEN_RESIZE = 2;

constexpr unsigned short COIN_ANIMATION_SPEED = 4;
//Since we deactivate everything outside the view, we need to have a safe area around the view where objects start updating.
constexpr unsigned short ENTITY_UPDATE_AREA = 64;
constexpr unsigned short GOOMBA_DEATH_DURATION = 32;
//Why do I have so many animation speed constants? I use them only once.
constexpr unsigned short GOOMBA_WALK_ANIMATION_SPEED = 32;
constexpr unsigned short KOOPA_GET_OUT_ANIMATION_SPEED = 4;
constexpr unsigned short KOOPA_GET_OUT_DURATION = 512;
constexpr unsigned short KOOPA_WALK_ANIMATION_SPEED = 32;
constexpr unsigned short MARIO_WALK_ANIMATION_SPEED = 4;
constexpr unsigned short QUESTION_BLOCK_ANIMATION_SPEED = 8;
constexpr unsigned short SCREEN_HEIGHT = 240;
constexpr unsigned short SCREEN_WIDTH = 320;

constexpr std::chrono::microseconds FRAME_DURATION(16667);

enum Cell
{
	ActivatedQuestionBlock,
	Brick,
	Coin,
	Empty,
	Pipe,
	QuestionBlock,
	Wall
};

//We use this for brick particles.
//Damn, I should've called it "Particle"! I'm so dumb!
struct Object
{
	float horizontal_speed;
	float vertical_speed;
	float x;
	float y;

	Object(const float i_x, const float i_y, const float i_horizontal_speed = 0, const float i_vertical_speed = 0) :
		horizontal_speed(i_horizontal_speed),
		vertical_speed(i_vertical_speed),
		x(i_x),
		y(i_y)
	{

	}
};

typedef std::vector<std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>> Map;**/



#endif //GLOBAL_HPP
