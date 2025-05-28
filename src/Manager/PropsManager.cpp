//
// Created by 112590007 on 2025/4/11.
//

#include "Manager/PropsManager.hpp"
#include "BackgroundImage.hpp"
#include "App.hpp"

std::vector<std::shared_ptr<Util::GameObject>> PropsManager::GetChildren() const {
    std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
    for (const auto & m_prop : m_props) {
        all_obj.emplace_back(m_prop);
    }
    return all_obj;
}

void PropsManager::UpdatePropsAnimation() const {
    for (const auto & prop : m_props) {
        prop->Update(1.0f);
    }
}

void PropsManager::CheckCollisionWithMario(std::shared_ptr<Mario> mario) {
    if (mario->GetDying() || mario->GetDead()) return;
    for (auto& prop : m_props) {
        if (prop->GetState() == Props::PropsState::Moving && prop->CheckCollisionWithMario(mario)) {
            prop->SetState(Props::PropsState::Active);
            prop->AfterCollisionEvents(mario);
        }
    }
}

void PropsManager::AddAllPropsCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) const {
    for (const auto & prop : m_props) {
        prop->AddCollisionBlocks(blocks);
    }
}

void PropsManager::AddAllPropsCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) const {
    for (const auto & prop : m_props) {
        prop->AddCollisionBoxes(boxes);
    }
}

std::vector<int> PropsManager::GetpropsX(int phase){
    switch (phase)
    {
        case 1:
            return props_tmp_x;
        case 2:
            return props_tmp_x2;
        case 3:
            return props_tmp_x3;
        default:
            return props_tmp_x;
    }
}

std::vector<int> PropsManager::GetpropsY(int phase){
    switch (phase)
    {
        case 1:
            return props_tmp_y;
        case 2:
            return props_tmp_y2;
        case 3:
            return props_tmp_y3;
        default:
            return props_tmp_y;
    }
}

std::vector<int> PropsManager::Getpropsidx(int phase){
    switch (phase)
    {
        case 1:
            return props_imgidx;
        case 2:
            return props_imgidx2;
        case 3:
            return props_imgidx3;
        default:
            return props_imgidx;
    }
}

void PropsManager::SetProps(std::vector<std::shared_ptr<Props>> props){
    this->m_props.clear();
    this->m_props = props;
}

std::vector<std::shared_ptr<BackgroundImage>> PropsManager::GetBackground() {
    std::vector<std::shared_ptr<BackgroundImage>> all_prop;
    for (const auto & m_prop : m_props) {
        all_prop.push_back(std::static_pointer_cast<BackgroundImage>(m_prop));
    }
    return all_prop;
}

std::vector<std::shared_ptr<Props>> PropsManager::GetProps(){
    return m_props;
}