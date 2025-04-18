// FlyKoopa.hpp
#ifndef FLYKOOPA_HPP
#define FLYKOOPA_HPP

#include "Enemy.hpp"
#include "Block.hpp"
#include "CollisionState.hpp"

class FlyKoopa : public Enemy{
public:
    FlyKoopa();

    // move function
    void Action(float distance) override;
    void OnUpdate(float delta);
    void Move() override;

    // collision function
    bool AABBCollides(glm::vec2 flykoopa_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 flykoopa_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta) override;
    bool DetectHole(float next_x, float y); // 檢測前方是否有坑洞

    void UpdateAnimation() override;
    void UpdateYMovementRange();

    // Custom flight range setters
    void SetFlightHeight(float height);
    void SetFlightRange(float min_y, float max_y);

    // State transition functions
    void ConvertToUnfly(); // 從飛行轉為地面行走
    void ConvertToShell(); // 從地面行走轉為龜殼
    void ActivateShell(bool move_right); // 激活移動的龜殼

    // getter and setter
    void SetPosition(float x, float y);
    void SetLive(int live);
    [[nodiscard]] int GetLive() const;
    [[nodiscard]] int GetScore() const { return score; }
    [[nodiscard]] bool IsFlying() const { return isFlying; }
    [[nodiscard]] bool IsShell() const { return isShell; }
    [[nodiscard]] bool IsMovingShell() const { return isMovingShell; }

    void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) override;
    void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) override;
    void ClearCollisionBoxes() override;
    void ClearCollisionBlocks() override;
    void SetMoveVelocity(float speed);
private:
    int live = 1;
    // 被擊倒的分數
    int score = 100;

    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<Block>> collision_blocks;

    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;

    bool isFacingRight = false;
    bool isFacingUp = true;      // For vertical movement
    bool isFlying = true;        // Track flying state
    bool isShell = false;        // Track shell state
    bool isMovingShell = false;  // Track if shell is moving

    float delta_time = 1.0f;
    float velocityY = 0.0f;
    float velocityX = 0.0f;
    float GRAVITY = -300.0f;
    float shell_move_velocity = 8.0f; // 龜殼移動速度

    // Y movement range for flying state
    float min_y_position = 0.0f;
    float max_y_position = 0.0f;
    float default_flight_height = 3.0f; // Default flight height in blocks
    float custom_flight_height = -1.0f; // -1 means use default calculation

    // Animation paths
    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/fly_koopa0.png",RESOURCE_DIR"/Entities/fly_koopa1.png"};
    std::vector<std::string> AnimationDead = {RESOURCE_DIR"/Entities/unfly_koopa0.png",RESOURCE_DIR"/Entities/unfly_koopa1.png"};
    std::vector<std::string> AnimationShell = {RESOURCE_DIR"/Entities/unfly_koopa_shell0.png",RESOURCE_DIR"/Entities/unfly_koopa_shell1.png"};
    std::vector<std::string> AnimationMovingShell = {RESOURCE_DIR"/Entities/unfly_koopa_shell0.png",RESOURCE_DIR"/Entities/unfly_koopa_shell1.png"};

    bool is_dead = false;
    bool is_set_runanimation = false;
};
#endif //FLYKOOPA_HPP