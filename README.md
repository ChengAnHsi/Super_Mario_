# Abstract

遊戲名稱：超級瑪利歐

組員：

- 112590007 鄭安琋
- 112590054 婁開翔

# Game Introduction

- 遊戲內容：操控遊戲角色瑪利歐經過層層關卡到達終點(綠色旗子處)，玩家在遊戲地圖中持續奔跑、跳躍，躲避怪物的攻擊，此外可透過道具、遊戲機制擊敗怪物及獲取金幣，目標是到達終點
- 獲勝條件：瑪利歐到達綠色旗子處
- 失敗情況：失去所有生命值
- 關卡情形：每個關卡具有不同的地圖障礙及怪物擺放位置
- 遊玩影片-初版(1-1)：https://youtu.be/-avspZlbOWU?si=w2jzLgnK2WW6tRMu
- 遊玩影片-自製版(1-1)：https://www.youtube.com/watch?v=HkLZ9ESYxCU&ab_channel=FR
- 遊玩影片-全(full walkthrough)：https://www.youtube.com/watch?v=JJR7ER4_jmk
- 遊玩影片-全(1-1 ~ 8-4)：https://www.youtube.com/watch?v=rLl9XBg7wSs&ab_channel=Pii89
- 講解內部細節影片(1-1)：https://www.youtube.com/watch?v=ZH2wGpEZVgE&ab_channel=ExtraHistory
- 主要素材來源：https://github.com/pablogozalvez/Super-Mario-Phaser
- 主要素材來源：https://www.mariomayhem.com/downloads/sprites/super_mario_bros_sprites.php
- 其餘素材來源：https://github.com/feresr/super-mario-bros
- 其餘素材來源：https://github.com/Kofybrek/Super-Mario-Bros
- 其餘素材來源：https://github.com/Gold872/Super-Mario-Bros/tree/master
- 遊玩網站：https://supermarioplay.com/
- 製作參考：https://www.youtube.com/watch?v=7D4uoSoQsjw&t=60s
- 製作參考(camera)：https://github.com/MitchellSternke/Mario/blob/master/source/Camera.cpp
- 移動參考：https://www.youtube.com/watch?v=dMbP4nvwONc
- 移動參考(github)：https://github.com/Cakez77/CelesteClone/blob/master/src/game.cpp
- 重力參考(js)：https://www.youtube.com/watch?v=RFPn9r9mKe0&t=70s
- 重力參考(python)：https://pastebin.com/eJEjjQwv
- 重力參考(C++)：https://www.youtube.com/watch?v=V0cGs2ZENSY&ab_channel=Voidmatrix
- 跳躍參考(C++)：https://www.youtube.com/watch?v=JwRYJLI3P0Q&ab_channel=Voidmatrix
- 碰撞參考(C++)：https://www.youtube.com/watch?v=eAjl4V61svE&ab_channel=Voidmatrix
- 碰撞參考(C++)：https://www.youtube.com/watch?v=IaUcAt0jDqs
- 移動平台參考(C++)：https://www.youtube.com/watch?v=X33Oj1jc9vc&ab_channel=Cakez

# Development timeline

- Week 1：準備素材
    - [x] 蒐集遊戲的素材

- Week 2：處理遊戲的封面
    - [x] 遊戲加載封面
- Week 3：實作地圖關卡
    - [x] 地圖背景
    - [x] 地板
    - [x] 碰撞箱
- Week 4：實作地圖關卡
    - [x] 地圖背景
    - [x] 地板
    - [x] 碰撞箱

- Week 5：人物移動邏輯
    - [x] 角色左右移動邏輯
    - [x] 角色跳躍及碰撞箱邏輯
    - [x] 碰撞音效
    - [x] 動畫
- Week 6：人物移動邏輯
    - [x] 角色左右移動邏輯
    - [x] 角色跳躍及碰撞箱邏輯
    - [x] 碰撞音效
    - [x] 動畫
- Week 7：人物移動邏輯
    - [x] 角色左右移動邏輯
    - [x] 角色跳躍及碰撞箱邏輯
    - [x] 碰撞音效
    - [x] 動畫

- Week 8：製作道具機制
    - 道具功能
        - [x] 超級蘑菇
        - [ ] 火之花
        - [ ] 無敵星
        - [x] 1UP蘑菇
    - [x] 生成位置
    - [ ] 音效
    - [ ] 積分
- Week 9：製作道具機制
    - 道具功能
        - [x] 超級蘑菇
        - [ ] 火之花
        - [ ] 無敵星
        - [x] 1UP蘑菇
    - [x] 生成位置
    - [ ] 音效
    - [ ] 積分
- Week 10：製作道具機制
    - 道具功能
        - [x] 超級蘑菇
        - [ ] 火之花
        - [ ] 無敵星
        - [x] 1UP蘑菇
    - [x] 生成位置
    - [ ] 音效
    - [ ] 積分
- Week 11：製作道具機制
    - 道具功能
        - [x] 超級蘑菇
        - [ ] 火之花
        - [ ] 無敵星
        - [x] 1UP蘑菇
    - [x] 生成位置
    - [ ] 音效
    - [ ] 積分

- Week 12：怪物移動邏輯與動畫
    - [x] 怪物移動路徑
    - [x] 碰撞邏輯
- Week 13：怪物移動邏輯與動畫
    - [x] 怪物移動路徑
    - [ ] 碰撞邏輯
- Week 14：怪物移動邏輯與動畫
    - [x] 怪物移動路徑
    - [ ] 碰撞邏輯

- Week 15：移動平台
    - [ ] 上升與下降
    - [ ] 位移判斷

- Week 16：死亡畫面與過關轉場
    - [ ] GameOver畫面
    - [ ] 死亡轉場(性命黑畫面)
    - [x] 復活地圖更新
    - [ ] 拉旗子
  
- Week 17：收尾
    - [ ] 針對遊戲細節進行優化