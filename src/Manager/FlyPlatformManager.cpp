#include "Manager//FlyPlatformManager.hpp"

void FlyPlatformManager::UpdatePlatforms(float deltaTime) {
    // Update all platforms
    for (auto& platform : m_Platforms) {
        platform->Update(deltaTime);

        // Check if platform needs repositioning due to ceiling
        if (platform->GetPosition().y >= m_CeilingHeight) {
            platform->ResetToInitialPosition();
        }
    }
}

void FlyPlatformManager::AddPlatform(std::shared_ptr<FlyPlatform> platform) {
    m_Platforms.push_back(platform);
}

std::vector<std::shared_ptr<Util::GameObject>> FlyPlatformManager::GetChildren() {
    std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
    for (auto& platform : m_Platforms) {
        all_obj.push_back(std::static_pointer_cast<Util::GameObject>(platform));
    }
    return all_obj;
}

void FlyPlatformManager::SetPlatforms(std::vector<std::shared_ptr<FlyPlatform>> platforms) {
    m_Platforms = platforms;
}

std::vector<std::shared_ptr<FlyPlatform>> FlyPlatformManager::GetPlatforms() {
    return m_Platforms;
}

void FlyPlatformManager::ClearPlatforms() {
    m_Platforms.clear();
}

std::vector<int> FlyPlatformManager::GetPlatformX(int phase) {
    switch (phase) {
        case 1:
            return platform_x_phase1;
        case 2:
            return platform_x_phase2;
        case 3:
            return platform_x_phase3;
        default:
            return platform_x_phase1;
    }
}

std::vector<int> FlyPlatformManager::GetPlatformY(int phase) {
    switch (phase) {
        case 1:
            return platform_y_phase1;
        case 2:
            return platform_y_phase2;
        case 3:
            return platform_y_phase3;
        default:
            return platform_y_phase1;
    }
}

void FlyPlatformManager::SetCeilingHeight(float height) {
    m_CeilingHeight = height;
}