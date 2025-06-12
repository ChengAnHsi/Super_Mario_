#ifndef PROPS_HPP
#define PROPS_HPP

#include "BackgroundImage.hpp"
#include "Blocks/Block.hpp"
#include "CollisionState.hpp"
#include "Mario.hpp"

class Props : public BackgroundImage {
public:
    enum class PropsState {
        Not_Activated,
        Spawning,
        Moving,
        Active,
        After_Activated
    };

    Props() = default;
    ~Props() override = default;

    virtual void Move();
    virtual void Action(float distance);
    virtual bool GravityAndCollision(float delta);

    virtual void UpdateAnimation();

    virtual void SpawnProps() = 0;
    virtual void Update(float dt) = 0;
    [[nodiscard]] bool CheckCollisionWithMario(std::shared_ptr<Mario> mario) const;
    virtual void AfterCollisionEvents(std::shared_ptr<Mario> mario);

    void SetState(PropsState props_state);
    [[nodiscard]] PropsState GetState() const;

    void AddCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes);
    void AddCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks);
    void ClearCollisionBoxes();
    void ClearCollisionBlocks();

protected:
    std::vector<std::shared_ptr<BackgroundImage>> collision_boxes;
    std::vector<std::shared_ptr<Block>> collision_blocks;

    bool isFacingRight = true;

    CollisionState X_state = CollisionState::None;
    CollisionState Y_state = CollisionState::None;
    PropsState state = PropsState::Not_Activated;
};
#endif //PROPS_HPP