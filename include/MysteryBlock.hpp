//
// Created by 112590007 on 2025/4/11.
//
#ifndef MYSTERYBLOCK_HPP
#define MYSTERYBLOCK_HPP

#include "Props.hpp"
#include "Block.hpp"

class MysteryBlock : public Block {
public:
    MysteryBlock();

    void AfterCollisionEvents() override;

    void SetProps(std::shared_ptr<Props> prop);
    std::shared_ptr<Props> GetProps();
private:
    std::shared_ptr<Props> inside_prop;
};

#endif //MYSTERYBLOCK_HPP