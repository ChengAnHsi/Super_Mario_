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
    m_ScoreText->SetZIndex(100);

    m_MoneyText = std::make_shared<TaskText>();
    m_MoneyText->SetPosition(-70, 285);
    m_MoneyText->SetTxtIdx(2, 0);
    m_MoneyText->SetZIndex(100);

    m_WorldText = std::make_shared<TaskText>();
    m_WorldText->SetPosition(100, 300);
    m_WorldText->SetTxtIdx(3, 1);
    m_WorldText->SetZIndex(100);

    m_TimeText = std::make_shared<TaskText>();
    m_TimeText->SetPosition(300, 300);
    m_TimeText->SetTxtIdx(4, 400);
    m_TimeText->SetZIndex(100);

    m_OtherText = std::make_shared<TaskText>();
    m_OtherText->SetPosition(0, -100);
    m_OtherText->SetTxtIdx(0, 0);
    m_OtherText->SetZIndex(100);

    m_TopScoreText = std::make_shared<TaskText>();
    m_TopScoreText->SetPosition(0, -200);
    m_TopScoreText->SetTxtIdx(6, 0);
    m_TopScoreText->SetZIndex(100);

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
    m_Background.back()->SetPosition(0, 100);
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

bool PhaseResourceManger::CheckMarioCollisionFlag(std::shared_ptr<Mario> mario) {
    if (mario->GetPull() || mario->GetBackToCastle()) return false;

    // m_Background[2][3]: flag
    glm::vec2 a = m_Background[2]->GetPosition();
    glm::vec2 prop_size = m_Background[2]->GetSize();
    prop_size *= 3.0f;

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
    // fixed mario position
    mario->SetPosition(m_Background[2]->GetPosition().x - (mario->GetSize().x * MARIO_MAGNIFICATION) / 2, mario->GetPosition().y);
    return true;
}

bool PhaseResourceManger::CheckMarioCollisionTube(std::shared_ptr<Mario> mario) {
    // 1-2 m_CollisionBoxes[0]: horizontal tube
    glm::vec2 a = m_CollisionBoxes[0]->GetPosition();
    glm::vec2 prop_size = m_CollisionBoxes[0]->GetSize();
    prop_size *= tube_magnification[0];

    glm::vec2 b = mario->GetPosition();
    glm::vec2 b_size = mario->GetSize();

    // * 1.5f to solve detect collision problem
    b_size.x *= MARIO_MAGNIFICATION * 1.5f;
    b_size.y *= MARIO_MAGNIFICATION * 1.5f;

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
    // calculate minimum overlap area
    float minOverlap = std::min({bright - aleft, aright - bleft,atop - bbottom, btop - abottom});

    // CollisionState == Left
    if (minOverlap == bright - aleft && !mario->GetDrill()) {
        mario->SetPosition(m_CollisionBoxes[0]->GetPosition().x - BLOCK_SIZE, m_CollisionBoxes[0]->GetPosition().y);
        mario->SetDrill(true);
        mario->SetDrillState(DrillState::Right);
        mario->SetDrillDistance(2 * BLOCK_SIZE);
    }

    return true;
}

