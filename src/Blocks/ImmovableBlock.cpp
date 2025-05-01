//
// Created by 112590007 on 2025/4/11.
//
#include "ImmovableBlock.hpp"

ImmovableBlock::ImmovableBlock()
{
    Blocktype = TYPE::ImmovableBlock;
}
void ImmovableBlock::AfterCollisionEvents() {
    // nothing to do
}