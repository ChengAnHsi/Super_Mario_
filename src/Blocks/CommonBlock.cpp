//
// Created by 112590007 on 2025/4/11.
//

#include "Blocks/CommonBlock.hpp"
#include "Util/SFX.hpp"

CommonBlock::CommonBlock()
{
    Blocktype = TYPE::CommonBlock;
}
void CommonBlock::AfterCollisionEvents() {
    if(isbreak == true) {
        return ;
    }

    isbreak = true;
    SetVisible(false);

    std::shared_ptr<Util::SFX> break_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/break_block.wav");
    break_sfx->SetVolume(100);
    break_sfx->Play();

    collision_time -= 1;
    if (collision_time == 0) iscollision = true;
}