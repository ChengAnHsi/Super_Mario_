//
// Created by 112590007 on 2025/4/4.
//

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "AnimatedImage.hpp"

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

private:
    bool isMoving = false;
    bool isFacingRight = false;
    float move_velocity = 3.5f;
};
#endif //ENEMY_HPP
