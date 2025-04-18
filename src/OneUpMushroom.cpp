#include "OneUpMushroom.hpp"

void OneUpMushroom::AfterCollisionEvents(){

}

void OneUpMushroom::SpawnProps() {
    state = MushroomState::Spawning;
    remaining_distance = BLOCK_SIZE;
    velocityY = 3.0f; // 向上速度
    Update(1.0f);
}

void OneUpMushroom::Update(float dt) {
    if (state == MushroomState::Spawning) {
        float move = velocityY * dt;
        float actual_move = std::min(-move, remaining_distance);
        this->SetPosition(this->GetPosition().x, this->GetPosition().y - actual_move);
        remaining_distance -= actual_move;

        if (remaining_distance <= 0.0f) {
            // 浮出完成，進入正常狀態
            state = MushroomState::Active;

            // 初始化正常下落速度，反向
            velocityY = 0.0f;
        }
    } else if (state == MushroomState::Active) {
        // 模擬重力
        velocityY += gravity * dt;
        this->SetPosition(this->GetPosition().x + velocityX * dt, this->GetPosition().y + velocityY * dt);

        // TODO: add collision detect, change direction
    }
}



