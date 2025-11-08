//
// Created by CNVHK on 25-8-15.
//

#pragma once
#include "page.h"
class arduboy3d_page : public page
{
public:
    arduboy3d_page();
    void update_ui() override;
    void enter() override;
    void leave() override;
    void on_encoder_changed(int32_t diff) override;
};
extern arduboy3d_page arduboy3d_page_instance;