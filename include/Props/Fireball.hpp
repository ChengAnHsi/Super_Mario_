//
// Created by 112590007 on 2025/5/20.
//

#ifndef FIREBALL_HPP
#define FIREBALL_HPP

#include "BackgroundImage.hpp"
#include "Blocks/Block.hpp"
#include "CollisionState.hpp"

class Fireball final : public AnimatedImage {
public:
    Fireball() = default;
    ~Fireball() override = default;

    void Update(float delta); // update position and animation

    void Action(float distance);
    void Jump();

    // collision function
    bool AABBCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta);

    void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes);
    void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks);
    void ClearCollisionBoxes();
    void ClearCollisionBlocks();

    void SetFacingRight(bool isFacingRight);
    void Exploded();
    bool GetExploded();
    bool GetExploing();
private:
    float velocityX = 8.5f;
    bool isFacingRight = true;

    float velocityY = 0.0f;
    float GRAVITY = -300.0f;
    float JUMP_VELOCITY = 170.0f;
    bool isJumping = false;

    bool isExploded = false;
    bool isExploing = false;

    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;
    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<Block>> collision_blocks;
};

#endif //FIREBALL_HPP