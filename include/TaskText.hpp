#ifndef TASKTEXT_HPP
#define TASKTEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"

class TaskText : public Util::GameObject {
public:
    TaskText() : GameObject(std::make_unique<Util::Text>(RESOURCE_DIR"/Font/SuperMario.ttf", 30, append_string_views(s_text[0], s_Validation), Util::Color::FromName(Util::Colors::WHITE)), 100) {
        m_Transform.translation = {0, 0};
    }

    void SetTxtIdx(const int idx, const int other) {
        auto temp = std::dynamic_pointer_cast<Util::Text>(m_Drawable);

        switch (idx) {
            case 0: {
                temp->SetText(append_string_views(s_text[0], s_Validation));
                break;
            }
            case 1: {
                std::string score = std::to_string(other);
                while (score.length() < 6)
                    score = "0" + score;
                temp->SetText(append_string_views(s_text[1], score));
                break;
            }
            case 2: {
                std::string coin = std::to_string(other);
                if (other < 10)
                    coin = "0" + coin;
                temp->SetText(std::string(s_text[2]) + coin);
                break;
            }
            case 3:{
                std::string phase = " 1-" + std::to_string(other);
                temp->SetText(append_string_views(s_text[3], phase));
                break;
            }
            case 4: {
                std::string time = " " + std::to_string(other);
                temp->SetText(append_string_views(s_text[4], time));
                break;
            }
            case 5: {
                temp->SetText(append_string_views(s_text[5], ""));
                break;
            }
            case 6: {
                std::string score = std::to_string(other);
                while (score.length() < 6)
                    score = "0" + score;
                temp->SetText(std::string(s_text[6]) + score);
                break;
            }
            case 7: {
                std::string live = std::to_string(other);
                if (other < 10)
                    live = "0" + live;
                live = " " + live;
                temp->SetText(append_string_views(std::string(s_text[7]),live));
                break;
            }
            default:
                break;
        }
    }

    void SetPosition(float x, float y) {
        m_Transform.translation = {x, y};
    }

private:
    static std::string append_string_views(std::string_view sv1, std::string_view sv2) {
        return std::string(sv1) + "\n" + std::string(sv2);
    }

    static constexpr std::string_view s_text[8] = {
            "1 PLAYER GAME",
            "MARIO",
            "x",
            "WORLD",
            "TIME",
            "",
            "Top- ",
            "Live"
    };
    static constexpr std::string_view s_Validation = "Press Enter to validate";
};

#endif //TASKTEXT_HPP
