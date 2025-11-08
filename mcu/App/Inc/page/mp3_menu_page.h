#pragma once
#include "menu_page.h"
#include "mp3.h"
#include "mp3_page.h"

class mp3_menu_page :public menu_page
{
public:
    mp3_menu_page(const char *title, menu_item *items, size_t item_count, page *parent);
    void update_ui() override;
    void enter() override;
    void leave() override;
};


extern mp3_menu_page mp3_menu_page_instance;
