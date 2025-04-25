#ifndef PROPS_HPP
#define PROPS_HPP

#include "BackgroundImage.hpp"
#include "Block.hpp"
#include "CollisionState.hpp"

class Props : public BackgroundImage {
public:
    enum class PropsState {
        Not_Activated,
        Spawning,
        Active
    };

    Props() = default;

    virtual void Move() = 0;
    virtual void Action(float distance) = 0;
    virtual bool GravityAndCollision(float delta) = 0;

    virtual void SpawnProps() = 0;
    virtual void Update(float dt) = 0;
    virtual void AfterCollisionEvents() = 0;

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
private:
    bool iscollision = false;
};
#endif //PROPS_HPP