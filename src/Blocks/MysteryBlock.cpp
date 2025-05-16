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
        auto prop_to_spawn = GetProps();
        prop_to_spawn->SpawnProps();
        collision_time -= 1;

        if (collision_time == 0) {
            iscollision = true;
            if (isoverworld) {
                SetImage({RESOURCE_DIR"/Blocks/Overworld/emptyBlock.png"}, 1000, 0);
            } else {
                SetImage({RESOURCE_DIR"/Blocks/Underworld/emptyBlock.png"}, 1000, 0);
            }
        } else {
            prop_to_spawn->SetState(Props::PropsState::Not_Activated);
            prop_to_spawn->SetVisible(true);
        }
    }
}

void MysteryBlock::SetCollisionTime(int time) {
    collision_time = time;
}

void MysteryBlock::SetInsidePropType(PROP_TYPE prop_type, PROP_TYPE prop_type2){
    inside_prop_type = prop_type;
    inside_prop_type2 = prop_type2;
}

std::vector<Block::PROP_TYPE> MysteryBlock::GetInsidePropType(){
    return {inside_prop_type, inside_prop_type2};
}

void MysteryBlock::SetProps(std::shared_ptr<Props> prop1, std::shared_ptr<Props> prop2, bool multipleProps) {
    inside_prop1 = prop1;
    inside_prop2 = prop2;
    hasMultipleProps = multipleProps;
}

std::shared_ptr<Props> MysteryBlock::GetProps() {
    //if (hasMultipleProps && mario->GetGrow()) {
    if (hasMultipleProps) {
        // return fire flower
        return inside_prop2;
    }
    return inside_prop1;
}
