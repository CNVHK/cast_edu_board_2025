// Copyright (C) 2023 ArcticLampyrid <alampy.com>
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "breathing_light.h"
#include "buzzer.h"
#include "encoder_reader.h"
#include "gpio.h"
#include "oled.h"
#include "page/main_menu_page.h"
#include "page/page.h"
#include "spi.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include <app_main.h>

#include "adc.h"
#include "mp3.h"
#include "page/adc_page.h"

#define ENCODER_A_Pin USER_KEY1_Pin
#define ENCODER_B_Pin USER_KEY2_Pin

static encoder_reader_t encoder =
    encoder_reader_t(USER_KEY1_GPIO_Port, ENCODER_A_Pin, USER_KEY2_GPIO_Port, ENCODER_B_Pin);

extern "C" void app_pre_init()
{
    // do nothing
}

extern "C" void app_init()
{
    // do nothing
}

extern "C" void app_sys_init()
{
    // do nothing
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case ENCODER_A_Pin:
        encoder.handle_a();
        break;
    case ENCODER_B_Pin:
        encoder.handle_b();
        break;
    default:
        break;
    }
}

extern "C" void sys_tick_callback()
{
    // call from SysTick_Handler (interrupt)
    buzzer_tick_1ms();
}

void infrared_transmitter_pulse_init()
{
    //TIM4->SR &= ~TIM_SR_UIF; // clear update interrupt flag
    //TIM4->DIER |= TIM_IT_UPDATE;
}

void infrared_transmitter_schedule_pulse(uint16_t us)
{
    //TIM4->ARR = us - 1;
    //TIM4->CNT = 0;
    //TIM4->CR1 |= TIM_CR1_CEN;
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        if (mp3::Get_Instance().state.current_time<Song_List[mp3::Get_Instance().music_id].time)
            mp3::Get_Instance().state.current_time++;
        else
        {
            mp3::Get_Instance().state.current_time=0;
            mp3::Get_Instance().music_id=(mp3::Get_Instance().music_id+1)%MUSIC_NUM;
            mp3::Get_Instance().current_music_id=mp3::Get_Instance().music_id;
            mp3::Get_Instance().Set_Play_Music();
        }
    }
    else if (htim->Instance == TIM4)
    {
        breathing_light_update();
    }
    else if (htim->Instance == TIM9) {
        static bool first_time = true;
        if (first_time) {
            first_time = false;
        }
        else
        {
            mp3::Get_Instance().state.volume_set_state=false;
            HAL_TIM_Base_Stop_IT(&htim9);
        }
    }
}

extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        // infrared_receiver_on_captured();
    }
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc==&hadc1) {
        adc_page_instance.get_current_adc();
    }
}




extern "C" void app_main()
{
    oled_init();
    mp3::Get_Instance();
    // Logo
    u8g2_SetFont(&screen, u8g2_font_wqy14_t_gb2312_lite);
    u8g2_DrawUTF8(&screen, 8, 39, "欢迎来到通院科协");
    u8g2_SendBuffer(&screen);

    infrared_transmitter_pulse_init();
    buzzer_init();
    breathing_light_begin();

    HAL_Delay(750);

    mp3::Get_Instance().Set_Volume();

    route_to(&main_menu_page_instance);
    for (;;)
    {
        current_page->update_ui();
        int32_t encoder_diff = encoder.count;
        if (encoder_diff != 0)
        {
            encoder.count = 0;
            current_page->on_encoder_changed(-encoder_diff);
        }
        dispatch_for_keys(current_page->key_handlers);
        current_page->tick();
    }
}
