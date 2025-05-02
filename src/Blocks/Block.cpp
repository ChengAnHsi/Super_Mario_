//
// Created by 112590007 on 2025/4/11.
//

#include "Blocks/Block.hpp"
#include "Global.hpp"

void Block::TriggerJumpAnimation() {
    if (state == BlockState::Idle && iscollision == false) {
        state = BlockState::Bouncing;
        velocityY = 10.0f;
        remaining_distance = BLOCK_SIZE * 0.5f;
        is_goingup = true;
    }
}

void Block::Update(float dt) {
    if (state == BlockState::Bouncing) {
        float move = velocityY * dt;

        if (is_goingup) {
            float actual_move = std::min(move, remaining_distance);
            SetPosition(GetPosition().x, GetPosition().y + actual_move);
            remaining_distance -= actual_move;

            if (remaining_distance <= 0.0f) {
                is_goingup = false;
                remaining_distance = BLOCK_SIZE * 0.5f;
            }
        } else {
            float actual_move = std::min(move, remaining_distance);
            SetPosition(GetPosition().x, GetPosition().y - actual_move);
            remaining_distance -= actual_move;

            if (remaining_distance <= 0.0f) {
                state = BlockState::Idle;
                // animation end，位置回復
            }
        }
    }
}

bool Block::GetBroken() {
    return isbreak;
}

int Block::GetCollisionTime() {
    return collision_time;
}

Block::TYPE Block::GetBlockType(){
    return Blocktype;
};

void Block::SetInsidePropType(PROP_TYPE prop_type){
    inside_prop_type = prop_type;
};

Block::PROP_TYPE Block::GetInsidePropType(){
    return inside_prop_type;
};