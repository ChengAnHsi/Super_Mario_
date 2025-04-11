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

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

    [[nodiscard]] bool IfCollides(const std::shared_ptr<Character>& other) const {
        (void) other;
        /**float width = 60.0f;
        float height = 60.0f;

        auto posA = m_Transform.translation;
        auto posB = other->GetPosition();
        return (posA.x < posB.x + width &&
            posA.x + width > posB.x &&
            posA.y < posB.y + height &&
            posA.y + height > posB.y);**/
        //std::cout << this->GetPosition().x << std::endl;
        //std::cout << this->GetPosition().y << std::endl;
        auto po1 = this->GetPosition();
        auto po2 = this->GetPosition();
        auto po3 = this->GetPosition();
        auto po4 = this->GetPosition();
        auto po_other = other->GetPosition();
        po2.x += 80;
        po3.y += 95;
        po4.x += 80;
        po4.y += 95;
        if(po1.x >= po_other.x and po1.x <= po_other.x + 80) {
            if(po1.y >= po_other.y and po1.y <= po_other.y + 100) {
                return true;
            }
        }
        if(po2.x >= po_other.x and po2.x <= po_other.x + 80) {
            if(po2.y >= po_other.y and po2.y <= po_other.y + 100) {
                return true;
            }
        }
        if(po3.x >= po_other.x and po3.x <= po_other.x + 80) {
            if(po3.y >= po_other.y and po3.y <= po_other.y + 100) {
                return true;
            }
        }
        if(po4.x >= po_other.x and po4.x <= po_other.x + 80) {
            if(po4.y >= po_other.y and po4.y <= po_other.y + 100) {
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
