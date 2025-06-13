//
// Created by loedd on 2025/3/7.
//

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#pragma once

#include <vector>

constexpr float BLOCK_SIZE = 48.0f;
constexpr float BLOCK_MAGNIFICATION = 3.0f;
constexpr float FENCE_MAGNIFICATION = 1.0f;
constexpr float MOUNT_MAGNIFICATION = 1.5;
constexpr float CLOUD_MAGNIFICATION = 0.5;
constexpr float SMALL_TUBE_X_OFFSET = -310.0f;
constexpr float MID_TUBE_X_OFFSET = -313.0f;
constexpr float LARGE_TUBE_X_OFFSET = -313.0f;
constexpr float SMALL_TUBE_Y_OFFSET = -360.0f;
constexpr float MID_TUBE_Y_OFFSET = -385.0f;
constexpr float LARGE_TUBE_Y_OFFSET = -410.0f;
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
constexpr float BACKGROUND_Y_OFFSET = -335.0f;
// Start: 400, Level1_1: 400, ..., Level1_3: 300
const std::vector<int> LEVEL_TIME = {400, 400, 400, 300};
//constexpr float WINDOW_WIDTH = 720;
//constexpr float WINDOW_HEIGHT = 720;

#endif //GLOBAL_HPP
