//
// Created by 112590007 on 2025/3/14.
//

#ifndef PROPSMANAGER_HPP
#define PROPSMANAGER_HPP

#include "Props.hpp"
#include "Util/Renderer.hpp"

class PropsManager {
public:
    PropsManager();

    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const {
        std::vector<std::shared_ptr<Util::GameObject>> all_obj = {};
        for (int i = 0; i < m_props.size(); i++) {
            all_obj.emplace_back(m_props[i]);
        }
        return all_obj;
    }

    std::vector<std::string> imagePaths = {
        RESOURCE_DIR"Collectibles/oneupmushroom.png",
        RESOURCE_DIR"Collectibles/magicmushroom.png",
        RESOURCE_DIR"Collectibles/superstar0.png",
        RESOURCE_DIR"Collectibles/superstar1.png",
        RESOURCE_DIR"Collectibles/superstar2.png",
        RESOURCE_DIR"Collectibles/superstar3.png",//5
        RESOURCE_DIR"Collectibles/superstar4.png",
        RESOURCE_DIR"Collectibles/superstar5.png",
        RESOURCE_DIR"Collectibles/Overworld/fire_flower0.png",
        RESOURCE_DIR"Collectibles/Overworld/fire_flower1.png",
        RESOURCE_DIR"Collectibles/Overworld/fire_flower2.png",//10
        RESOURCE_DIR"Collectibles/Overworld/fire_flower3.png",
        RESOURCE_DIR"Collectibles/Underworld/fire_flower0.png",
        RESOURCE_DIR"Collectibles/Underworld/fire_flower1.png",
        RESOURCE_DIR"Collectibles/Underworld/fire_flower2.png",
        RESOURCE_DIR"Collectibles/Underworld/fire_flower3.png",//15

    };
    std::vector<int> GetX(int phase);
    std::vector<int> GetY(int phase);
    std::vector<int> Getidx(int phase);
    void SetProps(std::vector<std::shared_ptr<Props>> props);
    std::vector<std::shared_ptr<BackgroundImage>> GetBackground();
    std::vector<std::shared_ptr<Props>> GetProps();
    std::vector<float> GetPosX();

private:
    // 0-1: block, 2-3: floor, ...
    // map 1-1
    std::vector<int> tmp_x = {21,64,78,101,109};
    std::vector<int> tmp_y = {5,6,5,5,9};
    std::vector<int> imgidx = {1,0,1,2};
    //map 1-2
    std::vector<int> tmp_x2 = {10,10,46,69,69,88,150,150};
    std::vector<int> tmp_y2 = {5,5,7,6,6,12,6,6};
    std::vector<int> imgidx2 = {1,8,2,1,8,0,1,8};
    //map 1-3
    std::vector<int> tmp_x3 = {69,69};
    std::vector<int> tmp_y3 = {5,5};
    std::vector<int> imgidx3 = {1,8};
    std::vector<std::shared_ptr<Props>> m_props;
    std::vector<float> m_PositionX;
    std::vector<float> m_PositionY;
};

#endif //PROPSMANAGER_HPP
