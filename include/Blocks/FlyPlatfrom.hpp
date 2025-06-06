#ifndef FLY_PLATFORM_HPP
#define FLY_PLATFORM_HPP

#include "BackgroundImage.hpp"

class FlyPlatform final : public BackgroundImage {
public:
    FlyPlatform() = default;
    ~FlyPlatform() override = default;

    void Update(float delta);

    void SetMovementRange(float rangeX, float rangeY);
    void SetMovementSpeed(float speedX, float speedY);
    void SetMovingDirection(bool directionRightX, bool directionUpY);
    void SetTempMoveXY(float tmepX, float tempY);
    void SetMoving(bool isMoving);
    bool GetMoving();

    void ResetToInitialPosition();

private:
    // Movement properties
    float MoveRangeX = 0.0f;
    float MoveRangeY = 0.0f;
    float SpeedX = 0.0f;
    float SpeedY = 0.0f;

    // Movement counters
    float TempMoveX = 0.0f;
    float TempMoveY = 0.0f;

    // Movement direction flags
    bool MovingRightX = false;
    bool MovingUpY = false;

    bool isMoving = false;
};

#endif // FLY_PLATFORM_HPP