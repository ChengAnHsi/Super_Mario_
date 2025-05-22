//
// Created by 112590007 on 2025/5/20.
//

#include "Manager/FireballManager.hpp"

void FireballManager::UpdateFireballsAnimation() {
    if(fireballs.empty() == false) {
        for (auto& fireball : fireballs) {
            if(fireball->GetVisible() == false) {
                // is collision or disappear
                continue;
            }
            fireball->Update(1.0f);
        }
    }

}

std::vector<std::shared_ptr<Util::GameObject>> FireballManager::GetChildren() {
    std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
    for (auto fireball : fireballs) {
        all_obj.push_back(std::static_pointer_cast<Util::GameObject>(fireball));
    }
    return all_obj;
}

void FireballManager::AddFireball(std::shared_ptr<Fireball> fireball) {
    fireball->SetImage({imagePaths[0], imagePaths[1], imagePaths[2], imagePaths[3]}, 500, 0);
    fireball->SetScale(3.5f, 3.5f);
    fireballs.push_back(fireball);
}

std::shared_ptr<Fireball> FireballManager::GetNewFireball() {
    if (fireball_idx < fireballs.size()) {
        fireball_idx += 1;
        return fireballs[fireball_idx - 1];
    }
    return nullptr;
}

std::vector<std::shared_ptr<Fireball>> FireballManager::GetFireballs() {
    return fireballs;
}

void FireballManager::ResetFireballs() {
    fireball_idx = 0;
    fireballs.clear();
}