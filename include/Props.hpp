#ifndef PROPS_HPP
#define PROPS_HPP

#include "BackgroundImage.hpp"

class Props : public BackgroundImage {
public:
    enum class PropsState {
        Not_Activated,
        Spawning,
        Active
    };

    Props() = default;

    virtual void SpawnProps() = 0;
    virtual void Update(float dt) = 0;
    virtual void AfterCollisionEvents() = 0;
private:
    bool iscollision = false;
};
#endif //PROPS_HPP