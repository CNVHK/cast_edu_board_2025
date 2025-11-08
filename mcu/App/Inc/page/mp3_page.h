//
// Created by insmtr on 24-10-12.
//

#pragma once
#include "page.h"
class mp3_page : public page
{
  public:
    mp3_page();
    void update_ui() override;
    void enter() override;
    void leave() override;
    void on_encoder_changed(int32_t diff) override;
    uint16_t music_id = 0;
};

extern mp3_page mp3_page_instance;

