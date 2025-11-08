#ifndef ADC_PAGE_H
#define ADC_PAGE_H
#include "page.h"

class adc_page :public page
{
  public:
    adc_page();
    void update_ui() override;
    void enter() override;
    void leave() override;
    void get_current_adc();
    uint16_t adc_buffer[10];
private:
    uint16_t current_adc_num;
    float current_voltage;
};

extern adc_page adc_page_instance;


#endif //ADC_PAGE_H
