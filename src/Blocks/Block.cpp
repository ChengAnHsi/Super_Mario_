//
// Created by 112590007 on 2025/4/11.
//

#include "Blocks/Block.hpp"

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