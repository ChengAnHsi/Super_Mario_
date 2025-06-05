#include "Manager//FlyPlatformManager.hpp"
#include "Blocks/FlyPlatfrom.hpp"
#include "Global.hpp"
#include "BackgroundImage.hpp"
FlyPlatformManager::FlyPlatformManager()
    : m_CeilingHeight(600.0f) { // Default ceiling height
    m_Platforms -> SetImage(PlatFromImg[0],1000,0);
    InitializePlatformsForPhase(1);
}

void FlyPlatformManager::Update(float deltaTime) {
    // Update all platforms
    for (auto& platform : m_Platforms) {
        platform->Update(deltaTime);

        // Check if platform needs repositioning due to ceiling
        platform->CheckRepositioning(m_CeilingHeight);
    }
}

void FlyPlatformManager::AddPlatform(std::shared_ptr<FlyPlatform> platform) {
    if (platform) {
        m_Platforms.push_back(platform);
    }
}

std::vector<std::shared_ptr<Util::GameObject>> FlyPlatformManager::GetChildren() {
    std::vector<std::shared_ptr<Util::GameObject>> all_obj;
    all_obj.reserve(m_Platforms.size()); // Reserve space for better performance

    for (auto& platform : m_Platforms) {
        all_obj.push_back(std::static_pointer_cast<Util::GameObject>(platform));
    }

    return all_obj;
}

std::shared_ptr<FlyPlatform> FlyPlatformManager::GetNewPlatform() {
    return std::make_shared<FlyPlatform>();
}

std::vector<std::shared_ptr<FlyPlatform>> FlyPlatformManager::GetPlatforms() {
    return m_Platforms;
}

void FlyPlatformManager::ClearPlatforms() {
    m_Platforms.clear();
}

void FlyPlatformManager::InitializePlatformsForPhase(int phase) {
    ClearPlatforms();
    CreatePlatformsForPhase(phase);
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

void FlyPlatformManager::CreatePlatformsForPhase(int phase) {
    std::vector<int> x_positions = GetPlatformX(phase);
    std::vector<int> y_positions = GetPlatformY(phase);

    // Ensure both vectors have the same size
    size_t platformCount = std::min(x_positions.size(), y_positions.size());

    for (size_t i = 0; i < platformCount; ++i) {
        // Create platform with different movement patterns based on position
        int moveRangeX = 0; // Default no horizontal movement
        int moveRangeY = 50; // Default vertical movement range
        float speedX = 0.0f;
        float speedY = 30.0f; // Default vertical speed

        // Customize movement based on platform index or position
        if (i % 3 == 0) {
            // Every 3rd platform moves horizontally
            moveRangeX = 64; // 2 block widths
            speedX = 20.0f;
        }

        if (i % 2 == 0) {
            // Every other platform has different vertical speed
            speedY = 25.0f;
        }

        // Create platform
        auto platform = std::make_shared<FlyPlatform>(
            x_positions[i], y_positions[i],
            moveRangeX, moveRangeY,
            speedX, speedY
        );

        // Set platform appearance
        platform->SetScale(1.0f, 1.0f);

        // Add to collection
        m_Platforms.push_back(platform);
    }
}