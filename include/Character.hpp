#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "Util/GameObject.hpp"

class Character : public Util::GameObject {
public:
    explicit Character(const std::string& ImagePath);

    Character(const Character&) = delete;

    Character(Character&&) = delete;

    Character& operator=(const Character&) = delete;

    Character& operator=(Character&&) = delete;

    [[nodiscard]] const std::string& GetImagePath() const { return m_ImagePath; }

    [[nodiscard]] const glm::vec2& GetPosition() const { return m_Transform.translation; }

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }

    void SetImage(const std::string& ImagePath);

    void SetPosition(float posx, float posy) {
        m_Transform.translation.x = posx;
        m_Transform.translation.y = posy;
    }

    [[nodiscard]] bool IfCollides(const std::shared_ptr<Character>& other) const {
        (void) other;
        auto po1 = this->GetPosition();
        auto po2 = this->GetPosition();
        auto po3 = this->GetPosition();
        auto po4 = this->GetPosition();
        auto po_other = other->GetPosition();
        po2.x += 80;
        po3.y += 95;
        po4.x += 80;
        po4.y += 95;
        if(po1.x >= po_other.x && po1.x <= po_other.x + 80) {
            if(po1.y >= po_other.y && po1.y <= po_other.y + 100) {
                return true;
            }
        }
        if(po2.x >= po_other.x && po2.x <= po_other.x + 80) {
            if(po2.y >= po_other.y && po2.y <= po_other.y + 100) {
                return true;
            }
        }
        if(po3.x >= po_other.x && po3.x <= po_other.x + 80) {
            if(po3.y >= po_other.y && po3.y <= po_other.y + 100) {
                return true;
            }
        }
        if(po4.x >= po_other.x && po4.x <= po_other.x + 80) {
            if(po4.y >= po_other.y && po4.y <= po_other.y + 100) {
                return true;
            }
        }
        return false;
    }

private:
    void ResetPosition() { m_Transform.translation = {0, 0}; }

    std::string m_ImagePath;
};


#endif //CHARACTER_HPP
