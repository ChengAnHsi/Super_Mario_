#include <Enemies/Enemy.hpp>

#include "Global.hpp"
#include "Util/SFX.hpp"

bool Enemy::CheckFireballCollision(const std::shared_ptr<Fireball>& fireball) {
    glm::vec2 a = GetPosition();
    glm::vec2 a_size = this->m_Drawable->GetSize();
    a_size *= ENEMY_MAGNIFICATION;

    glm::vec2 b = fireball->GetPosition();
    glm::vec2 b_size = fireball->GetSize();

    b_size.x *= fireball->GetScale().x;
    b_size.y *= fireball->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    float aleft = a.x - a_size.x / 2;
    float aright = a.x + a_size.x / 2;
    float atop = a.y + a_size.y / 2;
    float abottom = a.y - a_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) {
        return false;
    }

    // if shot then ...
    this->SetDeadState(DeadState::Shot);
    this->SetLive(0);
    this->SetScale(ENEMY_MAGNIFICATION, -ENEMY_MAGNIFICATION);

    death_timer = 0.0f;
    velocityY = DEATH_JUMP_VELOCITY * 1.5f;

    std::shared_ptr<Util::SFX> kick_sfx = std::make_shared<Util::SFX>(RESOURCE_DIR"/Sound/Effects/kick.wav");
    kick_sfx->SetVolume(200);
    kick_sfx->Play();

    return true;
}

void Enemy::SetVelocityX(float velocityX){
    move_velocity = velocityX;
}

void Enemy::SetVelocityY(float velocityY) {
    this->velocityY = velocityY;
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

int Enemy::GetScore() {
    return score;
}

int Enemy::GetLive() {
    return live;
}