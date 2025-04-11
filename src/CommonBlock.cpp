//
// Created by 112590007 on 2025/4/11.
//
#include "CommonBlock.hpp"

void CommonBlock::AfterCollisionEvents() {
    SetVisible(false);
    isbreak = true;
}