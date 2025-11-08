#include <cmath>
#include <cstdio>

#include "mp3.h"
#include "oled.h"
#include "page/mp3_page.h"
#include "page/mp3_menu_page.h"


mp3_page::mp3_page()
{
    this->key_handlers[USER_KEY_1].on_pressed = [this](key_state state) { mp3::Get_Instance().Transit_State();};
    this->key_handlers[USER_KEY_2].on_pressed = [this](key_state state) { route_to(&mp3_menu_page_instance); };
    this->key_handlers[KEY_X3Y1].on_pressed = [this](key_state state) { mp3::Increase_The_Volume(); };
    this->key_handlers[KEY_X3Y0].on_pressed = [this](key_state state) { mp3::Decrease_The_Volume(); };
}

void mp3_page::update_ui()
{
    u8g2_ClearBuffer(&screen);
    u8g2_SetFont(&screen, u8g2_font_wqy14_t_gb2312_lite);
    if (mp3_page_instance.music_id==2) {
        u8g2_DrawUTF8(&screen, 0, 14,"I Really");
        u8g2_DrawUTF8(&screen, 0, 28,"Like You");
    }
    else {
        u8g2_DrawUTF8(&screen, 0, 14, Song_List[mp3_page_instance.music_id].title);
    }
    u8g2_DrawXBM(&screen,79,0,48,48,Song_List[mp3_page_instance.music_id].album);

    mp3::Get_Instance().Show_Time();

    if (mp3::Get_Instance().state.volume_set_state) {
        mp3::Get_Instance().Show_Volume();
    }

    u8g2_SendBuffer(&screen);
}

void mp3_page::enter()
{
    if (mp3::Get_Instance().music_id==mp3::Get_Instance().current_music_id)
    {
        if (mp3::Get_Instance().state.play_state == false)
        {
            mp3::Get_Instance().Start();
        }
    }
    else
    {
        mp3::Get_Instance().current_music_id=mp3::Get_Instance().music_id;
        mp3::Get_Instance().Set_Play_Music();
    }
}

void mp3_page::leave()
{
    //mp3::Get_Instance().Stop();
}

void mp3_page::on_encoder_changed(int32_t diff)
{
    route_to(&mp3_menu_page_instance);
}

mp3_page mp3_page_instance;
