//
// Created by 112590007 on 2025/4/11.
//
#ifndef ONEUPMUSHROOM_HPP
#define ONEUPMUSHROOM_HPP

#include "PropsManager.hpp"
#include "Global.hpp"

class OneUpMushroom : public Props {
public:
    enum class MushroomState {
        Spawning,
        Active
    };

    OneUpMushroom() = default;
    void SpawnProps() override;         // 叫出蘑菇
    void Update(float dt);
    void AfterCollisionEvents() override;

private:
    bool is_spawning = false;
    MushroomState state = MushroomState::Spawning;
    float remaining_distance = BLOCK_SIZE;
    float velocityY = -64.0f; // 向上浮出速度
    float gravity = 150.0f;   // 任意你想設的下落速度
    float velocityX = 40.0f;  // 左右移動用

};

#endif //ONEUPMUSHROOM_HPP