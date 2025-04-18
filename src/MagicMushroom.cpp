#include "MagicMushroom.hpp"
#include "Global.hpp"

void MagicMushroom::AfterCollisionEvents(){

}

void MagicMushroom::SpawnProps() {
    float mushroom_x = this->GetPosition().x;
    float mushroom_y = this->GetPosition().y;

    // each step move block size / 4
    const float step = BLOCK_SIZE / 4.0f;
    // distance = 1.0f, spawn speed
    float remaining_distance = 1.0f;

    while (std::abs(remaining_distance) > 0.0f) {
        float step_distance = (remaining_distance > 0.0f) ? std::min(step, remaining_distance)
                                                          : std::max(-step, remaining_distance);
        mushroom_y += step_distance;
        this->SetPosition(mushroom_x, mushroom_y);
        remaining_distance -= step_distance;
    }
}
