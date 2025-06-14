#include "Blocks/FlyPlatfrom.hpp"

void FlyPlatform::Update(float deltaTime) {
    previousX = m_Transform.translation.x;
    previousY = m_Transform.translation.y;

    // Update X movement if range is set
    if (MoveRangeX > 0) {
        if (MovingRightX) {
            TempMoveX += SpeedX * deltaTime;
            m_Transform.translation.x += SpeedX * deltaTime;

            if (TempMoveX >= MoveRangeX) {
                MovingRightX = false;
            }
        } else {
            TempMoveX -= SpeedX * deltaTime;
            m_Transform.translation.x -= SpeedX * deltaTime;

            if (TempMoveX <= 0) {
                MovingRightX = true;
            }
        }
    }

    // Update Y movement if range is set
    if (MoveRangeY > 0) {
        if (MovingUpY) {
            TempMoveY += SpeedY * deltaTime;
            m_Transform.translation.y += SpeedY * deltaTime;

            if (TempMoveY >= MoveRangeY) {
                MovingUpY = false;
            }
        } else {
            TempMoveY -= SpeedY * deltaTime;
            m_Transform.translation.y -= SpeedY * deltaTime;

            if (TempMoveY <= 0) {
                MovingUpY = true;
            }
        }
    }

    deltaX = m_Transform.translation.x - previousX;
    deltaY = m_Transform.translation.y - previousY;
}

void FlyPlatform::SetMovementRange(float rangeX, float rangeY) {
    MoveRangeX = rangeX;
    MoveRangeY = rangeY;
}

void FlyPlatform::SetMovementSpeed(float speedX, float speedY) {
    SpeedX = speedX;
    SpeedY = speedY;
}

void FlyPlatform::SetMovingDirection(bool directionRightX, bool directionUpY) {
    this->MovingRightX = directionRightX;
    this->MovingUpY = directionUpY;
}

void FlyPlatform::SetTempMoveXY(float tmepX, float tempY) {
    this->TempMoveX = tmepX;
    this->TempMoveY = tempY;
}

void FlyPlatform::SetMoving(bool isMoving) {
    this->isMoving = isMoving;
}

bool FlyPlatform::GetMoving() {
    return isMoving;
}

float FlyPlatform::GetDeltaX() const {
    return deltaX;
}
float FlyPlatform::GetDeltaY() const {
    return deltaY;
}

void FlyPlatform::ResetToInitialPosition() {
    if (m_Transform.translation.y >= 360.0f) {
        m_Transform.translation.y = -360.0f;
    }else {
        m_Transform.translation.y = 360.0f;
    }
    if (MovingUpY) TempMoveY = 0.0f;
    else TempMoveY = MoveRangeY;
}