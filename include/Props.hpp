#include "BackgroundImage.hpp"

class Props : public BackgroundImage {
public:
    Props() = default;

    virtual void AfterCollisionEvents();
private:
    bool iscollision = false;
};