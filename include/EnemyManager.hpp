//
// Created by 112590007 on 2025/3/14.
//

#ifndef ENEMYMANAGER_HPP
#define ENEMYMANAGER_HPP

#include "BackgroundImage.hpp"
#include "Util/Renderer.hpp"

class EnemyManager{
public:
    EnemyManager();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
        for (int i = 0; i < m_Backgrounds.size(); i++) {
            all_obj.emplace_back(m_Backgrounds[i]);
        }
        return all_obj;
    }
    std::vector<std::string> imageFiles = {
        RESOURCE_DIR"/Entities/Overworld/flower0.png",
        RESOURCE_DIR"/Entities/Overworld/flower1.png",
        RESOURCE_DIR"/Entities/Overworld/goomba0.png",
        RESOURCE_DIR"/Entities/Overworld/goomba1.png",
        RESOURCE_DIR"/Entities/Overworld/goomba2.png",
        RESOURCE_DIR"/Entities/Underworld/goomba0.png",
        RESOURCE_DIR"/Entities/Underworld/goomba1.png",
        RESOURCE_DIR"/Entities/Underworld/goomba2.png",
        RESOURCE_DIR"/Entities/Underworld/goombaDead.png",
        RESOURCE_DIR"/Entities/Underworld/flower0.png",
        RESOURCE_DIR"/Entities/Underworld/flower1.png",
        RESOURCE_DIR"/Entities/Underworld/koopa0.png",
        RESOURCE_DIR"/Entities/Underworld/koopa1.png",
        RESOURCE_DIR"/Entities/Underworld/shell0.png",
        RESOURCE_DIR"/Entities/Underworld/shell1.png",
        RESOURCE_DIR"/Entities/shell0.png",
        RESOURCE_DIR"/Entities/shell1.png",
        RESOURCE_DIR"/Entities/koopa0.png",
        RESOURCE_DIR"/Entities/koopa1.png",
        RESOURCE_DIR"/Entities/fly_koopa0.png",
        RESOURCE_DIR"/Entities/fly_koopa1.png",
        RESOURCE_DIR"/Entities/unfly_koopa0.png",
        RESOURCE_DIR"/Entities/unfly_koopa1.png",
        RESOURCE_DIR"/Entities/unfly_koopa_shell0.png",
        RESOURCE_DIR"/Entities/unfly_koopa_shell1.png",

    };
    std::vector<float> GetX(int phase);
    std::vector<float> GetY(int phase);
    std::vector<int> Getidx(int phase);
    void SetBackground(std::vector<std::shared_ptr<BackgroundImage>> backgrounds);
    std::vector<std::shared_ptr<BackgroundImage>> GetBackground();
    std::vector<int> GetPosX();

private:
    // 0-1: block 2-3: floor 4-5: immovable block 6-11: mistery block
    // map 1-1
    std::vector<float> tmp_x = {22,40,51,52.5,80,82};
    std::vector<float> tmp_y = {2,2,2,2,10,10};
    std::vector<int> imgidx = {3,3,3,3,3,3};
    //map 1-2
    std::vector<float> tmp_x2 = {};
    std::vector<float> tmp_y2 = {};
    std::vector<int> imgidx2 = {};
    //map 1-3
    std::vector<float> tmp_x3 = {};
    std::vector<float> tmp_y3 = {};
    std::vector<int> imgidx3 = {};

    std::vector<std::shared_ptr<BackgroundImage>> m_Backgrounds;
    std::vector<int> m_PositionX;
    std::vector<int> m_PositionY;
};

#endif //ENEMYMANAGER_HPP
