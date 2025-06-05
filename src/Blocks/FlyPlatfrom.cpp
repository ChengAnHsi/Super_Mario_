#include "Blocks/FlyPlatfrom.hpp"
#include "BackgroundImage.hpp"
#include "Core/Texture.hpp"
#include "Core/Drawable.hpp"

FlyPlatform::FlyPlatform()

    : InitialPosition(0, 0),
      MoveRangeX(0),
      MoveRangeY(50),
      SpeedX(0.0f),
      SpeedY(1.0f),
      TempMoveX(0.0f),
      TempMoveY(0.0f),
      MovingRightX(true),
      MovingUpY(true) {
}

FlyPlatform::FlyPlatform(int x, int y, int moveRangeX, int moveRangeY,
                         float speedX, float speedY)
    : InitialPosition(x, y),
      MoveRangeX(moveRangeX),
      MoveRangeY(moveRangeY),
      SpeedX(speedX),
      SpeedY(speedY),
      TempMoveX(0.0f),
      TempMoveY(0.0f),
      MovingRightX(true),
      MovingUpY(true) {

    // Set initial position
    m_Transform.translation = glm::vec2(x * PLATFORM_SIZE + BACKGROUND_X_OFFSET,
                                       y * PLATFORM_SIZE + BACKGROUND_Y_OFFSET);
}

void FlyPlatform::Update(float deltaTime) {
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
}


void FlyPlatform::SetScale(float scaleX, float scaleY) {
    m_Transform.scale = glm::vec2(scaleX, scaleY);
}

glm::vec2 FlyPlatform::GetPosition() const {
    return m_Transform.translation;
}

void FlyPlatform::SetMovementRange(int rangeX, int rangeY) {
    MoveRangeX = rangeX;
    MoveRangeY = rangeY;
}

void FlyPlatform::SetMovementSpeed(float speedX, float speedY) {
    SpeedX = speedX;
    SpeedY = speedY;
}

void FlyPlatform::ResetToInitialPosition() {
    m_Transform.translation = InitialPosition;
    TempMoveX = 0.0f;
    TempMoveY = 0.0f;
    MovingRightX = true;
    MovingUpY = true;
}

void FlyPlatform::CheckRepositioning(float ceilingHeight) {
    // If platform moves above ceiling, reposition to bottom
    if (m_Transform.translation.y > ceilingHeight) {
        m_Transform.translation.y = BACKGROUND_Y_OFFSET; // Reset to bottom
        TempMoveY = 0.0f;
        MovingUpY = true;
    }
}