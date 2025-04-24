#include "FireFlower.hpp"
#include "Global.hpp"

void FireFlower::AfterCollisionEvents(){

}

void FireFlower::SpawnProps() {
    state = PropsState::Spawning;
    // 對道具大小作偏移 10.0f
    remaining_distance = BLOCK_SIZE - 10.0f;
    velocityY = 3.0f; // 向上速度
}

void FireFlower::Update(float dt) {
    if (state == PropsState::Spawning) {
        float move = velocityY * dt;
        float actual_move = std::min(move, remaining_distance);
        this->SetPosition(this->GetPosition().x, this->GetPosition().y + actual_move);
        remaining_distance -= actual_move;

        if (remaining_distance <= 0.0f) {
            // 浮出完成，進入正常狀態
            state = PropsState::Active;

            // 初始化正常下落速度，反向
            velocityY = 0.0f;
        }
    } else if (state == PropsState::Active) {
        // 模擬重力
        // velocityY += gravity * dt;
        // this->SetPosition(this->GetPosition().x + velocityX * dt, this->GetPosition().y + velocityY * dt);

        // TODO: add collision detect, change direction
    }
}

// void FireFlower::SpawnProps() {
//     float mushroom_x = this->GetPosition().x;
//     float mushroom_y = this->GetPosition().y;
//
//     // each step move block size / 4
//     const float step = BLOCK_SIZE / 4.0f;
//     // distance = 1.0f, spawn speed
//     float remaining_distance = 1.0f;
//
//     while (std::abs(remaining_distance) > 0.0f) {
//         float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
//                                                           : std::max(-step, remaining_distance);
//         mushroom_y += step_distance;
//         this->SetPosition(mushroom_x, mushroom_y);
//         remaining_distance -= step_distance;
//     }
// }