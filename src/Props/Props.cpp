//
// Created by 112590007 on 2025/4/11.
//

#include "Props/Props.hpp"
#include "Global.hpp"

void Props::Move() {}

void Props::Action(float distance) {}

bool Props::GravityAndCollision(float delta) {return false;}

bool Props::CheckCollisionWithMario(std::shared_ptr<Mario> mario) const {
    glm::vec2 a = mario->GetPosition();
    glm::vec2 mario_size = mario->GetSize();
    mario_size *= MARIO_MAGNIFICATION;

    glm::vec2 b = this->m_Transform.translation;
    glm::vec2 b_size = this->GetSize();

    b_size.x *= this->GetScale().x;
    b_size.y *= this->GetScale().y;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    float aleft = a.x - mario_size.x / 2;
    float aright = a.x + mario_size.x / 2;
    float atop = a.y + mario_size.y / 2;
    float abottom = a.y - mario_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) return false;
    return true;
}

void Props::AfterCollisionEvents(std::shared_ptr<Mario> mario) {}

void Props::UpdateAnimation(){}

void Props::SetState(PropsState props_state) {
    state = props_state;
}

Props::PropsState Props::GetState() const {
    return state;
}

void Props::AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) {
    for (const auto& box : boxes) {
        collision_boxes.push_back(box);
    }
}

void Props::ClearCollisionBoxes() {
    collision_boxes.clear();
}

void Props::AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) {
    for (const auto& block : blocks) {
        collision_blocks.push_back(block);
    }
}

void Props::ClearCollisionBlocks() {
    collision_blocks.clear();
}