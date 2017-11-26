#if !defined __I2L_SLAVE_H__
#define __I2L_SLAVE_H__

#include "stm32f1xx_hal.h"

typedef enum _e_i2l_state {
  e_i2l_init = 0,
  e_i2l_start = 1,
  e_i2l_ready = 2,
  e_i2l_data  = 3,
  e_i2l_stop  = 4,

  e_i2l_error = 0xFF

} e_i2l_state;

#define F3_ANSW_LEN (8*8)

e_i2l_state i2l_get_state(void);

uint8_t i2l_init(void);
uint8_t i2l_start(void);
uint8_t i2l_set_ready(void);
uint8_t i2l_set_data(void);
uint8_t i2l_set_data_bit(void);


#endif
