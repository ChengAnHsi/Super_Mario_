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
        if (phase != 2){
            temp->SetImage(RESOURCE_DIR"/Scenery/Overworld/sky.png");
        }else{
            // black image
            temp->SetImage(RESOURCE_DIR"/Scenery/Overworld/under_sky.png");
        }
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

    glm::vec2 GetSize() {
        return m_Drawable->GetSize();
    }
};

#endif //BACKGROUND_IMAGE_HPP
