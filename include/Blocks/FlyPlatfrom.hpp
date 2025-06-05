#ifndef FLY_PLATFORM_HPP
#define FLY_PLATFORM_HPP

#include "pch.hpp"
#include "Util/GameObject.hpp"
#include "Global.hpp"


static std::vector<std::string> PlatFromImg = {
    RESOURCE_DIR"/Scenery/FlyPlatfrom.png"
};

class FlyPlatform : public Util::GameObject {
public:
    FlyPlatform();
    FlyPlatform(int x, int y, int moveRangeX = 0, int moveRangeY = 50,
                float speedX = 0.0f, float speedY = 1.0f);

    virtual ~FlyPlatform() = default;


    void Update(float deltaTime);

    void SetScale(float scaleX, float scaleY);

    glm::vec2 GetPosition() const;

    void SetMovementRange(int rangeX, int rangeY);

    void SetMovementSpeed(float speedX, float speedY);

    void ResetToInitialPosition();

    void CheckRepositioning(float ceilingHeight);

private:
    // Initial position
    glm::vec2 InitialPosition;

    // Movement properties
    int MoveRangeX;
    int MoveRangeY;
    float SpeedX;
    float SpeedY;

    // Movement counters
    float TempMoveX;
    float TempMoveY;

    // Movement direction flags
    bool MovingRightX;
    bool MovingUpY;

    // Platform properties
    static constexpr float PLATFORM_MAGNIFICATION = 1.0f;
    static constexpr int PLATFORM_SIZE = 32; // Assuming 32x32 platform size
};

#endif // FLY_PLATFORM_HPP