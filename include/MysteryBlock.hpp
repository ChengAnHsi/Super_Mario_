//
// Created by 112590007 on 2025/4/11.
//
#ifndef MYSTERYBLOCK_HPP
#define MYSTERYBLOCK_HPP

#include "Props.hpp"
#include "Block.hpp"

class MysteryBlock : public Block {
public:
    MysteryBlock() = default;

    void AfterCollisionEvents() override;
private:
    Props inside_prop;
};

#endif //MYSTERYBLOCK_HPP