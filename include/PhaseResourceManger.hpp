#ifndef PHASE_MANGER_HPP
#define PHASE_MANGER_HPP

#include "Util/GameObject.hpp"
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

    void NextPhase(int m_phase);
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
    std::shared_ptr<Mario> m_Mario;

    int time = 400;
    std::chrono::steady_clock::time_point last_update;
};


#endif //PHASE_MANGER_HPP
