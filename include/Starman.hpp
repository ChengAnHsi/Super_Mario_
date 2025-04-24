//
// Created by 112590007 on 2025/4/11.
//

#ifndef STARMANAN_HPP
#define STARMANAN_HPP

#include "PropsManager.hpp"
#include "Global.hpp"

class Starman : public Props {
public:
    Starman() = default;

    void SpawnProps() override;
    void Update(float dt) override;
    void AfterCollisionEvents() override;

private:
    bool is_spawning = false;
    PropsState state = PropsState::Not_Activated;
    float remaining_distance = BLOCK_SIZE;
    float velocityY = -64.0f; // 向上浮出速度
    float gravity = 150.0f;   // 任意你想設的下落速度
    float velocityX = 40.0f;  // 左右移動用
};

#endif //STARMANAN_HPP