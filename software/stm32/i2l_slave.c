#include "stm32f1xx_hal.h"
#include "i2l_slave.h"


extern uint8_t am2320_f3_answ[8];
static __IO uint32_t s_am2320_f3_answ_bit;
static __IO e_i2l_state s_i2l_state;

e_i2l_state i2l_get_state(void){
  return s_i2l_state;
}

uint8_t i2l_init(void){
  s_i2l_state = e_i2l_init;
  s_am2320_f3_answ_bit = 0;
  return 0;
}

uint8_t i2l_start(void){
  s_i2l_state = e_i2l_start;
  return 0;
}

uint8_t i2l_set_ready(void){
  HAL_GPIO_WritePin(I2L_SDA_GPIO_Port, I2L_SDA_Pin, GPIO_PIN_RESET);
  s_i2l_state = e_i2l_ready;
  return 0;
}

uint8_t i2l_set_data(void){
  s_i2l_state = e_i2l_data;
  s_am2320_f3_answ_bit = 0;
  return 0;
}

uint8_t i2l_set_data_bit(void){
  if (s_am2320_f3_answ_bit < F3_ANSW_LEN){
    GPIO_PinState bit = (am2320_f3_answ[s_am2320_f3_answ_bit >> 3] & (1 << ((s_am2320_f3_answ_bit & 0x07))))? GPIO_PIN_SET:GPIO_PIN_RESET;
    HAL_GPIO_WritePin(I2L_SDA_GPIO_Port, I2L_SDA_Pin, bit);
  }else{
    HAL_GPIO_WritePin(I2L_SDA_GPIO_Port, I2L_SDA_Pin, GPIO_PIN_SET);
    s_i2l_state = e_i2l_stop;
  }
  return 0;
}

