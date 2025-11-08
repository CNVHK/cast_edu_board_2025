//
// Created by CNVHK on 25-8-15.
//

#include "Game_CAPI.h"
#include "oled.h"
#include "page/arduboy3d_page.h"
#include "page/game_menu_page.h"

arduboy3d_page arduboy3d_page_instance;

arduboy3d_page::arduboy3d_page()
{
    // this->key_handlers[KEY_OK].on_pressed = [](key_state state) {
    //
    // };
    this->key_handlers[USER_KEY_1].on_pressed = [this](key_state state) { route_to(&game_menu_page_instance); };
    this->key_handlers[USER_KEY_2].on_pressed = [this](key_state state) { route_to(&game_menu_page_instance); };
}

void arduboy3d_page::update_ui()
{
    u8g2_ClearBuffer(&screen);
    Game_Tick();
    Game_Draw();
    u8g2_SendBuffer(&screen);
}

void arduboy3d_page::enter()
{
    Game_Init();
}

void arduboy3d_page::leave()
{

}

void arduboy3d_page::on_encoder_changed(int32_t diff)
{
    route_to(&game_menu_page_instance);
}