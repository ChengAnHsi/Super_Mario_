//
// Created by 112590007 on 2025/4/11.
//

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "AnimatedImage.hpp"

enum class BlockState {
    Idle,
    Bouncing
};

class Block : public BackgroundImage {

public:
    enum class TYPE{
        None,
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
    ~Block() override = default;

    void TriggerJumpAnimation();
    void Update(float dt);

    virtual void AfterCollisionEvents() = 0;

    bool GetBroken();
    int GetCollisionTime();
    TYPE GetBlockType();

protected:
    bool isoverworld = true;
    bool isbreak = false;
    bool iscollision = false;
    TYPE Blocktype = TYPE::None;
    int collision_time = 1;

    // animation
    BlockState state = BlockState::Idle;
    float velocityY = 0.0f;
    float remaining_distance = 0.0f;
    bool is_goingup = true;
};

#endif //BLOCK_HPP