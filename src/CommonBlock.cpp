//
// Created by 112590007 on 2025/4/11.
//
#include "CommonBlock.hpp"
#include "App.hpp"
void CommonBlock::AfterCollisionEvents() {
    SetVisible(false);
    isbreak = true;
    std::shared_ptr<Util::SFX> break_sfx = std::make_shared<Util::SFX >(RESOURCE_DIR"/Sound/Effects/break_block.wav");
    break_sfx->SetVolume(100);
    break_sfx ->Play();
}