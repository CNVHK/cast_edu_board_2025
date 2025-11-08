#include "page/adc_page.h"

#include <cstdio>

#include "adc.h"
#include "my_fonts.h"
#include "oled.h"
#include "stm32f4xx_hal_adc.h"
#include "tim.h"
#include "page/main_menu_page.h"

adc_page::adc_page()
    : adc_buffer{0,0,0,0,0,0,0,0,0,0},current_adc_num(0),current_voltage(0.0f){
    this->key_handlers[USER_KEY_2].on_pressed = [this](key_state) { route_to(&main_menu_page_instance); };
    this->key_handlers[KEY_X0Y0].on_pressed = [this](key_state) { route_to(&main_menu_page_instance); };
}

void adc_page::enter() {
    HAL_ADC_Start_DMA(&hadc1,reinterpret_cast<uint32_t *>(this->adc_buffer),10);
}

void adc_page::leave() {
    HAL_ADC_Stop_DMA(&hadc1);
}

void adc_page::update_ui() {
    static char buffer[30];
    static char count=0;
    if(count==0) {
        u8g2_SetFont(&screen, u8g2_font_wqy14_t_gb2312_lite);
        u8g2_ClearBuffer(&screen);
        sprintf(buffer,"当前ADC采样值:%d",this->current_adc_num);
        u8g2_DrawUTF8(&screen,0,14,buffer);
        this->current_voltage=static_cast<float>(this->current_adc_num)*3.3f/4095;
        sprintf(buffer,"当前电压值:%.2fV",this->current_voltage);
        u8g2_DrawUTF8(&screen,0,28,buffer);
        u8g2_SendBuffer(&screen);
    }
    count=(count+1)%15;
}


void adc_page::get_current_adc() {
    uint32_t sum=0;
    for (unsigned short i : this->adc_buffer) {
        sum+=i;
    }
this->current_adc_num=sum/10;
}


adc_page adc_page_instance;





