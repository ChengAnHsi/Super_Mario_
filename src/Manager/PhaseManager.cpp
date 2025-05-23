#include "Util/Logger.hpp"

#include "AnimatedImage.hpp"
#include "App.hpp"
#include "Global.hpp"

#include "Manager/PhaseResourceManger.hpp"

PhaseResourceManger::PhaseResourceManger() {
    last_update = std::chrono::steady_clock::now();

    m_ScoreText = std::make_shared<TaskText>();
    m_ScoreText->SetPosition(-250, 300);
    m_ScoreText->SetTxtIdx(1, 0);

    m_MoneyText = std::make_shared<TaskText>();
    m_MoneyText->SetPosition(-70, 285);
    m_MoneyText->SetTxtIdx(2, 0);

    m_WorldText = std::make_shared<TaskText>();
    m_WorldText->SetPosition(100, 300);
    m_WorldText->SetTxtIdx(3, 1);

    m_TimeText = std::make_shared<TaskText>();
    m_TimeText->SetPosition(300, 300);
    m_TimeText->SetTxtIdx(4, 400);

    m_OtherText = std::make_shared<TaskText>();
    m_OtherText->SetPosition(0, -200);
    m_OtherText->SetTxtIdx(0, 0);

    std::vector<std::string> imageFiles = {
        RESOURCE_DIR"/Scenery/Overworld/bush1.png",
        RESOURCE_DIR"/Scenery/Overworld/bush2.png",
        RESOURCE_DIR"/Scenery/Overworld/cloud1.png",
        RESOURCE_DIR"/Scenery/Overworld/cloud2.png",
        RESOURCE_DIR"/Scenery/Overworld/mountain1.png",
        RESOURCE_DIR"/Scenery/Overworld/mountain2.png",
        RESOURCE_DIR"/Scenery/castle.png",
        RESOURCE_DIR"/Scenery/flag-mast.png"
    };

    // initial screen: background image setting
    // [0]: logo
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->SetPosition(0, 0);
    m_Background.back()->SetScale(3.0f,3.0f);
    // The background z-index is placed at the bottom
    m_Background.back()->SetZIndex(-50);
    // [1]: start mountain(left)
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->SetImage(imageFiles[5]);
    m_Background.back()->SetPosition(-240.0f, 2.5 * BLOCK_SIZE - 315.0f);
    m_Background.back()->SetScale(1.5f,1.5f);
    // [2]: start bush(right)
    m_Background.push_back(std::make_shared<BackgroundImage>());
    m_Background.back()->SetImage(imageFiles[0]);
    m_Background.back()->SetPosition(11 * BLOCK_SIZE - 360.0f, 2 * BLOCK_SIZE- 325.0f);
    m_Background.back()->SetScale(1.3f,1.3f);
}

bool PhaseResourceManger::CheckMarioCollisionFlag(std::shared_ptr<Mario> mario, int m_phase) {
    if (mario->GetPull() || mario->GetBackToCastle()) return false;

    // todo after finish nextphase 1-2 and 1-3 remove it
    if (m_phase != 1) return false;

    // m_Background[2][3]: flag
    glm::vec2 a = m_Background[2]->GetPosition();
    glm::vec2 prop_size = m_Background[2]->GetSize();
    prop_size *= FIREBALL_MAGNIFICATION;

    glm::vec2 b = mario->GetPosition();
    glm::vec2 b_size = mario->GetSize();

    b_size.x *= MARIO_MAGNIFICATION;
    b_size.y *= MARIO_MAGNIFICATION;
    if(b_size.x < 0) b_size.x *= -1;
    if(b_size.y < 0) b_size.y *= -1;

    float aleft = a.x - prop_size.x / 2;
    float aright = a.x + prop_size.x / 2;
    float atop = a.y + prop_size.y / 2;
    float abottom = a.y - prop_size.y / 2;

    float bleft = b.x - b_size.x / 2;
    float bright = b.x + b_size.x / 2;
    float btop = b.y + b_size.y / 2;
    float bbottom = b.y - b_size.y / 2;

    bool collisionX = (aleft < bright) && (aright > bleft);
    bool collisionY = (abottom < btop) && (atop > bbottom);

    if (!(collisionX && collisionY)) {
        return false;
    }
    mario->SetPull(true);
    mario->SetPosition({m_Background[2]->GetPosition().x - (mario->GetSize().x * MARIO_MAGNIFICATION) / 2, mario->GetPosition().y});
    return true;
}

