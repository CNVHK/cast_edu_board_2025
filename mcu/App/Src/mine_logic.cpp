//
// Created by CNVHK on 25-7-27.
//

#include "mine_logic.h"
#include <cstring>
#include <bits/locale_classes.h>

mine_logic_t mine_logic_instance;

mine_logic_t::mine_logic_t()
{
    initMine();
}

void mine_logic_t::initMine()
{
    mine_logic_instance.frame_pos_t.x = 0;
    mine_logic_instance.frame_pos_t.y = 0;
}

void mine_logic_t::random_bomb()
{
    memset(mine_logic_instance.bomb_map, UNOPENED, sizeof(mine_logic_instance.bomb_map));
    memset(mine_logic_instance.result_map, UNOPENED, sizeof(mine_logic_instance.result_map));
    memset(mine_logic_instance.sweep_map, UNOPENED, sizeof(mine_logic_instance.sweep_map));
    memset(mine_logic_instance.flag_map, UNOPENED, sizeof(mine_logic_instance.flag_map));

    uint8_t tempx, tempy;
    for (uint8_t i = 0; i < mine_logic_instance.bomb_num; i++)
    {
        tempx = random() % mine_logic_instance.frame_size_t.x;
        tempy = random() % mine_logic_instance.frame_size_t.y;

        if (mine_logic_instance.bomb_map[tempy * mine_logic_instance.frame_size_t.x + tempx] == BOMB)
        {
            i --;
            continue;
        }

        mine_logic_instance.bomb_map[tempy * mine_logic_instance.frame_size_t.x + tempx] = BOMB;
        add_result(tempx, tempy);
    }
}

void mine_logic_t::add_result(uint8_t x, uint8_t y)
{
    uint8_t temp = true;
    mine_logic_instance.result_map[y * mine_logic_instance.frame_size_t.x + x] = BOMB;
    if (x + 1 < mine_logic_instance.frame_size_t.x)
    {
        temp = false;
        add_num(y * mine_logic_instance.frame_size_t.x + (x + 1));

        if (y - 1 >= 0)
        {
            add_num((y - 1) * mine_logic_instance.frame_size_t.x + x);
            add_num((y - 1) * mine_logic_instance.frame_size_t.x + (x + 1));
        }
        if (y + 1 < mine_logic_instance.frame_size_t.y)
        {
            add_num((y + 1) * mine_logic_instance.frame_size_t.x + x);
            add_num((y + 1) * mine_logic_instance.frame_size_t.x + (x + 1));
        }
    }
    if (x - 1 >= 0)
    {
        add_num(y * mine_logic_instance.frame_size_t.x + (x - 1));

        if (y - 1 >= 0)
        {
            if (temp)
            {
                add_num((y - 1) * mine_logic_instance.frame_size_t.x + x);
            }
            add_num((y - 1) * mine_logic_instance.frame_size_t.x + (x - 1));
        }
        if (y + 1 < mine_logic_instance.frame_size_t.y)
        {
            if (temp)
            {
                add_num((y + 1) * mine_logic_instance.frame_size_t.x + x);
            }
            add_num((y + 1) * mine_logic_instance.frame_size_t.x + (x - 1));
        }
    }
}

void mine_logic_t::add_num(uint8_t pos)
{
    if (mine_logic_instance.result_map[pos] != BOMB)
    {
        mine_logic_instance.result_map[pos] += 1;
    }
}

void mine_logic_t::expandBlank(uint8_t x, uint8_t y)
{
    uint8_t index = y * mine_logic_instance.frame_size_t.x + x;

    if (x >= mine_logic_instance.frame_size_t.x || y >= mine_logic_instance.frame_size_t.y || mine_logic_instance.flag_map[index] == 1)
    {
        return;
    }

    mine_logic_instance.sweep_map[index] = OPENED;
    mine_page::sweepMine(x, y);

    if (mine_logic_instance.result_map[index] != 0)
    {
        return;
    }

    for (int8_t dy = -1; dy <= 1; dy++)
    {
        for (int8_t dx = -1; dx <= 1; dx++)
        {
            int8_t nx = x + dx;
            int8_t ny = y + dy;
            if (mine_logic_instance.sweep_map[ny * mine_logic_instance.frame_size_t.x + nx] == OPENED)  continue;
            if (nx >= 0 && nx < mine_logic_instance.frame_size_t.x && ny >= 0 && ny < mine_logic_instance.frame_size_t.y)
            {
                expandBlank(nx, ny);
            }
        }
    }
}
