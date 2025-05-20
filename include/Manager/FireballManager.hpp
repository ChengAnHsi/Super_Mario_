//
// Created by 112590007 on 2025/5/20.
//

#ifndef FIREBALLMANAGER_HPP
#define FIREBALLMANAGER_HPP

#include "BackgroundImage.hpp"
#include "Props/Fireball.hpp"

class FireballManager {
public:
    FireballManager() = default;

    std::vector<std::string> imagePaths = {
        RESOURCE_DIR"/Entities/fireball0.png",
        RESOURCE_DIR"/Entities/fireball1.png",
        RESOURCE_DIR"/Entities/fireball2.png",
        RESOURCE_DIR"/Entities/fireball3.png",
        RESOURCE_DIR"/Entities/fireball_explosion0.png",
        RESOURCE_DIR"/Entities/fireball_explosion1.png",
        RESOURCE_DIR"/Entities/fireball_explosion2.png"
    };

    void UpdateFireballsAnimation();

    std::vector<std::shared_ptr<Util::GameObject>> GetChildren();
    void AddFireball(std::shared_ptr<Fireball> fireball);
    std::shared_ptr<Fireball> GetNewFireball();
    std::vector<std::shared_ptr<Fireball>> GetFireballs();
    void ResetFireballs();

private:
    std::vector<std::shared_ptr<Fireball>> fireballs = {};
    int fireball_idx = 0;
};

#endif //FIREBALLMANAGER_HPP
