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

    enum class PROP_TYPE{
        None,
        FireFlower,
        MagicMushroom,
        OneUpMushroom,
        Starman,
        Coin
    };

    Block() = default;
    virtual ~Block() = default;

    virtual void AfterCollisionEvents() = 0;

    bool GetBroken();
    int GetCollisionTime();
    TYPE GetBlockType();
    void SetInsidePropType(PROP_TYPE prop_type);
    PROP_TYPE GetInsidePropType();

protected:
    bool isoverworld = true;
    bool isbreak = false;
    bool iscollision = false;
    TYPE Blocktype;
    PROP_TYPE inside_prop_type;
    int collision_time = 1;
private:

    // TODO block up and down animation
};

#endif //BLOCK_HPP