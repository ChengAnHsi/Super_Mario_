#ifndef FLYKOOPA_HPP
#define FLYKOOPA_HPP

#include "Enemy.hpp"
#include "Blocks/Block.hpp"
#include "CollisionState.hpp"
#include "Koopa.hpp"

class FlyKoopa : public Koopa {
public:
    FlyKoopa();

    // Move function
    void Action(float distance) override;
    void OnUpdate(float delta);
    void Move() override;

    // Collision function
    bool AABBCollides(glm::vec2 flykoopa_pos, std::shared_ptr<BackgroundImage> box);
    bool CCDDCollides(glm::vec2 flykoopa_pos, std::shared_ptr<BackgroundImage> box);
    bool GravityAndCollision(float delta) override;
    bool DetectHole(float next_x, float y);
    bool CheckMarioCollision(std::shared_ptr<Mario> mario) override;
    void UpdateAnimation() override;
    void UpdateYMovementRange();

    // Custom flight range setters
    void SetFlightHeight(float height);
    void SetFlightRange(float min_y, float max_y);

    // State transition functions
    void ConvertToUnfly();
    void ConvertToShell();
    void ActivateShell(bool move_right);

    // Getter and setter
    void SetLive(int live);
    [[nodiscard]] int GetLive() const;
    [[nodiscard]] int GetScore() const { return score; }
    [[nodiscard]] bool IsFlying() const { return isFlying; }
    [[nodiscard]] bool IsShell() const { return isShell; }
    [[nodiscard]] bool IsMovingShell() const { return isMovingShell; }
    bool IsFly() const;
    void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) override;
    void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) override;
    void ClearCollisionBoxes() override;
    void ClearCollisionBlocks() override;

private:
    enum class AnimationState {
        Flying,
        Walking,
        Shell,
        MovingShell
    };

    int live = 1;
    int score = 100;
    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<Block>> collision_blocks;

    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;
    bool isFacingRight = false;
    bool isFacingUp = true;
    bool isFlying = true;
    bool isShell = false;
    bool isMovingShell = false;
    AnimationState current_animation = AnimationState::Flying;
    float delta_time = 1.0f;
    float velocityY = 0.0f;
    float velocityX = 0.0f;
    float GRAVITY = -300.0f;
    float shell_timer = 0.0f;
    float death_timer = 0.0f;
    const float DEATH_JUMP_DURATION = 120.0f;
    const float DEATH_ANIMATION_TIME = 80.0f;
    const float DEATH_JUMP_VELOCITY = 300.0f;

    float min_y_position = -30.0f;
    float max_y_position = 30.0f;
    float default_flight_height = 3.0f;
    float custom_flight_height = -1.0f;

    std::vector<std::string> AnimationFly = {RESOURCE_DIR"/Entities/fly_koopa0.png", RESOURCE_DIR"/Entities/fly_koopa1.png"};
    std::vector<std::string> AnimationRun = {RESOURCE_DIR"/Entities/unfly_koopa0.png", RESOURCE_DIR"/Entities/unfly_koopa1.png"};
    std::vector<std::string> AnimationShell = {RESOURCE_DIR"/Entities/unfly_koopa_shell0.png", RESOURCE_DIR"/Entities/unfly_koopa_shell1.png"};
    std::vector<std::string> AnimationMovingShell = {RESOURCE_DIR"/Entities/unfly_koopa_shell1.png"};

    bool is_dead = false;
};
#endif //FLYKOOPA_HPP