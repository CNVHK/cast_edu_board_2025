//
// Created by CNVHK on 25-7-27.
//

#include "oled.h"
#include "keyboard.h"
#include "mine_logic.h"
#include "page/mine_page.h"

#include "stm32f4xx_hal.h"
#include "page/game_menu_page.h"
#include <string>
#include <sys/types.h>

mine_page mine_page_instance;

mine_page::mine_page()
{
    this->key_handlers[KEY_X0Y3].on_pressed = [](key_state state) {
        checkResult();
    };
    this->key_handlers[USER_KEY_1].on_pressed = [this](key_state state) {
        gameWin();
    };
    this->key_handlers[USER_KEY_2].on_pressed = [this](key_state state) {
        route_to(&game_menu_page_instance);
    };
    this->key_handlers[KEY_X0Y0].on_pressed = [this](key_state state) {
        updatePos(mine_logic_instance.frame_pos_t.x - 1, mine_logic_instance.frame_pos_t.y);
    };
    this->key_handlers[KEY_X2Y0].on_pressed = [this](key_state state) {
        updatePos(mine_logic_instance.frame_pos_t.x + 1, mine_logic_instance.frame_pos_t.y);
    };
    this->key_handlers[KEY_X1Y0].on_pressed = [this](key_state state) {
        updatePos(mine_logic_instance.frame_pos_t.x, mine_logic_instance.frame_pos_t.y + 1);
    };
    this->key_handlers[KEY_X1Y1].on_pressed = [this](key_state state) {
        updatePos(mine_logic_instance.frame_pos_t.x, mine_logic_instance.frame_pos_t.y - 1);
    };
    this->key_handlers[KEY_X3Y0].on_pressed = [this](key_state state) {
        sweepMine(mine_logic_instance.frame_pos_t.x, mine_logic_instance.frame_pos_t.y);
        mine_logic_t::expandBlank(mine_logic_instance.frame_pos_t.x, mine_logic_instance.frame_pos_t.y);
    };
    this->key_handlers[KEY_X3Y1].on_pressed = [this](key_state state) {
        drawFlag(mine_logic_instance.frame_pos_t.x, mine_logic_instance.frame_pos_t.y);
    };
}

void mine_page::update_ui()
{
    u8g2_SendBuffer(&screen);
}

void mine_page::initGame()
{
    mine_logic_t::random_bomb();
    mine_logic_instance.flag_num = mine_logic_instance.bomb_num;
    u8g2_ClearBuffer(&screen);
    drawMap(drawBox);
    updatePos(0, 0);
    updateFlag();
    updateLevel();
    u8g2_SendBuffer(&screen);
}
void mine_page::gameOver()
{
    drawMap(drawResult);
}

void mine_page::gameWin()
{
    u8g2_SetDrawColor(&screen, 1);
    u8g2_DrawFrame(&screen, 36, 12, 48, 36);
    u8g2_DrawBox(&screen, 36, 12, 46, 34);
    u8g2_SetDrawColor(&screen, 0);
    u8g2_SetFont(&screen, u8g2_font_wqy14_t_gb2312_lite);
    u8g2_DrawUTF8(&screen, 45, 35, "胜利");
    u8g2_SetDrawColor(&screen, 1);
    u8g2_SendBuffer(&screen);
    mine_logic_instance.level ++;
    mine_logic_instance.flag_num = ++ mine_logic_instance.bomb_num;
    HAL_Delay(2000);
    initGame();
}

void mine_page::checkResult()
{
    uint8_t temp = true;
    for (uint8_t y = 0; y < mine_logic_instance.frame_size_t.y; y++)
    {
        for (uint8_t x = 0; x < mine_logic_instance.frame_size_t.x; x++)
        {
            if (mine_logic_instance.bomb_map[y * mine_logic_instance.frame_size_t.x + x] == mine_logic_t::BOMB)
            {
                if (mine_logic_instance.flag_map[y * mine_logic_instance.frame_size_t.x + x] != 1)
                {
                    temp = false;
                }
            }
            if (mine_logic_instance.flag_map[y * mine_logic_instance.frame_size_t.x + x] == 1)
            {
                if (mine_logic_instance.bomb_map[y * mine_logic_instance.frame_size_t.x + x] != mine_logic_t::BOMB)
                {
                    temp = false;
                }
            }
        }
    }

    if (temp)
    {
        gameWin();
    }
}

void mine_page::drawFlag(uint8_t x, uint8_t y) {
    uint8_t tempx = x * frame_width + 4;
    uint8_t tempy = y * frame_height + 2;
    if (mine_logic_instance.sweep_map[y * mine_logic_instance.frame_size_t.x + x] == mine_logic_t::UNOPENED)
    {
        if (mine_logic_instance.flag_map[y * mine_logic_instance.frame_size_t.x + x] == 0)
        {
            if (mine_logic_instance.flag_num > 0)
            {
                mine_logic_instance.flag_num --;
                updateFlag();
                mine_logic_instance.flag_map[y * mine_logic_instance.frame_size_t.x + x] = mine_logic_t::OPENED;

                u8g2_SetDrawColor(&screen, 0);
                u8g2_DrawVLine(&screen, tempx, tempy, 6);
                u8g2_DrawLine(&screen, tempx, tempy, tempx + 3, tempy + 2);
                u8g2_DrawLine(&screen, tempx + 3, tempy + 2, tempx, tempy + 4);
                u8g2_DrawLine(&screen, tempx, tempy, tempx, tempy + 4);
                u8g2_SetDrawColor(&screen, 1);
            }
        }
        else
        {
            mine_logic_instance.flag_num ++;
            updateFlag();

            mine_logic_instance.flag_map[y * mine_logic_instance.frame_size_t.x + x] = mine_logic_t::UNOPENED;

            u8g2_SetDrawColor(&screen, 1);
            u8g2_DrawVLine(&screen, tempx, tempy, 6);
            u8g2_DrawLine(&screen, tempx, tempy, tempx + 3, tempy + 2);
            u8g2_DrawLine(&screen, tempx + 3, tempy + 2, tempx, tempy + 4);
            u8g2_DrawLine(&screen, tempx, tempy, tempx, tempy + 4);
            u8g2_SetDrawColor(&screen, 0);
        }
    }
}

