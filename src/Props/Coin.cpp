//
// Created by 112590007 on 2025/4/25.
//

#include "Props/Coin.hpp"
#include "Global.hpp"

void Coin::SpawnProps() {
    state = PropsState::Spawning;
    remaining_distance = BLOCK_SIZE * 4.0f;
    velocityY = 18.0f; // 向上速度
    is_goingup = true;
    SetVisible(true);
}

void Coin::Update(float dt) {
    if (state == PropsState::Spawning) {
        float move = velocityY * dt;

        if (is_goingup) {
            float actual_move = std::min(move, remaining_distance);
            this->SetPosition(this->GetPosition().x, this->GetPosition().y + actual_move);
            remaining_distance -= actual_move;

            if (remaining_distance <= 0.0f) {
                is_goingup = false;
                remaining_distance = BLOCK_SIZE * 4.0f;
            }
        }else {
            float actual_move = std::min(move, remaining_distance);
            SetPosition(GetPosition().x, GetPosition().y - actual_move);
            remaining_distance -= actual_move;

            if (remaining_distance <= 0.0f) {
                state = PropsState::Active;
            }
        }
    } else if (state == PropsState::Active) {
        SetVisible(false);
    }
}