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

bool Enemy::IsDead(){
    return  is_daed;
}
