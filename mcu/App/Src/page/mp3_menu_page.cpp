#include "page/mp3_menu_page.h"
#include "page/menu_page.h"
#include "oled.h"
#include <cmath>

#include "mp3.h"
#include "page/mp3_page.h"
#include "page/main_menu_page.h"

constexpr int SHADOW_BOTTOM_OFFSET = 16;

mp3_menu_page::mp3_menu_page(const char *title, menu_item *items, size_t item_count, page *parent)
    :menu_page(title, items, item_count, parent) {
    this->key_handlers[KEY_X3Y1].on_pressed = [this](key_state state) { mp3::Increase_The_Volume(); };
    this->key_handlers[KEY_X3Y0].on_pressed = [this](key_state state) { mp3::Decrease_The_Volume(); };
}


void mp3_menu_page::enter() {
    if (mp3::Get_Instance().state.enabled) {
        this->item_lines = static_cast<float>(screen_height - header_height-SHADOW_BOTTOM_OFFSET) / (float)item_height;
    }else {
        this->item_lines = static_cast<float>(screen_height - header_height) / (float)item_height;
    }
}

void mp3_menu_page::leave() {

}

void mp3_menu_page::update_ui() {
    top_index.update();
    shadow_top.update();
    shadow_bottom.update();
    shadow_width.update();

    u8g2_SetFont(&screen, u8g2_font_wqy14_t_gb2312_lite);
    u8g2_ClearBuffer(&screen);
    // Draw header
    u8g2_SetClipWindow(&screen, 0, 0, screen_width, header_height);
    u8g2_DrawUTF8(&screen, 0, header_height - 1, title);

    // Draw content
    if (!mp3::Get_Instance().state.enabled) {
        u8g2_SetClipWindow(&screen, 0, header_height, screen_width, screen_height);
        u8g2_uint_t item_bottom = header_height;
        const auto top_index_value = top_index.value();
        auto rounded_offset_y = std::lround((top_index_value - std::floor(top_index_value)) * (float)item_height);
        item_bottom -= static_cast<u8g2_uint_t>(rounded_offset_y);
        for (size_t i = std::floor(top_index_value); i < item_count; i++)
        {
            item_bottom += item_height;
            u8g2_DrawLine(&screen, 2, item_bottom - item_height / 2 - 1, 4, item_bottom - item_height / 2 - 1);
            u8g2_DrawUTF8(&screen, 10, item_bottom - 2, items[i].text);
        }

        u8g2_SetDrawColor(&screen, 2);
        auto biased_shadow_top = std::lround(shadow_top.value() - top_index_value * (float)item_height);
        auto biased_shadow_bottom = std::lround(shadow_bottom.value() - top_index_value * (float)item_height);
        auto shadow_height = biased_shadow_bottom - biased_shadow_top;
        if (biased_shadow_top < 0)
        {
            shadow_height += biased_shadow_top;
            biased_shadow_top = 0;
        }
        if (shadow_height > 0)
        {
            u8g2_SetDrawColor(&screen, 2);
            u8g2_DrawRBox(&screen, 0, (u8g2_uint_t)biased_shadow_top, (u8g2_uint_t)shadow_width.value(),
                          (u8g2_uint_t)shadow_height, 2);
            u8g2_SetDrawColor(&screen, 1);
        }

    }else {
        u8g2_SetClipWindow(&screen, 0, header_height, screen_width, 48);
        u8g2_uint_t item_bottom = header_height;
        const auto top_index_value = top_index.value();
        auto rounded_offset_y = std::lround((top_index_value - std::floor(top_index_value)) * (float)item_height);
        item_bottom -= static_cast<u8g2_uint_t>(rounded_offset_y);
        for (size_t i = std::floor(top_index_value); i < item_count; i++)
        {
            item_bottom += item_height;
            u8g2_DrawLine(&screen, 2, item_bottom - item_height / 2 - 1, 4, item_bottom - item_height / 2 - 1);
            u8g2_DrawUTF8(&screen, 10, item_bottom - 2, items[i].text);
        }

        u8g2_SetDrawColor(&screen, 2);
        auto biased_shadow_top = std::lround(shadow_top.value() - top_index_value * (float)item_height);
        auto biased_shadow_bottom = std::lround(shadow_bottom.value() - top_index_value * (float)item_height);
        auto shadow_height = biased_shadow_bottom - biased_shadow_top;
        if (biased_shadow_top < 0)
        {
            shadow_height += biased_shadow_top;
            biased_shadow_top = 0;
        }
        if (shadow_height > 0)
        {
            u8g2_SetDrawColor(&screen, 2);
            u8g2_DrawRBox(&screen, 0, (u8g2_uint_t)biased_shadow_top, (u8g2_uint_t)shadow_width.value(),
                          (u8g2_uint_t)shadow_height, 2);
            u8g2_SetDrawColor(&screen, 1);
        }
        u8g2_SetClipWindow(&screen, 0, 49, screen_width, screen_height);

        mp3::Get_Instance().Show_Time();

    }

    u8g2_SetMaxClipWindow(&screen);

    if (mp3::Get_Instance().state.volume_set_state) {
        mp3::Get_Instance().Show_Volume();
    }

    u8g2_SendBuffer(&screen);
}


static void on_enter_music1()
{
    mp3_page_instance.music_id = 0;
    route_to(&mp3_page_instance);
}

static void on_enter_music2()
{
    mp3_page_instance.music_id = 1;
    route_to(&mp3_page_instance);
}

static void on_enter_music3()
{
    mp3_page_instance.music_id = 2;
    route_to(&mp3_page_instance);
}

// static void on_enter_music4()
// {
//     mp3_page_instance.music_id = 3;
//     route_to(&mp3_page_instance);
// }
//
// static void on_enter_music5()
// {
//     mp3_page_instance.music_id = 4;
//     route_to(&mp3_page_instance);
// }

static void on_enter_exit()
{
    mp3::Reset();
    mp3::Get_Instance().state.enabled=false;
    mp3::Get_Instance().state.volume_set_state=false;
    mp3::Get_Instance().state.current_time=0;
    mp3::Get_Instance().state.play_state=false;
    mp3::Get_Instance().current_music_id=0xFFFF;
    route_to(&main_menu_page_instance);
}

static menu_item items[] = {
    {
        .text = Song_List[0].title,
        .on_enter = on_enter_music1,
    },
    {
        .text = Song_List[1].title,
        .on_enter = on_enter_music2,
    },
    {
        .text = Song_List[2].title,
        .on_enter = on_enter_music3,
    },
    /*{
        .text = Song_List[3].title,
        .on_enter = on_enter_music3,
    },
    {
        .text = Song_List[4].title,
        .on_enter = on_enter_music3,
    },*/
    {
        .text = "退出",
        .on_enter = on_enter_exit,
    },
};





mp3_menu_page mp3_menu_page_instance("[ MP3测试 ]", items, std::size(items), &main_menu_page_instance);

