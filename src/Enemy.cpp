#include <Enemy.hpp>

float Enemy::GetMoveVelocity(){
    return move_velocity;
}

bool Enemy::GetOverworld() {
    return isOverWorld;
}

void Enemy::SetOverworld(bool is_overworld) {
    isOverWorld = is_overworld;
}