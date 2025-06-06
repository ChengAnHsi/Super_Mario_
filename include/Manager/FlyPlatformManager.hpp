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

    void SetCeilingHeight(float height);

private:
    std::vector<std::shared_ptr<FlyPlatform>> m_Platforms;
    float m_CeilingHeight;

    // Platform spawn positions for different phases
    // Phase 1 positions(no platforms here)
    std::vector<int> platform_x_phase1 = {};
    std::vector<int> platform_y_phase1 = {};
    std::vector<bool> platform_direcion_is_X_phase1 = {};
    std::vector<bool> platform_direcion_up_phase1 = {};
    std::vector<bool> platform_direcion_left_phase1 = {};

    // Phase 2 positions
    std::vector<int> platform_x_phase2 = {141, 156, 141, 156};
    std::vector<int> platform_y_phase2 = {9, 9, 3, 3};
    std::vector<bool> platform_direcion_is_X_phase2 = {false, false, false, false};
    std::vector<bool> platform_direcion_up_phase2 = {false, false, true, true};
    std::vector<bool> platform_direcion_left_phase2 = {false, false, false, false};

    // Phase 3 positions
    // todo up down
    std::vector<int> platform_x_phase3 = {131, 87, 92, 56};
    std::vector<int> platform_y_phase3 = {8, 6, 5, 4};
    std::vector<bool> platform_direcion_is_X_phase3 = {false, true, true, true};
    std::vector<bool> platform_direcion_up_phase3 = {false, true, true, true};
    std::vector<bool> platform_direcion_left_phase3 = {false, true, true, true};
};

#endif // FLY_PLATFORM_MANAGER_HPP