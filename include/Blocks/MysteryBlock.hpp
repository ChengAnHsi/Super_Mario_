//
// Created by 112590007 on 2025/4/11.
//
#ifndef MYSTERYBLOCK_HPP
#define MYSTERYBLOCK_HPP

#include "Props/Props.hpp"
#include "Block.hpp"

class MysteryBlock : public Block {
public:
    MysteryBlock();

    void AfterCollisionEvents() override;

    void SetCollisionTime(int time);
    void SetInsidePropType(PROP_TYPE prop_type, PROP_TYPE prop_type2);
    void SetChooseProp(bool choose1prop);
    std::vector<PROP_TYPE> GetInsidePropType();
    void SetProps(std::shared_ptr<Props> prop1, std::shared_ptr<Props> prop2, bool multipleProps = false);
    std::shared_ptr<Props> GetProps();
private:
    std::shared_ptr<Props> inside_prop1;
    std::shared_ptr<Props> inside_prop2;
    PROP_TYPE inside_prop_type;
    PROP_TYPE inside_prop_type2;
    bool choose_1prop = true;
    bool hasMultipleProps;
};

#endif //MYSTERYBLOCK_HPP