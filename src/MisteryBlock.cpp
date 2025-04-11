//
// Created by 112590007 on 2025/4/11.
//
 #include "MisteryBlock.hpp"

 void MisteryBlock::AfterCollisionEvents() {
     // TODO check props pos
     if (isoverworld) {
         SetImage({RESOURCE_DIR"/Blocks/Overworld/emptyBlock.png"}, 1000, 0);
     }else {
         SetImage({RESOURCE_DIR"/Blocks/Underworld/emptyBlock.png"}, 1000, 0);
     }

 }