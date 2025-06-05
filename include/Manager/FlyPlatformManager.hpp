#ifndef FLY_PLATFORM_MANAGER_HPP
#define FLY_PLATFORM_MANAGER_HPP

#include "pch.hpp"
#include "Blocks/FlyPlatfrom.hpp"
#include "Util/GameObject.hpp"
#include "Global.hpp"
#include "BackgroundImage.hpp"
class FlyPlatformManager {
public:

    FlyPlatformManager();

    ~FlyPlatformManager() = default;

    void Update(float deltaTime);

    void AddPlatform(std::shared_ptr<FlyPlatform> platform);

    std::vector<std::shared_ptr<Util::GameObject>> GetChildren();

    std::shared_ptr<FlyPlatform> GetNewPlatform();

    std::vector<std::shared_ptr<FlyPlatform>> GetPlatforms();
    void ClearPlatforms();
    void InitializePlatformsForPhase(int phase);

    std::vector<int> GetPlatformX(int phase);

    std::vector<int> GetPlatformY(int phase);
    void SetCeilingHeight(float height);

private:
    std::vector<std::shared_ptr<FlyPlatform>> m_Platforms;
    float m_CeilingHeight;

    // Platform spawn positions for different phases
    // Phase 1 positions
    std::vector<int> platform_x_phase1 = {22, 94, 109, 129, 130,
                                          64,
                                          16, 21, 23, 78, 94, 101, 106, 109, 112, 170};
    std::vector<int> platform_y_phase1 = {9, 9, 9, 9, 9,
                                          6,
                                          5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

    // Phase 2 positions
    std::vector<int> platform_x_phase2 = {30, 50, 70, 90, 110, 130, 150};
    std::vector<int> platform_y_phase2 = {8, 7, 6, 8, 9, 7, 6};

    // Phase 3 positions
    std::vector<int> platform_x_phase3 = {40, 80, 120, 160, 200};
    std::vector<int> platform_y_phase3 = {10, 8, 6, 9, 7};


    void CreatePlatformsForPhase(int phase);
};

#endif // FLY_PLATFORM_MANAGER_HPP