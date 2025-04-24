//
// Created by 112590007 on 2025/4/11.
//

#include "Props.hpp"

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