void PhaseResourceManger::NextPhase(int m_Phase) {
    // LOG_DEBUG("Passed! Next phase: {}", m_Phase);
    if(m_Phase == 0) {
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
        m_Background.clear();
        // [0]: logo
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->SetPosition(0, 100);
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

        m_CollisionBoxes.clear();
        m_CollectibleCoins.clear();
    }
    if (m_Phase == 1){
        m_Background.clear();
        // [0]: blue background image
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->SetPhaseImgIsOverWorld(true);
        m_Background.back()->SetPosition(0.0f,0.0f);
        m_Background.back()->SetScale(80.0f,7.0f);
        m_Background.back()->SetZIndex(-50);
        // [1]: castle
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
            m_CollisionBoxes.back()->SetZIndex(80);
        }
    }
    if (m_Phase == 2){
        m_Background.clear();
        // [0]: black background image
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->SetPhaseImgIsOverWorld(false);
        m_Background.back()->SetPosition(0.0f,0.0f);
        m_Background.back()->SetScale(80.0f,7.0f);
        m_Background.back()->SetZIndex(-50);

        // to fix camera move position change problem
        float diff_x = -11 * BLOCK_SIZE;
        // [1]: castle
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->SetImage(RESOURCE_DIR"/Scenery/castle.png");
        m_Background.back()->SetPosition(204 * BLOCK_SIZE + BACKGROUND_X_OFFSET + diff_x, 19 * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        m_Background.back()->SetScale(3.0f, 3.0f);
        // [2][3]: flag set
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->SetImage(RESOURCE_DIR"/Scenery/flag-mast.png");
        m_Background.back()->SetPosition(198 * BLOCK_SIZE + BACKGROUND_X_OFFSET + diff_x, 23 * BLOCK_SIZE - 385.0f);
        m_Background.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);

        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->SetImage(RESOURCE_DIR"/Scenery/final-flag.png");
        m_Background.back()->SetPosition(197.5 * BLOCK_SIZE + BACKGROUND_X_OFFSET + diff_x, 26 * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        m_Background.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);

        // tube & lifting platform init(map 1-2)
        m_CollisionBoxes.clear();
        // end horizontal tube
        m_CollisionBoxes.push_back(std::make_shared<BackgroundImage>());
        m_CollisionBoxes.back()->SetImage(RESOURCE_DIR"/Scenery/horizontal-final-tube.png");
        m_CollisionBoxes.back()->SetPosition(167 * BLOCK_SIZE + tubex_offset[1], 6 * BLOCK_SIZE + tubey_offset[0]);
        m_CollisionBoxes.back()->SetScale(tube_magnification[0], tube_magnification[0]);
        m_CollisionBoxes.back()->SetZIndex(80);
        // end vertical tube
        m_CollisionBoxes.push_back(std::make_shared<BackgroundImage>());
        m_CollisionBoxes.back()->SetImage(RESOURCE_DIR"/Scenery/vertical-xlarge-tube.png");
        m_CollisionBoxes.back()->SetPosition(168 * BLOCK_SIZE + tubex_offset[0], 12 * BLOCK_SIZE + tubey_offset[2]);
        m_CollisionBoxes.back()->SetScale(tube_magnification[0], tube_magnification[0]);
        m_CollisionBoxes.back()->SetZIndex(80);
        // top tube
        m_CollisionBoxes.push_back(std::make_shared<BackgroundImage>());
        m_CollisionBoxes.back()->SetImage(RESOURCE_DIR"/Scenery/vertical-medium-tube.png");
        m_CollisionBoxes.back()->SetPosition(2 * BLOCK_SIZE + tubex_offset[1], 14 * BLOCK_SIZE + tubey_offset[1]);
        m_CollisionBoxes.back()->SetScale(tube_magnification[1], -tube_magnification[1]);
        m_CollisionBoxes.back()->SetZIndex(80);

        // over world spawn tube
        m_CollisionBoxes.push_back(std::make_shared<BackgroundImage>());
        m_CollisionBoxes.back()->SetImage(RESOURCE_DIR"/Scenery/vertical-xslarge-tube.png");
        m_CollisionBoxes.back()->SetPosition(168 * BLOCK_SIZE + tubex_offset[1] + 3, 17 * BLOCK_SIZE + tubey_offset[1]);
        m_CollisionBoxes.back()->SetScale(tube_magnification[2], tube_magnification[2]);
        m_CollisionBoxes.back()->SetZIndex(80);

        for (size_t i = 0; i < collisionboxes_x2.size(); i++) {
            m_CollisionBoxes.push_back(std::make_shared<BackgroundImage>());
            m_CollisionBoxes.back()->SetImage(imagePaths[collisionboxes_imgidx2[i]]);
            m_CollisionBoxes.back()->SetPosition(collisionboxes_x2[i] * BLOCK_SIZE + tubex_offset[collisionboxes_imgidx2[i]], collisionboxes_y2[i] * BLOCK_SIZE  + tubey_offset[collisionboxes_imgidx2[i]]);
            m_CollisionBoxes.back()->SetScale(tube_magnification[collisionboxes_imgidx2[i]], tube_magnification[collisionboxes_imgidx2[i]]);
            m_CollisionBoxes.back()->SetZIndex(80);
        }

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
        m_Background.back()->SetPhaseImgIsOverWorld(true);
        m_Background.back()->SetPosition(0.0f,0.0f);
        m_Background.back()->SetScale(80.0f,7.0f);
        m_Background.back()->SetZIndex(-50);
        // [1]: castle is visible
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[1]->SetImage(RESOURCE_DIR"/Scenery/castle.png");
        m_Background[1]->SetPosition(159 * BLOCK_SIZE + BACKGROUND_X_OFFSET, 4 * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        m_Background[1]->SetScale(3.0f, 3.0f);
        // [2][3]: flag set
        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background[2]->SetImage(RESOURCE_DIR"/Scenery/flag-mast.png");
        m_Background[2]->SetPosition(152 * BLOCK_SIZE + BACKGROUND_X_OFFSET, 8 * BLOCK_SIZE - 385.0f);
        m_Background[2]->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);

        m_Background.push_back(std::make_shared<BackgroundImage>());
        m_Background.back()->SetImage(RESOURCE_DIR"/Scenery/final-flag.png");
        m_Background.back()->SetPosition(151.5 * BLOCK_SIZE + BACKGROUND_X_OFFSET, 11 * BLOCK_SIZE + BACKGROUND_Y_OFFSET);
        m_Background.back()->SetScale(BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION);
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
    m_OtherText->SetVisible(false);
    m_TopScoreText->SetVisible(false);
    if(m_Phase == 0) {
        SetCoin(0);
        SetScore(0);
        m_TimeText->SetTxtIdx(4, time);
        m_WorldText->SetTxtIdx(3, 1);
        m_OtherText->SetVisible(true);
        m_TopScoreText->SetVisible(true);
    }
}

void PhaseResourceManger::ResetPosition(float disx, float disy) const {
    m_ScoreText->m_Transform.translation.x += disx;
    m_MoneyText->m_Transform.translation.x += disx;
    m_WorldText->m_Transform.translation.x += disx;
    m_TimeText->m_Transform.translation.x += disx;
    m_OtherText->m_Transform.translation.x += disx;
    m_TopScoreText->m_Transform.translation.x += disx;

    m_ScoreText->m_Transform.translation.y += disy;
    m_MoneyText->m_Transform.translation.y += disy;
    m_WorldText->m_Transform.translation.y += disy;
    m_TimeText->m_Transform.translation.y += disy;
    m_OtherText->m_Transform.translation.y += disy;
    m_TopScoreText->m_Transform.translation.y += disy;
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

void PhaseResourceManger::SetScore(const int score) {
    m_ScoreText->SetTxtIdx(1, score);
    if(top_score < score) {
        m_TopScoreText->SetTxtIdx(6, score);
        top_score = score;
    }
}