void PhaseResourceManger::NextPhase(int m_Phase) {
    LOG_DEBUG("Passed! Next phase: {}", m_Phase);
    if (m_Phase == 1){
        m_Background.clear();
        // [0]: blue background image
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->NextPhase(m_Phase);
        m_Background.back()->SetScale(80.0f,7.0f);
        m_Background.back()->SetZIndex(-50);
        // [1]: castle is visible
        // map 1-1 castle
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->SetImage(RESOURCE_DIR"/Scenery/castle.png");
        m_Background.back()->SetPosition(204 * BLOCK_SIZE + BACKGROUND_X_OFFSET, 4 * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        m_Background.back()->SetScale(3.0f, 3.0f);
        // [2][3]: flag set
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->SetImage(RESOURCE_DIR"/Scenery/flag-mast.png");
        m_Background.back()->SetPosition(198 * BLOCK_SIZE + BACKGROUND_X_OFFSET, 8 * BLOCK_SIZE - 385.0f);
        m_Background.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);

        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->SetImage(RESOURCE_DIR"/Scenery/final-flag.png");
        m_Background.back()->SetPosition(197.5 * BLOCK_SIZE + BACKGROUND_X_OFFSET, 11 * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        m_Background.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        // TODO [4]> mountain, cloud... set invisible use app->nextphase

        // TODO use function fix pos(not finish)
        // m_Tube.back()->SetPosition(28 * BLOCK_SIZE + BACKGROUND_X_OFFSET + m_Tube.back()->GetSize().x / 2, 2.5 * BLOCK_SIZE + BACKGROUND_Y_OFFSET - m_Tube.back()->GetSize().y / 2);
        // tube init(map 1-1)
        m_CollisionBoxes.clear();
        for (size_t i = 0; i < collisionboxes_imgidx.size(); i++) {
            m_CollisionBoxes.push_back(std::make_shared<BackgroundImage>());
            m_CollisionBoxes.back()->SetImage(imagePaths[collisionboxes_imgidx[i]]);
            m_CollisionBoxes.back()->SetPosition(collisionboxes_x[i] * BLOCK_SIZE + tubex_offset[collisionboxes_imgidx[i]], collisionboxes_y[i] * BLOCK_SIZE  + tubey_offset[collisionboxes_imgidx[i]]);
            m_CollisionBoxes.back()->SetScale(tube_magnification[collisionboxes_imgidx[i]], tube_magnification[collisionboxes_imgidx[i]]);
        }
    }
    if (m_Phase == 2){
        m_Background.clear();
        // [0]: black background image
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->NextPhase(m_Phase);
        m_Background.back()->SetScale(80.0f,7.0f);
        m_Background.back()->SetZIndex(-50);
        // [1]: castle???
        // [2][3]: flag set???

        // tube & lifting platform init(map 1-2)
        m_CollisionBoxes.clear();
        for (size_t i = 0; i < collisionboxes_x2.size(); i++) {
            m_CollisionBoxes.push_back(std::make_shared<BackgroundImage>());
            m_CollisionBoxes.back()->SetImage(imagePaths[collisionboxes_imgidx2[i]]);
            m_CollisionBoxes.back()->SetPosition(collisionboxes_x2[i] * BLOCK_SIZE + tubex_offset[collisionboxes_imgidx2[i]], collisionboxes_y2[i] * BLOCK_SIZE  + tubey_offset[collisionboxes_imgidx2[i]]);
            m_CollisionBoxes.back()->SetScale(tube_magnification[collisionboxes_imgidx2[i]], tube_magnification[collisionboxes_imgidx2[i]]);
        }
        // top tube
        m_CollisionBoxes.push_back(std::make_shared<BackgroundImage>());
        m_CollisionBoxes.back()->SetImage(RESOURCE_DIR"/Scenery/vertical-medium-tube.png");
        m_CollisionBoxes.back()->SetPosition(2 * BLOCK_SIZE + tubex_offset[1], 14 * BLOCK_SIZE + tubey_offset[1]);
        m_CollisionBoxes.back()->SetScale(tube_magnification[1], -tube_magnification[1]);
        // end horizontal tube
        m_CollisionBoxes.push_back(std::make_shared<BackgroundImage>());
        m_CollisionBoxes.back()->SetImage(RESOURCE_DIR"/Scenery/horizontal-final-tube.png");
        m_CollisionBoxes.back()->SetPosition(167 * BLOCK_SIZE + tubex_offset[1], 6 * BLOCK_SIZE + tubey_offset[0]);
        m_CollisionBoxes.back()->SetScale(tube_magnification[0], tube_magnification[0]);
        // end vertical tube
        m_CollisionBoxes.push_back(std::make_shared<BackgroundImage>());
        m_CollisionBoxes.back()->SetImage(RESOURCE_DIR"/Scenery/vertical-xlarge-tube.png");
        m_CollisionBoxes.back()->SetPosition(168 * BLOCK_SIZE + tubex_offset[0], 11 * BLOCK_SIZE + tubey_offset[2]);
        m_CollisionBoxes.back()->SetScale(tube_magnification[0], tube_magnification[0]);

        // coin init(map 1-2)
        m_CollectibleCoins.clear();
        for (size_t i = 0; i < coin_imgidx2.size(); i++) {
            m_CollectibleCoins.push_back(std::make_shared<AnimatedImage>());
            m_CollectibleCoins.back()->SetImage({imagePaths[coin_imgidx2[i]],imagePaths[coin_imgidx2[i] + 1],imagePaths[coin_imgidx2[i] + 1]},1000,0);
            m_CollectibleCoins.back()->SetPosition(coin_x2[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET, coin_y2[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
            m_CollectibleCoins.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        }
    }
    if (m_Phase == 3) {
        m_Background.clear();
        // [0]: black background image
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->NextPhase(m_Phase);
        m_Background.back()->SetScale(80.0f,7.0f);
        m_Background.back()->SetZIndex(-50);
        // [1]: castle is visible
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[1]->SetImage(RESOURCE_DIR"/Scenery/castle.png");
        m_Background[1]->SetPosition(202 * BLOCK_SIZE - 320.0f, 4 * BLOCK_SIZE - 325.0f);
        m_Background[1]->SetScale(3.0f, 3.0f);
        // [2]: flag set
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[2]->SetImage(RESOURCE_DIR"/Scenery/flag-mast.png");
        m_Background[2]->SetPosition(198 * BLOCK_SIZE - 320.0f, 8 * BLOCK_SIZE - 390.0f);
        m_Background[2]->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        // TODO append lifting platform to background
        // TODO getter: lifting platform, add them to mario's collision box
        m_CollisionBoxes.clear();

        // coin init(map 1-3)
        m_CollectibleCoins.clear();
        for (size_t i = 0; i < coin_imgidx3.size(); i++) {
            m_CollectibleCoins.push_back(std::make_shared<AnimatedImage>());
            m_CollectibleCoins.back()->SetImage({imagePaths[coin_imgidx3[i]],imagePaths[coin_imgidx3[i] + 1],imagePaths[coin_imgidx3[i] + 1]},1000,0);
            m_CollectibleCoins.back()->SetPosition(coin_x3[i] * BLOCK_SIZE + BACKGROUND_X_OFFSET, coin_y3[i] * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
            m_CollectibleCoins.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
        }
    }

    SetTime(LEVEL_TIME[m_Phase]);
    m_WorldText->SetTxtIdx(3, m_Phase);
    m_OtherText->SetTxtIdx(5, 0);
}

void PhaseResourceManger::ResetPosition(float dis) const {
    m_ScoreText->m_Transform.translation.x += dis;
    m_MoneyText->m_Transform.translation.x += dis;
    m_WorldText->m_Transform.translation.x += dis;
    m_TimeText->m_Transform.translation.x += dis;
}

void PhaseResourceManger::ConvertTimeToScore(std::shared_ptr<Mario> mario) {
    if (time > 0) {
        time -= 1;
        m_TimeText->SetTxtIdx(4, time);
        mario->IncreaseScore(50);
    }
}

void PhaseResourceManger::DecreaseTime() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = now - last_update;

    if (elapsed.count() >= 1.0f) { // 每秒執行一次
        last_update = now;
        time -= 1;

        if (time < 0) time = 0;
        m_TimeText->SetTxtIdx(4, time);
    }
}

void PhaseResourceManger::SetTime(const int time) {
    this->time = time;
}

int PhaseResourceManger::GetTime() const {
    return time;
}

std::vector<std::shared_ptr<BackgroundImage>> PhaseResourceManger::GetCollisionBoxes() {
    return m_CollisionBoxes;
}

std::vector<std::shared_ptr<BackgroundImage>> PhaseResourceManger::GetBackground() {
    return m_Background;
}

void PhaseResourceManger::SetCoin(const int coin) const {
    m_MoneyText->SetTxtIdx(2, coin);
}

std::vector<std::shared_ptr<BackgroundImage>> PhaseResourceManger::GetCollectibleCoins() {
    return m_CollectibleCoins;
}

void PhaseResourceManger::SetScore(const int score) const {
    m_ScoreText->SetTxtIdx(1, score);
}