//
// Created by 112590007 on 2025/4/11.
//

#ifndef COMMONBLOCK_HPP
#define COMMONBLOCK_HPP

#include "Block.hpp"

class CommonBlock : public Block{
public:
    CommonBlock();
    ~CommonBlock() override = default;

    void AfterCollisionEvents() override;
};

#endif //COMMONBLOCK_HPP