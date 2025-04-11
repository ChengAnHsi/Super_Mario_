#include "BackgroundImage.hpp"

class Props : public BackgroundImage {
public:
    Props() = default;

    virtual void AfterCollisionEvents() = 0;
private:
    bool iscollision = false;
};