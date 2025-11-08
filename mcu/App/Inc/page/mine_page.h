//
// Created by CNVHK on 25-7-27.
//

#pragma once
#include "page.h"
class mine_page : public page
{
  public:
    mine_page();
    void update_ui() override;
    void enter() override;
    void leave() override;
    void on_encoder_changed(int32_t diff) override;
    static void initGame();
    static void gameOver();
    static void gameWin();
    static void checkResult();
    static void drawFlag(uint8_t x, uint8_t y);
    static void drawBomb(uint8_t x, uint8_t y);
    static void drawBox(uint8_t x, uint8_t y);
    static void drawFrame(uint8_t x, uint8_t y);
    static void drawFrame(uint8_t x, uint8_t y, uint8_t color);
    static void drawNum(uint8_t x, uint8_t y, const char *num);
    typedef void (*DrawMap)(uint8_t x, uint8_t y);
    static void drawMap(DrawMap draw);
    static void drawResult(uint8_t x, uint8_t y);
    static void clearBox(uint8_t x, uint8_t y);
    static void updatePos(uint8_t x, uint8_t y);
    static void updateFlag();
    static void updateLevel();
    static void sweepMine(uint8_t x, uint8_t y);
    static constexpr uint8_t frame_width = 12;
    static constexpr  uint8_t frame_height = 12;
    static constexpr  uint8_t cell_width = 10;
    static constexpr  uint8_t cell_height = 10;

};
extern mine_page mine_page_instance;
