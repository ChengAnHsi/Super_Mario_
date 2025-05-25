//
// Created by 112590007 on 2025/3/6.
//

#ifndef KOOPA_HPP
#define KOOPA_HPP

#include "Enemy.hpp"
#include "Blocks/Block.hpp"
#include "CollisionState.hpp"
#include "Mario.hpp"
class Koopa : public Enemy{
public:
    Koopa() = default;

    // move function
    void Action(float distance) override;
    void OnUpdate(float delta);
    void Move() override;

    // collision function
    bool AABBCollides(glm::vec2 Koopa_pos, std::shared_ptr<BackgroundImage> box);
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
    void TurnToShell();
    bool KickShell(std::shared_ptr<Mario> mario);
    void KillEnemy(std::shared_ptr<Enemy> enemy);
    void BounceOffShell(std::shared_ptr<Koopa> other_koopa);

    // getter and setter
    void SetLive(int live) override;
    [[nodiscard]] int GetLive() const;

private:
    bool TouchGrand = false;
    std::vector<std::shared_ptr<Enemy>> other_enemies;
    float shell_timer = 0.0f;  // Timer for shell state management

    bool is_shell = false;  // Flag to track if Koopa is in shell state
    bool shell_is_moving = false;  // Flag to track if shell is moving
    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/koopa0.png",RESOURCE_DIR"/Entities/koopa1.png"};
    std::vector<std::string> AnimationUnderWorldRun = {RESOURCE_DIR"/Entities/Underworld/koopa0.png",RESOURCE_DIR"/Entities/Underworld/koopa1.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/shell1.png"};
    std::vector<std::string> AnimationUnderWorldDead = {RESOURCE_DIR"/Entities/Underworld/shell1.png"};

    bool is_set_runanimation = false;
};
#endif //KOOPA_HPP