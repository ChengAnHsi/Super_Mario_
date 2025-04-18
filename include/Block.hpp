//
// Created by 112590007 on 2025/4/11.
//

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "AnimatedImage.hpp"

class Block : public BackgroundImage {

public:
    enum class TYPE{
        MysteryBlock,
        CommonBlock,
        ImmovableBlock
    };

    Block() = default;
    virtual ~Block() = default;

    virtual void AfterCollisionEvents() = 0;

    bool GetBroken();
    TYPE GetBlocktype(){
        return Blocktype;
    };

protected:
    bool isoverworld = true;
    bool isbreak = false;
    bool iscollision = false;
    TYPE Blocktype;
private:

    // TODO block up and down animation
};

#endif //BLOCK_HPP