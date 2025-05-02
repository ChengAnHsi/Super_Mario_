//
// Created by 112590007 on 2025/4/25.
//

#include "Props/Coin.hpp"
#include "Global.hpp"

void Coin::SpawnProps() {
    state = PropsState::Spawning;
    remaining_distance = BLOCK_SIZE * 3.5f;
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
                remaining_distance = BLOCK_SIZE * 3.5f;
            }
        }else {
            float actual_move = std::min(move, remaining_distance);
            SetPosition(GetPosition().x, GetPosition().y - actual_move);
            remaining_distance -= actual_move;

            if (remaining_distance <= BLOCK_SIZE * 1.5f) {
                state = PropsState::Active;
                SetPosition(GetPosition().x, GetPosition().y - remaining_distance);
            }
        }
    } else if (state == PropsState::Active) {
        SetVisible(false);
    }
}