void mine_page::drawBomb(uint8_t x, uint8_t y)
{
    u8g2_SetDrawColor(&screen, 1);
    u8g2_DrawDisc(&screen, x + 6, y + 6, 3, U8G2_DRAW_ALL);
    u8g2_DrawLine(&screen, x + 6, y, x + 6, y + 2);
    u8g2_DrawLine(&screen, x + 5, y + 1, x + 7, y + 1);
    u8g2_DrawTriangle(&screen, x + 6, y, x + 3, y + 3, x + 9, y + 3);
}

void mine_page::drawBox(uint8_t x, uint8_t y)
{
    u8g2_DrawFrame(&screen, x * frame_width, y * frame_height, frame_width, frame_height);
    u8g2_DrawBox(&screen, x * frame_width, y * frame_height, cell_width, cell_height);
}

void mine_page::drawFrame(uint8_t x, uint8_t y)
{
    u8g2_SetDrawColor(&screen, 1);
    u8g2_DrawFrame(&screen, x * frame_width, y * frame_height, frame_width, frame_height);
}

void mine_page::drawFrame(uint8_t x, uint8_t y, uint8_t color)
{
    u8g2_SetDrawColor(&screen, color);
    u8g2_DrawFrame(&screen, x * frame_width, y * frame_height, frame_width, frame_height);
    u8g2_SetDrawColor(&screen, color);
}

void mine_page::drawNum(uint8_t x, uint8_t y, const char *num)
{
    clearBox(x, y);
    u8g2_SetFont(&screen, u8g2_font_wqy12_t_gb2312_lite);
    u8g2_DrawUTF8(&screen, x * frame_width + 3, y * frame_height + 10, num);
}

void mine_page::clearBox(uint8_t x, uint8_t y)
{
    u8g2_SetDrawColor(&screen, 0);
    drawBox(x, y);
    u8g2_SetDrawColor(&screen, 1);
    drawFrame(x, y);
}

void mine_page::updatePos(uint8_t x, uint8_t y)
{
    if (x < mine_logic_instance.frame_size_t.x && y < mine_logic_instance.frame_size_t.y && x >= 0 && y >= 0)
    {
        drawFrame(mine_logic_instance.frame_pos_t.x, mine_logic_instance.frame_pos_t.y);
        mine_logic_instance.frame_pos_t = {x, y};
        drawFrame(x, y, 0);
    }
}

void mine_page::sweepMine(uint8_t x, uint8_t y)
{
    uint8_t index = y * mine_logic_instance.frame_size_t.x + x;

    if (mine_logic_instance.flag_map[index] != 0)
    {
        return;
    }

    mine_logic_instance.sweep_map[index] = mine_logic_t::OPENED;
    if (mine_logic_instance.result_map[index] == 0)
    {
        clearBox(x, y);
    }

    if (mine_logic_instance.result_map[index] != 0)
    {
        clearBox(x, y);
        drawResult(x, y);
    }

    if (mine_logic_instance.result_map[index] == mine_logic_t::BOMB)
    {
        gameOver();
    }
}

void mine_page::drawMap(DrawMap draw)
{
    for (uint8_t y = 0; y < mine_logic_instance.frame_size_t.y; y++)
    {
        for (uint8_t x = 0; x < mine_logic_instance.frame_size_t.x; x++)
        {
            draw(x, y);
        }
    }
}

void mine_page::drawResult(uint8_t x, uint8_t y)
{
    std::pmr::string ch;
    u8g2_SetDrawColor(&screen, 1);
    u8g2_SetFont(&screen, u8g2_font_wqy12_t_gb2312_lite);
    clearBox(x, y);

    if (mine_logic_instance.result_map[y * mine_logic_instance.frame_size_t.x + x] != 0)
    {
        ch = std::to_string(mine_logic_instance.result_map[y * mine_logic_instance.frame_size_t.x + x]);
        u8g2_DrawUTF8(&screen, x * frame_width + 3, y * frame_height + 10, ch.c_str());
    }

    if (mine_logic_instance.result_map[y * mine_logic_instance.frame_size_t.x + x] == mine_logic_t::BOMB)
    {
        drawBomb(x * frame_width, y * frame_height);
    }
}

void mine_page::updateFlag()
{
    std::pmr::string ch;
    ch = std::to_string(mine_logic_instance.flag_num);
    u8g2_SetDrawColor(&screen, 0);
    u8g2_DrawBox(&screen, 120, 0, 12, 12);
    u8g2_SetDrawColor(&screen, 1);
    u8g2_DrawUTF8(&screen, 122, 12, ch.c_str());
}

void mine_page::updateLevel()
{
    std::pmr::string ch;
    ch = std::to_string(mine_logic_instance.level);
    u8g2_SetDrawColor(&screen, 0);
    u8g2_DrawBox(&screen, 120, 48, 12, 12);
    u8g2_SetDrawColor(&screen, 1);
    u8g2_DrawUTF8(&screen, 122, 60, ch.c_str());
}

void mine_page::enter()
{
    initGame();
}

void mine_page::leave()
{

}

void mine_page::on_encoder_changed(int32_t diff)
{
    route_to(&game_menu_page_instance);
}
