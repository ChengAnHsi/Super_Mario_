#ifndef PHASE_MANGER_HPP
#define PHASE_MANGER_HPP

#include "Util/GameObject.hpp"
#include "TaskText.hpp"
#include "Character.hpp"
#include "BackgroundImage.hpp"
#include "Mario.hpp"

class PhaseResourceManger {
public:
    PhaseResourceManger();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> all_obj = {m_MarioText, m_MoneyText, m_WorldText, m_TimeText, m_OtherText};
        for (const auto & i : m_Background) {
            all_obj.emplace_back(i);
        }
        return all_obj;
    }

    void NextPhase();
    void ResetPosition() const;
    std::shared_ptr<BackgroundImage> GetBackground(int idx);

private:
    std::shared_ptr<TaskText> m_MarioText;
    std::shared_ptr<TaskText> m_MoneyText;
    std::shared_ptr<TaskText> m_WorldText;
    std::shared_ptr<TaskText> m_TimeText;
    std::shared_ptr<TaskText> m_OtherText;
    std::vector<std::shared_ptr<BackgroundImage>> m_Background;
    std::shared_ptr<Mario> m_Mario;

    int m_Phase = 1;
    int time = 0;
};


#endif //PHASE_MANGER_HPP
