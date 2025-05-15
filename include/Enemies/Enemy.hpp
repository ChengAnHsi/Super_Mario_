//
// Created by 112590007 on 2025/4/4.
//

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "AnimatedImage.hpp"
#include "Blocks/Block.hpp"

class Enemy : public BackgroundImage {

public:
    Enemy() = default;

    virtual void Move() = 0;
    virtual void Action(float distance) = 0;
    virtual bool GravityAndCollision(float delta) = 0;
    virtual void UpdateAnimation() = 0;
    //void OnUpdate(float delta);

    // getter and setter
    bool GetMoving() {return isMoving;}
    void SetMoving(bool moving) {isMoving = moving;}
    float GetMoveVelocity();
    bool GetOverworld();
    bool GetIsDead();
    void SetOverworld(bool is_overworld);
    void SetMoveVelocity(float veclocityX);
    bool GetFacingRight();
    void SetFacingRight(bool facingRight);
    virtual void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) = 0;
    virtual void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) = 0;
    virtual void ClearCollisionBoxes() = 0;
    virtual void ClearCollisionBlocks() = 0;
protected:
    bool isFacingRight = false;
private:
    bool is_dead = false;
    bool isMoving = false;
    float move_velocity = 2.0f;
    bool isOverWorld = true;
};
#endif //ENEMY_HPP
