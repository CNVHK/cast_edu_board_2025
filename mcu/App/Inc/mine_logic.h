//
// Created by CNVHK on 25-7-27.
//

#ifndef MINE_LOGIC_H
#define MINE_LOGIC_H
#include "page/mine_page.h"

class mine_logic_t
{
    private:
    static void initMine();
    public:
    mine_logic_t();
    static void random_bomb();
    static void add_num(uint8_t pos);
    static void add_result(uint8_t x, uint8_t y);
    static void expandBlank(uint8_t x, uint8_t y);
    struct frame_size
    {
        uint8_t x = 10;
        uint8_t y = 5;
    } frame_size_t;
    struct frame_pos
    {
        uint8_t x = 0;
        uint8_t y = 0;
    } frame_pos_t;
    uint8_t bomb_map[10*5]{};
    uint8_t result_map[10*5]{};
    uint8_t sweep_map[10*5]{};
    uint8_t flag_map[10*5]{};
    uint8_t bomb_num = 2;
    uint8_t flag_num = bomb_num;
    uint8_t level = 1;
    enum
    {
        UNOPENED = 0,
        OPENED = 1,
        BOMB = 9
    };
};

extern mine_logic_t mine_logic_instance;

#endif //MINE_LOGIC_H
