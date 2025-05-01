//
// Created by 112590007 on 2025/4/11.
//

#ifndef COMMONBLOCK_HPP
#define COMMONBLOCK_HPP

#include "../Props/Props.hpp"
#include "Block.hpp"

class CommonBlock : public Block{
public:
    CommonBlock();

    void AfterCollisionEvents() override;

    void SetProps(std::shared_ptr<Props> prop);
    std::shared_ptr<Props> GetProps();
private:
    std::shared_ptr<Props> inside_prop;
};

#endif //COMMONBLOCK_HPP