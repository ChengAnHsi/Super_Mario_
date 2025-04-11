#include <Enemy.hpp>

float Enemy::GetMoveVelocity(){
    return move_velocity;
}

/*void Enemy::OnUpdate(const float delta) {
    float distance = GetMoveVelocity() * delta;

    // facing left
    if (isFacingRight == false) {
        //m_Transform.scale.x *= -1;
        distance *= -1;
    }
    // facing right
    if (isFacingRight == true)  {
        //m_Transform.scale.x = abs(m_Transform.scale.x);
    }

    UpdateAnimation();

    Action(distance);
}*/