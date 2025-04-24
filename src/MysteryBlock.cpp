//
// Created by 112590007 on 2025/4/11.
//
#include "MysteryBlock.hpp"

MysteryBlock::MysteryBlock(){
    Blocktype = TYPE::MysteryBlock;
}

void MysteryBlock::AfterCollisionEvents() {
     // TODO check props pos
    if(iscollision == true) {
        return ;
    }

    if (isoverworld) {
        SetImage({RESOURCE_DIR"/Blocks/Overworld/emptyBlock.png"}, 1000, 0);
    }else {
        SetImage({RESOURCE_DIR"/Blocks/Underworld/emptyBlock.png"}, 1000, 0);
    }

    if (iscollision == false) {
        inside_prop->SpawnProps();
        iscollision = true;
    }
}
void MysteryBlock::SetProps(std::shared_ptr<Props> prop){
    inside_prop = prop;
}
std::shared_ptr<Props> MysteryBlock::GetProps() {
    return inside_prop;
}