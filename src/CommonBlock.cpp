//
// Created by 112590007 on 2025/4/11.
//
#include "CommonBlock.hpp"
#include "App.hpp"

CommonBlock::CommonBlock()
{
    Blocktype = TYPE::CommonBlock;
}
void CommonBlock::AfterCollisionEvents() {
    // TODO check props pos
    if(isbreak == true) {
        return ;
    }
    SetVisible(false);
    isbreak = true;
    std::shared_ptr<Util::SFX> break_sfx = std::make_shared<Util::SFX >(RESOURCE_DIR"/Sound/Effects/break_block.wav");
    break_sfx->SetVolume(100);
    break_sfx->Play();

    if (iscollision == false) {
        if (inside_prop) {
            inside_prop->SpawnProps();
            iscollision = true;
        }
    }
}
void CommonBlock::SetProps(std::shared_ptr<Props> prop){
    inside_prop = prop;
}
std::shared_ptr<Props> CommonBlock::GetProps() {
    return inside_prop;
}