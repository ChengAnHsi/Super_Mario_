//
// Created by 112590007 on 2025/3/6.
//

#ifndef GOOMBA_HPP
#define GOOMBA_HPP

#include "Enemy.hpp"
#include "Blocks/Block.hpp"
#include "Mario.hpp"

class Goomba final : public Enemy{
public:
    Goomba() = default;
    ~Goomba() override = default;

    // move function
    void Action(float distance) override;
    void OnUpdate(float delta);
    void Move() override;

    // collision function
    bool AABBCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 goomba_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta) override;
    void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) override;
    void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) override;
    void ClearCollisionBoxes() override;
    void ClearCollisionBlocks() override;
    bool CheckMarioCollision(std::shared_ptr<Mario> mario) override;
    bool CheckEnemyCollision(std::shared_ptr<Enemy> enemy);

    void UpdateAnimation() override;

    void AddEnemies(std::vector<std::shared_ptr<Enemy>> enemies);
    void ClearEnemies();
    void KillGoomba();

    // getter and setter
    void SetLive(int live) override;

private:
    std::vector<std::shared_ptr<Enemy>> other_enemies;

    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/Overworld/goomba0.png",RESOURCE_DIR"/Entities/Overworld/goomba1.png"};
    std::vector<std::string> AnimationUnderWorldRun = {RESOURCE_DIR"/Entities/Underworld/goomba0.png",RESOURCE_DIR"/Entities/Underworld/goomba1.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/Overworld/goombaDead.png"};
    std::vector<std::string> AnimationUnderWorldDead = {RESOURCE_DIR"/Entities/Underworld/goombaDead.png"};

    bool is_set_runanimation = false;
};
#endif //GOOMBA_HPP

