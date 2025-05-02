//
// Created by 112590007 on 2025/4/11.
//

#include "Blocks/ImmovableBlock.hpp"

ImmovableBlock::ImmovableBlock(){
    Blocktype = TYPE::ImmovableBlock;
    iscollision = true;
}

void ImmovableBlock::AfterCollisionEvents() {
    if(iscollision == true) {
        return ;
    }

    collision_time -= 1;
    if (collision_time == 0) iscollision = true;
}