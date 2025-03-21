#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class BackgroundImage : public Util::GameObject {

public:
    BackgroundImage() : GameObject(std::make_unique<Util::Image>(RESOURCE_DIR"/Scenery/logo.png"), -10) {
    }

    void NextPhase(const int phase) {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        temp->SetImage(RESOURCE_DIR"/Scenery/Overworld/sky.png");
    }

    void ChangeImg(std::string path) {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        temp->SetImage(path);
    }

    void SetPosition(float x, float y) {
        m_Transform.translation = {x, y};
    }

    void SetSize(float width, float height) {
        m_Transform.scale = {width, height};
    }

private:
    /**inline std::string ImagePath(const int phase) {
        // return RESOURCE_DIR"/Scenery/World1_Area" + std::to_string(phase) + ".png";
        //return RESOURCE_DIR"/Temp/Image/1-" + std::to_string(phase) + "_Grid.png";
        // return RESOURCE_DIR"/Scenery/1_1MAP.png";
        return RESOURCE_DIR"/Scenery/Overworld/sky.png";
    }**/
};

#endif //BACKGROUND_IMAGE_HPP
