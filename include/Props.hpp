#include "BackgroundImage.hpp"

class Props : public BackgroundImage {
public:
    Props();

    virtual void AfterCollisionEvents();
private:
    bool iscollision = false;
};