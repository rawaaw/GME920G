#include <stdio.h>
#include <errno.h>
#include "i2c_gpio.h"

//#define DO_PRINTOUT 1
#undef DO_PRINTOUT

void lm75a_delay(uint32_t val){
  if (val > 1)
    usleep(val);
  return;
}

uint8_t lm75a_address(uint8_t rd_bit, uint8_t scl_gpio_line, uint8_t scl_gpio_inv_line, uint8_t gpio_line, uint8_t gpio_inv_line){
  uint8_t ack;
  uint8_t addr = LM_ADDR | rd_bit;
  int i;

  /* start:*/ 
  sda_low(gpio_inv_line);
  scl_low(scl_gpio_inv_line);
#if defined DO_PRINTOUT && DO_PRINTOUT!=0
  printf("START\n");
#endif
  
  /* address: */   
  for (i = 7; i >= 0; i --){
    if (addr & (1 << i)){
      sda_high(gpio_line);
    }else{
      sda_low(gpio_inv_line);
    }      
    scl_high(scl_gpio_line);
    scl_low(scl_gpio_inv_line);
  }    
#if defined DO_PRINTOUT && DO_PRINTOUT!=0
  printf("ADDR: 0x%.2X\n", addr);
#endif
  /* ack */
  sda_high(gpio_line);
  scl_high(scl_gpio_line);
  ack = sda_read(gpio_line);
  if (ack == 0){
#if defined DO_PRINTOUT && DO_PRINTOUT!=0
    printf("ACK\n");
#endif
//    sda_low(gpio_inv_line);
  }else{
#if defined DO_PRINTOUT && DO_PRINTOUT!=0
    printf("NACK\n");
#endif
  }
  scl_low(scl_gpio_inv_line);
//  lm75a_delay(1);
//  sda_high(gpio_line);
  return ack;
}

uint8_t lm75a_command(uint8_t cmd, uint8_t scl_gpio_line, uint8_t scl_gpio_inv_line, uint8_t gpio_line, uint8_t gpio_inv_line){
  uint8_t ack;
  int i;

#if defined DO_PRINTOUT && DO_PRINTOUT!=0
  printf("CMD: 0x%.2X\n", cmd);
#endif
  for (i = 7; i >= 0; i --){
    if (cmd & (1 << i)){
      sda_high(gpio_line);
    }else{
      sda_low(gpio_inv_line);
    }      
    scl_high(scl_gpio_line);
    scl_low(scl_gpio_inv_line);
  }    
  /* ack */
  sda_high(gpio_line);
  scl_high(scl_gpio_line);
  ack = sda_read(gpio_line);
  if (ack == 0){
#if defined DO_PRINTOUT && DO_PRINTOUT!=0
    printf("ACK\n");
#endif
    sda_low(gpio_inv_line);
  }else{
#if defined DO_PRINTOUT && DO_PRINTOUT!=0
    printf("NACK\n");
#endif
  }
  scl_low(scl_gpio_inv_line);
//  lm75a_delay(1);
//  sda_high(gpio_line);
  return ack;
}

uint8_t lm75a_answer(uint8_t last_byte, uint8_t scl_gpio_line, uint8_t scl_gpio_inv_line, uint8_t gpio_line, uint8_t gpio_inv_line){
  uint8_t ack = 0;
  uint8_t answer = 0;
  int i;

#if defined DO_PRINTOUT && DO_PRINTOUT!=0
  printf("before READ\n");
#endif
  for (i = 7; i >= 0; i --){
    sda_high(gpio_line); /* bug in pca:  */
    sda_high(gpio_line); 
    scl_high(scl_gpio_line);
    ack = sda_read(gpio_line);
    answer |= ((ack & 0x1) << i);
    scl_low(scl_gpio_inv_line);
  }    
  if (!last_byte){
#if defined DO_PRINTOUT && DO_PRINTOUT!=0
    printf("master ACK\n");
#endif
    sda_low(gpio_inv_line); //master ACK
  }else{
#if defined DO_PRINTOUT && DO_PRINTOUT!=0
    printf("master NACK\n");
#endif
  }
  scl_high(scl_gpio_line);
  scl_low(scl_gpio_inv_line);
  if (!last_byte){
    sda_high(gpio_line);
  } 
#if defined DO_PRINTOUT && DO_PRINTOUT!=0
  printf("READ: 0x%.2X\n", answer);
#endif
  return answer;
}

uint8_t sda_stop(uint8_t scl_gpio_line, uint8_t scl_gpio_inv_line, uint8_t gpio_line, uint8_t gpio_inv_line){
#if defined DO_PRINTOUT && DO_PRINTOUT!=0
  printf("STOP\n");
#endif
  scl_high(scl_gpio_line);
  sda_high(gpio_line);
  return 0;
}

int main(int argc, char**argv){
  int ret_code = 0;
  float temp = 0;

  uint8_t lm75a_answ[8];
  uint8_t scl_line, scl_inv_line, sda_line, sda_inv_line;
  uint16_t crc;

/*  printf("%s\n", argv[1]);*/
  if (argc != 2 || (argv[1][0] != '1' && argv[1][0] != '2' && argv[1][0] != 'e')){
    printf("Usage: lm75aterm {1,2,e}\n");
    exit(1);
  }
  if (argv[1][0] == '1'){
    scl_line = GPIO_LINE1;
    scl_inv_line = GPIO_INV_LINE1;
    sda_line = GPIO_LINE2;
    sda_inv_line = GPIO_INV_LINE2;
  }else if (argv[1][0] == '2'){
    scl_line = GPIO_LINE3;
    scl_inv_line = GPIO_INV_LINE3;
    sda_line = GPIO_LINE4;
    sda_inv_line = GPIO_INV_LINE4;
  }else{
    /* emulator mode */
    printf("TEMP%c:%0.1f\n", 
           argv[1][0],
           10.5);
    return 0;
  }

  if (pca9533_open() == 0){

    lm75a_address(0x00, scl_line, scl_inv_line, sda_line, sda_inv_line); // WR
    lm75a_command(0x00, scl_line, scl_inv_line, sda_line, sda_inv_line); // select register 0 (temperature)
    sda_stop(scl_line, scl_inv_line, sda_line, sda_inv_line);

    lm75a_address(0x01, scl_line, scl_inv_line, sda_line, sda_inv_line); // RD
    lm75a_answ[0] = lm75a_answer(0, scl_line, scl_inv_line, sda_line, sda_inv_line); //
    lm75a_answ[1] = lm75a_answer(0, scl_line, scl_inv_line, sda_line, sda_inv_line); // only MSB: 0.5

    sda_stop(scl_line, scl_inv_line, sda_line, sda_inv_line);

#if defined DO_PRINTOUT && DO_PRINTOUT!=0
    printf("raw:%.2X %.2X\n", lm75a_answ[0], lm75a_answ[1]);
#endif

    if (! (lm75a_answ[0] & 0x80)){  // temperatute >= 0
      temp = (float)lm75a_answ[0] + (lm75a_answ[1] >> 7) * 0.5;
    }else{
      temp = (0x100 - (unsigned int)lm75a_answ[0])*(-1.0) + (lm75a_answ[1] >> 7) * 0.5;
    }

    printf("TEMP%c:%0.1f\n", 
            argv[1][0], temp);
    ret_code = 0;

    pca9533_close();

  }else{
    printf("pca9533 open error: %s\n", strerror(errno));
    ret_code = 1;
  }
  exit(ret_code);
  return 0;
}
