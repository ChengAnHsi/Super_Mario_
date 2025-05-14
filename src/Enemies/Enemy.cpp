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

<<<<<<< HEAD
bool Enemy::IsDead(){
    return  is_daed;
}
=======
bool Enemy::GetFacingRight() {
    return isFacingRight;
}

void Enemy::SetFacingRight(bool facingRight) {
    isFacingRight = facingRight;
}
>>>>>>> 7072fb60e9e9d5b0003810f6291ea186926f49e2
