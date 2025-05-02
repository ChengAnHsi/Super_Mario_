//
// Created by 112590007 on 2025/4/11.
//

#include "Props/Props.hpp"

void Props::Move() {}

void Props::Action(float distance) {}

bool Props::GravityAndCollision(float delta) {return false;}

void Props::AfterCollisionEvents() {}

void Props::UpdateAnimation(){}

void Props::SetState(PropsState props_state) {
    state = props_state;
}

void Props::AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    for (const auto& box : boxes) {
        collision_boxes.push_back(box);
    }
}

void Props::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void Props::AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    for (const auto& block : blocks) {
        collision_blocks.push_back(block);
    }
}

void Props::ClearCollisionBlocks() {
    collision_blocks.clear();
}