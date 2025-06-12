#ifndef FLY_PLATFORM_MANAGER_HPP
#define FLY_PLATFORM_MANAGER_HPP

#include "Blocks/FlyPlatfrom.hpp"
#include "BackgroundImage.hpp"

class FlyPlatformManager {
public:
    FlyPlatformManager() = default;
    ~FlyPlatformManager() = default;

    void UpdatePlatforms(float deltaTime);

    std::vector<std::shared_ptr<Util::GameObject>> GetChildren();

    void AddPlatform(std::shared_ptr<FlyPlatform> platform);
    void SetPlatforms(std::vector<std::shared_ptr<FlyPlatform>> platforms);
    std::vector<std::shared_ptr<FlyPlatform>> GetPlatforms();
    void ClearPlatforms();

    std::vector<int> GetPlatformX(int phase);
    std::vector<int> GetPlatformY(int phase);
    std::vector<float> GetPlatformRange(int phase);
    std::vector<int> GetPlatformDirection(int phase);

    void CheckMovingPlatform();
    void SetCeilingHeight(float height);

private:
    std::vector<std::shared_ptr<FlyPlatform>> m_Platforms;
    float m_CeilingHeight = 360.0f;

    // Platform spawn positions for different phases
    // XY
    // move range(one direction)
    // direction

    // Phase 1 positions(no platforms here)

    // Phase 2 positions
    std::vector<int> platform_x_phase2 = {141, 156, 141, 156};
    std::vector<int> platform_y_phase2 = {9, 9, 3, 3};
    // phase 2 all platform is one direction, so use bigger range(30.0f) to achieve it(no turn direction)
    std::vector<float> platform_range_phase2 = {30.0f, 30.0f, 30.0f, 30.0f};

    // X:left(0), right(1), Y:up(2), down(3)
    std::vector<int> platform_direcion_phase2 = {3, 2, 3, 2};

    // Phase 3 positions
    std::vector<int> platform_x_phase3 = {56, 131, 87, 95};
    std::vector<int> platform_y_phase3 = {9, 8, 6, 5};
    std::vector<float> platform_range_phase3 = {8, 3, 3, 3};

    std::vector<int> platform_direcion_phase3 = {3, 0 ,0 ,0};
};

#endif // FLY_PLATFORM_MANAGER_HPP