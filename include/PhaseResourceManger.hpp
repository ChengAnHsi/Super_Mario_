#ifndef PHASE_MANGER_HPP
#define PHASE_MANGER_HPP

#include "Util/GameObject.hpp"
#include "Util/BGM.hpp"
#include "TaskText.hpp"
#include "BlockManager.hpp"
#include "BackgroundImage.hpp"
#include "Mario.hpp"
#include <chrono>

class PhaseResourceManger {
public:
    PhaseResourceManger();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren(bool is_init) const {
        std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
        if (is_init){
            all_obj = {m_MarioText, m_MoneyText, m_WorldText, m_TimeText, m_OtherText};
        }
        for (const auto & img : m_Background) {
            all_obj.emplace_back(img);
        }
        for (const auto & img : m_Tube) {
            all_obj.emplace_back(img);
        }
        return all_obj;
    }
    std::vector<std::string> imagePaths = {
        RESOURCE_DIR"/Scenery/vertical-small-tube.png",
        RESOURCE_DIR"/Scenery/vertical-medium-tube.png",
        RESOURCE_DIR"/Scenery/vertical-large-tube.png"
    };

    void NextPhase(int m_phase, std::shared_ptr<Util::BGM> m_BGM);
    // fixed texts
    void ResetPosition() const;
    void DecreaseTime();

    // setter and getter
    void SetTime(int time);
    int GetTime() const;
    std::vector<std::shared_ptr<BackgroundImage>> GetBackground();
    std::vector<std::shared_ptr<BackgroundImage>> GetTube();

private:
    std::shared_ptr<TaskText> m_MarioText;
    std::shared_ptr<TaskText> m_MoneyText;
    std::shared_ptr<TaskText> m_WorldText;
    std::shared_ptr<TaskText> m_TimeText;
    std::shared_ptr<TaskText> m_OtherText;
    std::vector<std::shared_ptr<BackgroundImage>> m_Background;
    std::vector<std::shared_ptr<BackgroundImage>> m_Tube;
    std::vector<std::shared_ptr<BackgroundImage>> m_Collectibles_Coin;
    //std::shared_ptr<Mario> m_Mario;

    int time = 400;
    std::chrono::steady_clock::time_point last_update;

    // image index 0: small tube 1: medium tube 2: large tube
    // tube position
    std::vector<float> x_offset = {-310, -313, -313};
    std::vector<float> y_offset = {-350, -375, -400};
    std::vector<float> tube_magnification = {3.0f, 3.0f, 3.0f};
    // map 1-1
    std::vector<int> tmp_x = {28, 38, 46, 57, 163, 179};
    std::vector<int> tmp_y = {3, 4, 5, 5, 3, 3};
    std::vector<int> imgidx = {0, 1, 2, 2, 0, 0};
    // map 1-2
    std::vector<int> tmp_x2 = {103, 109, 115};
    std::vector<int> tmp_y2 = {4, 5, 3};
    std::vector<int> imgidx2 = {2, 2, 2};
    // map 1-3 no tube there
};


#endif //PHASE_MANGER_HPP
