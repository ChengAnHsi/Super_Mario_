//
// Created by 112590007 on 2025/3/14.
//

#ifndef ENEMYMANAGER_HPP
#define ENEMYMANAGER_HPP

#include "../BackgroundImage.hpp"
//#include "../../PTSD/include/Util/Renderer.hpp"
#include "../Enemies/Enemy.hpp"
#include "../Mario.hpp"

class EnemyManager {
public:
    EnemyManager();
    [[nodiscard]] std::vector<std::shared_ptr<Enemy>> GetEnemies() const {
        return m_Enemies;
    }

    std::vector<std::string> imageFiles = {
        RESOURCE_DIR"/Entities/Overworld/flower0.png",//0
        RESOURCE_DIR"/Entities/Overworld/flower1.png",
        RESOURCE_DIR"/Entities/Overworld/goomba0.png",
        RESOURCE_DIR"/Entities/Overworld/goomba1.png",
        RESOURCE_DIR"/Entities/Overworld/goombaDead.png",
        RESOURCE_DIR"/Entities/Underworld/goomba1.png",//5
        RESOURCE_DIR"/Entities/Underworld/goomba2.png",
        RESOURCE_DIR"/Entities/Underworld/goombaDead.png",
        RESOURCE_DIR"/Entities/Underworld/flower0.png",
        RESOURCE_DIR"/Entities/Underworld/flower1.png",
        RESOURCE_DIR"/Entities/Underworld/koopa0.png",//10
        RESOURCE_DIR"/Entities/Underworld/koopa1.png",
        RESOURCE_DIR"/Entities/Underworld/shell0.png",
        RESOURCE_DIR"/Entities/Underworld/shell1.png",
        RESOURCE_DIR"/Entities/shell0.png",
        RESOURCE_DIR"/Entities/shell1.png",//15
        RESOURCE_DIR"/Entities/koopa0.png",
        RESOURCE_DIR"/Entities/koopa1.png",
        RESOURCE_DIR"/Entities/fly_koopa0.png",
        RESOURCE_DIR"/Entities/fly_koopa1.png",
        RESOURCE_DIR"/Entities/unfly_koopa0.png",//20
        RESOURCE_DIR"/Entities/unfly_koopa1.png",
        RESOURCE_DIR"/Entities/unfly_koopa_shell0.png",
        RESOURCE_DIR"/Entities/unfly_koopa_shell1.png",
    };

    std::vector<float> GetX(int phase);
    std::vector<float> GetY(int phase);
    std::vector<int> Getidx(int phase);
    void SetEnemies(std::vector<std::shared_ptr<Enemy>> enemies);
    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
        for (const auto & enemy : m_Enemies) {
            all_obj.emplace_back(enemy);
        }
        return all_obj;
    }
    std::vector<std::shared_ptr<Enemy>> GetEnemies();
    void SetEnemyMoving();
    void SetAllEnemyCollisionBoxs(std::vector<std::shared_ptr<BackgroundImage>> boxes);
    void SetAllEnemyCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks);
    void CheckMarioCollisions(std::shared_ptr<Mario> mario);
private:
    // map 1-1
    std::vector<float> tmp_x = {22,40,51,52.5,80,82,97,98.5,107,114,115.5,124,125.5,128,129.5,174,175.5};
    std::vector<float> tmp_y = {2,2,2,2,10,10,2,2,2.2,2,2,2,2,2,2,2,2};
    std::vector<int> imgidx = {2,2,2,2,2,2,2,2,16,2,2,2,2,2,2,2,2};
    //map 1-2
    std::vector<float> tmp_x2 = {16,17,29,44,45.5,59,62,64,73,76,77.8,99,100.5,102,113,103.5,109.5,115.5,134.9,136.4,146};
    std::vector<float> tmp_y2 = {2,3,2,2.2,2.2,2.2,2,2,10,6,6,2,2,2,2,
                                    5.1,6.1,4.1,5,6,2.2};
    std::vector<int> imgidx2 = {5,5,5,10,10,10,5,5,5,5,5,5,5,5,5,
                                 8,8,8,5,5,20};
    //map 1-3
    std::vector<float> tmp_x3 = {30,44,46,74,79.8,110,114,133};
    std::vector<float> tmp_y3 = {10.2,11,11,10,9,8.2,9,2.2};
    std::vector<int> imgidx3 = {20,2,2,18,2 ,20,18,20};

    std::vector<std::shared_ptr<Enemy>> m_Enemies;
    std::vector<int> m_PositionX;
    std::vector<int> m_PositionY;
};

#endif //ENEMYMANAGER_HPP
