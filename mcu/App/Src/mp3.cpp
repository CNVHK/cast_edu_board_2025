#include "mp3.h"

#include "main.h"
#include "usart.h"
#include "cstdio"
#include "oled.h"
#include "tim.h"

static volatile mp3_state MP3_State{
    .enabled = false,
    .play_state = false,
    .volume_set_state = false,
    .current_time = 0,
};


static int16_t Get_Triangle_Area(int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t x3,int16_t y3){
    return static_cast<int16_t>(abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)));
}

static void u8g2_DrawFilledTriangle(u8g2_t *u8g2,int16_t x1,int16_t y1,int16_t x2,int16_t y2,int16_t x3,int16_t y3) {
    int16_t max_x,max_y,min_x,min_y,s1,s2,s3;
    if(x1>x2){
        if(x1>x3) max_x=x1;
        else max_x=x3;
        if(x2<x3) min_x=x2;
        else min_x=x3;}
    else{
        if(x2>x3) max_x=x2;
        else max_x=x3;
        if(x1<x3) min_x=x1;
        else min_x=x3;}

    if(y1>y2){
        if(y1>y3) max_y=y1;
        else max_y=y3;
        if(y2<y3) min_y=y2;
        else min_y=y3;}
    else{
        if(y2>y3) max_y=y2;
        else max_y=y3;
        if(y1<y3) min_y=y1;
        else min_y=y3;}

    for(int16_t i = min_x;i<max_x;i++)
        for(int16_t j = min_y;j<max_y;j++){
             s1=Get_Triangle_Area(x1,y1,x2,y2,i,j);
             s2=Get_Triangle_Area(x2,y2,x3,y3,i,j);
             s3=Get_Triangle_Area(x3,y3,x1,y1,i,j);
            if(s1==0||s2==0||s3==0)
                u8g2_DrawPixel(u8g2,i,j);
            if(s1+s2+s3<=Get_Triangle_Area(x1,y1,x2,y2,x3,y3))
                u8g2_DrawPixel(u8g2,i,j);
        }
}



mp3 & mp3::Get_Instance() {
static mp3 instance(MP3_State,15,mp3_page_instance.music_id,0xFFFF);
return instance;
}


void mp3::Send_Command(const uint8_t *Data, const uint8_t &len) {
    HAL_UART_Transmit(&huart1,Data,len,0xFF);
}


void mp3::Reset() {
    constexpr uint8_t len=4;
    constexpr uint8_t data[len]={0xAA,0x04,0x00,0xAE};
    Send_Command(data,len);
}


void mp3::Transit_State() {
    if (this->state.play_state==false) {
        Start();
    }else {
        Stop();
    }
}

void mp3::Start() {
    this->state.play_state=true;
    constexpr uint8_t len=4;
    constexpr uint8_t data[len]={0xAA,0x02,0x00,0xAC};
    Send_Command(data,len);
    HAL_TIM_Base_Start_IT(&htim5);
}

void mp3::Stop() {
    this->state.play_state=false;
    constexpr uint8_t len=4;
    constexpr uint8_t data[len]={0xAA,0x03,0x00,0xAD};
    Send_Command(data,len);
    HAL_TIM_Base_Stop_IT(&htim5);
}

uint8_t mp3::Get_Check_Num(const uint8_t *Data, const uint8_t &num) {
    uint8_t sum=0;
    for(int i = 0;i<num;i++)
        sum+=Data[i];
    return sum;
}

void mp3::Increase_The_Volume() {
    if (mp3::Get_Instance().volume<30) {
        mp3::Get_Instance().volume++;
        mp3::Get_Instance().Set_Volume();
        mp3::Get_Instance().state.volume_set_state=true;
        HAL_TIM_Base_Start_IT(&htim9);
        TIM9->CNT=0;
    }
}

void mp3::Decrease_The_Volume() {
    if (mp3::Get_Instance().volume>0) {
        mp3::Get_Instance().volume--;
        mp3::Get_Instance().Set_Volume();
        mp3::Get_Instance().state.volume_set_state=true;
        HAL_TIM_Base_Start_IT(&htim9);
        TIM9->CNT=0;
    }
}

void mp3::Set_Volume() const {
    uint8_t data[5]={0xAA,0x13,0x01,0x00,0x00};
    data[3]=volume;
    data[4]=Get_Check_Num(data,4);
    Send_Command(data,5);
}

void mp3::Set_Play_Music() const {
    mp3::Get_Instance().state.play_state=true;
    mp3::Get_Instance().state.current_time=0;
    this->state.enabled=true;
    uint8_t data[6]={0xAA,0x07,0x02,0x00,0x00,0x00};
    data[3]=static_cast<uint8_t>((music_id+1)>>8);
    data[4]=static_cast<uint8_t>(music_id+1);
    data[5]=Get_Check_Num(data,5);
    Send_Command(data,6);
    HAL_TIM_Base_Start_IT(&htim5);
    TIM4->CNT=0;
}

mp3::mp3(volatile mp3_state & state, const uint8_t volume, uint16_t& music_id,uint16_t current_music_id)
    : music_id(music_id), current_music_id(current_music_id), volume(volume) , state(state){
    Reset();
}

void mp3::Show_Time() {
    static char song_time_str[6],current_time_str[6];
    sprintf(song_time_str,"%02d:%02d",Song_List[music_id].time/60,Song_List[music_id].time%60);
    sprintf(current_time_str,"%02d:%02d",this->state.current_time/60,this->state.current_time%60);
    u8g2_SetFont(&screen,u8g2_font_spleen5x8_me);
    u8g2_DrawStr(&screen,0,63,current_time_str);
    u8g2_DrawStr(&screen,97,63,song_time_str);
    u8g2_DrawLine(&screen,0,51,127,51);
    u8g2_DrawDisc(&screen,this->state.current_time*123/Song_List[music_id].time+2,51,2,U8G2_DRAW_ALL);
    if (mp3::Get_Instance().state.play_state==true) {
        u8g2_DrawBox(&screen,60,56,2,7);
        u8g2_DrawBox(&screen,64,56,2,7);
    }else {
        u8g2_DrawFilledTriangle(&screen,60,56,60,62,65,59);
        u8g2_DrawPixel(&screen,60,62);
    }
}

void mp3::Show_Volume() {
    u8g2_SetDrawColor(&screen, 0);
    u8g2_DrawBox(&screen,110,0,18,64);
    u8g2_SetDrawColor(&screen, 1);
    u8g2_DrawDisc(&screen,115,12,2,U8G2_DRAW_ALL);
    u8g2_DrawDisc(&screen,125,12,2,U8G2_DRAW_ALL);
    u8g2_DrawLine(&screen,117,12,117,3);
    u8g2_DrawLine(&screen,127,12,127,3);
    u8g2_DrawBox(&screen,117,0,11,4);
    u8g2_DrawFrame(&screen,116,19,11,32);
    u8g2_DrawBox(&screen,116,50-mp3::volume,11,mp3::volume+1);
    static char str[3];
    sprintf(str,"%02d",mp3::volume);
    u8g2_SetFont(&screen,u8g2_font_spleen5x8_me);
    u8g2_DrawStr(&screen,116,62,str);
}


music_info Song_List[MUSIC_NUM]={
    "枫", album[0], 267,
    "兰亭序", album[1], 246,
    "I Really Like You", album[2], 199,
    // "水星记","",100,
    // "Things You Said","",100
};

