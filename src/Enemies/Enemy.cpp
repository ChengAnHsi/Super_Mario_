#include <Enemies/Enemy.hpp>

void Enemy::CheckFireballCollision(std::shared_ptr<Fireball> fireball) {
    // todo
}

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

void Enemy::SetDeadState(DeadState deadState) {
    dead_state = deadState;
}

bool Enemy::GetIsDead(){
    return is_dead;
}

bool Enemy::GetFacingRight() {
    return isFacingRight;
}

void Enemy::SetFacingRight(bool facingRight) {
    isFacingRight = facingRight;
}
