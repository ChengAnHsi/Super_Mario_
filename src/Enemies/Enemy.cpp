#include <Enemies/Enemy.hpp>

void Enemy::SetMoveVelocity(float veclocityX){
    move_velocity = veclocityX;
}

float Enemy::GetMoveVelocity(){
    return move_velocity;
}

bool Enemy::GetOverworld() {
    return isOverWorld;
}

void Enemy::SetOverworld(bool is_overworld) {
    isOverWorld = is_overworld;
}

bool Enemy::GetFacingRight() {
    return isFacingRight;
}

void Enemy::SetFacingRight(bool facingRight) {
    isFacingRight = facingRight;
}