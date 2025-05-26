#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Animation.hpp"

class BackgroundImage : public Util::GameObject {

public:
    BackgroundImage() : GameObject(std::make_unique<Util::Image>(RESOURCE_DIR"/Scenery/logo.png"), -10) {
    }

    void SetPhaseImgIsOverWorld(bool is_overworld) const {
        const auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        if (is_overworld){
            temp->SetImage(RESOURCE_DIR"/Scenery/Overworld/sky.png");
        }else{
            // black image
            temp->SetImage(RESOURCE_DIR"/Scenery/Underworld/sky.png");
        }
    }

    void SetImage(const std::string& path){
        const auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        temp->SetImage(path);
        m_ImagePath = path;
    }

    void SetImage(const std::vector<std::string>& AnimationPaths, int interval, int cooldown){
        m_Drawable = std::make_shared<Util::Animation>(AnimationPaths, true, interval, true, cooldown);
        m_ImagePath = AnimationPaths[0];
    }

    void SetPosition(float x, float y) {
        m_Transform.translation = {x, y};
    }

    [[nodiscard]] glm::vec2 GetPosition() const {
        return m_Transform.translation;
    }

    void SetScale(float width, float height) {
        m_Transform.scale = {width, height};
    }

    [[nodiscard]] glm::vec2 GetScale() const {
        return m_Transform.scale;
    }

    [[nodiscard]] glm::vec2 GetSize() const {
        return m_Drawable->GetSize();
    }

    [[nodiscard]] const std::string& GetImagePath() const {
        return m_ImagePath;
    }

    [[nodiscard]] bool GetVisible() const {
        return m_Visible;
    }

private:
    std::string m_ImagePath = RESOURCE_DIR"/Scenery/logo.png";
};

#endif //BACKGROUND_IMAGE_HPP
