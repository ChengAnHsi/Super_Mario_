//
// Created by 112590007 on 2025/4/11.
//
#ifndef IMMOVABLEBLOCK_HPP
#define IMMOVABLEBLOCK_HPP

#include "Block.hpp"

class ImmovableBlock : public Block {
public:
    ImmovableBlock() = default;

    void AfterCollisionEvents() override;
private:
};

#endif //IMMOVABLEBLOCK_HPP