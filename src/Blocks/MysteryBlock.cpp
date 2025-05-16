//
// Created by 112590007 on 2025/4/11.
//

#include "Blocks/MysteryBlock.hpp"

MysteryBlock::MysteryBlock(){
    Blocktype = TYPE::MysteryBlock;
}

void MysteryBlock::AfterCollisionEvents() {
    if(iscollision == true) {
        return;
    }

    if (collision_time > 0) {
        inside_prop->SpawnProps();
        collision_time -= 1;

        if (collision_time == 0) {
            iscollision = true;
            if (isoverworld) {
                SetImage({RESOURCE_DIR"/Blocks/Overworld/emptyBlock.png"}, 1000, 0);
            }else {
                SetImage({RESOURCE_DIR"/Blocks/Underworld/emptyBlock.png"}, 1000, 0);
            }
        }else {
            inside_prop->SetState(Props::PropsState::Not_Activated);
            inside_prop->SetVisible(true);
        }
    }
}

void MysteryBlock::SetCollisionTime(int time) {
    collision_time = time;
}

void MysteryBlock::SetProps(std::shared_ptr<Props> prop){
    inside_prop = prop;
}

std::shared_ptr<Props> MysteryBlock::GetProps() {
    return inside_prop;
}