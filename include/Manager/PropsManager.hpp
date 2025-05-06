//
// Created by 112590007 on 2025/3/14.
//

#ifndef PROPSMANAGER_HPP
#define PROPSMANAGER_HPP

#include "Props/Props.hpp"
#include "Mario.hpp"

class PropsManager {
public:
    PropsManager() = default;

    std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const;

    std::vector<std::string> propsImagePaths = {
        RESOURCE_DIR"/Collectibles/oneupmushroom.png",
        RESOURCE_DIR"/Collectibles/magicmushroom.png",
        RESOURCE_DIR"/Collectibles/superstar0.png",
        RESOURCE_DIR"/Collectibles/superstar1.png",
        RESOURCE_DIR"/Collectibles/superstar2.png",
        RESOURCE_DIR"/Collectibles/superstar3.png",//5
        RESOURCE_DIR"/Collectibles/superstar4.png",
        RESOURCE_DIR"/Collectibles/superstar5.png",
        RESOURCE_DIR"/Collectibles/Overworld/fire_flower0.png",
        RESOURCE_DIR"/Collectibles/Overworld/fire_flower1.png",
        RESOURCE_DIR"/Collectibles/Overworld/fire_flower2.png",//10
        RESOURCE_DIR"/Collectibles/Overworld/fire_flower3.png",
        RESOURCE_DIR"/Collectibles/Underworld/fire_flower0.png",
        RESOURCE_DIR"/Collectibles/Underworld/fire_flower1.png",
        RESOURCE_DIR"/Collectibles/Underworld/fire_flower2.png",
        RESOURCE_DIR"/Collectibles/Underworld/fire_flower3.png",//15
        RESOURCE_DIR"/Collectibles/Overworld/coin0.png",
        RESOURCE_DIR"/Collectibles/Overworld/coin1.png",
        RESOURCE_DIR"/Collectibles/Overworld/coin2.png",
        RESOURCE_DIR"/Collectibles/Overworld/coin3.png"
    };

    void UpdatePropsAnimation() const;
    void CheckCollisionWithMario(std::shared_ptr<Mario> mario);
    void AddAllPropsCollisionBlocks(std::vector<std::shared_ptr<Block>> blocks) const;
    void AddAllPropsCollisionBoxes(std::vector<std::shared_ptr<BackgroundImage>> boxes) const;

    std::vector<int> GetpropsX(int phase);
    std::vector<int> GetpropsY(int phase);
    std::vector<int> Getpropsidx(int phase);
    void SetProps(std::vector<std::shared_ptr<Props>> props);
    std::vector<std::shared_ptr<BackgroundImage>> GetBackground();
    std::vector<std::shared_ptr<Props>> GetProps();

private:
    // 0: oneupmushroom, 1: magicmushroom, 2-7: superstar, 8-15 fireflower
    // map 1-1
    std::vector<int> props_tmp_x = {22,94,109,129,130,
        64,
        16,21,23,78,94,101,106,109,112,170};
    std::vector<int> props_tmp_y = {9,9,9,9,9,
        6,
        5,5,5,5,5,5,5,5,5,5};
    std::vector<int> props_imgidx = {16,16,1,16,16,
        0,
        16,1,16,1,16,2,16,16,16,16};
    //map 1-2
    std::vector<int> props_tmp_x2 = {89,
        46,
        29,69,73,150,
        10,11,12,13,14};
    std::vector<int> props_tmp_y2 = {12,
        7,
        6,6,6,6,
        5,5,5,5,5};
    std::vector<int> props_imgidx2 = {0,
        2,
        16,1,16,1,
        1,16,16,16,16};
    // TODO two props in a block(flower and mushroom): 69,6 150,6 10,5
    //map 1-3
    std::vector<int> props_tmp_x3 = {59};
    std::vector<int> props_tmp_y3 = {4};
    std::vector<int> props_imgidx3 = {1};
    // TODO: solve 2 props in same block
    std::vector<std::shared_ptr<Props>> m_props;
};

#endif //PROPSMANAGER_HPP
