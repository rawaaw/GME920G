#include <stdio.h>
#include <errno.h>
#include "i2c_gpio.h"


void am2320_delay(uint32_t val){
  if (val > 1)
    usleep(val);
  return;
}

uint8_t am2320_wakeup(uint8_t scl_gpio_line, uint8_t scl_gpio_inv_line, uint8_t gpio_line, uint8_t gpio_inv_line){
  uint8_t ack;
  int i;

#if defined DO_PRINTOUT && DO_PRINTOUT!=0
  printf("WAKEUP\n");
#endif
  /* i2c init */
  sda_high(gpio_line);
  scl_high(scl_gpio_line);
  //am2320_delay(2000);
  /* start:*/ 
  sda_low(gpio_inv_line);
  scl_low(scl_gpio_inv_line);
  
  /* address: */   
  for (i = 7; i >= 0; i --){
    if (AM_ADDR & (1 << i)){
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
  scl_low(scl_gpio_inv_line);
  sda_low(gpio_inv_line);
  scl_high(scl_gpio_line);
  sda_high(gpio_line); /*<- stop*/
  return 0; 
}


uint8_t am2320_address(uint8_t rd_bit, uint8_t scl_gpio_line, uint8_t scl_gpio_inv_line, uint8_t gpio_line, uint8_t gpio_inv_line){
  uint8_t ack;
  uint8_t addr = AM_ADDR | rd_bit;
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
//  am2320_delay(1);
//  sda_high(gpio_line);
  return ack;
}

uint8_t am2320_command(uint8_t cmd, uint8_t scl_gpio_line, uint8_t scl_gpio_inv_line, uint8_t gpio_line, uint8_t gpio_inv_line){
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
//  am2320_delay(1);
//  sda_high(gpio_line);
  return ack;
}

uint8_t am2320_answer(uint8_t last_byte, uint8_t scl_gpio_line, uint8_t scl_gpio_inv_line, uint8_t gpio_line, uint8_t gpio_inv_line){
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

uint16_t crc16(uint8_t *ptr, uint8_t len){
  uint16_t crc =0xFFFF;
  uint8_t i;
  while(len--){
    crc ^=*ptr++;
    for(i=0;i<8;i++){
      if(crc & 0x01){
        crc >>= 1;
        crc ^= 0xA001;
      }else{
        crc>>=1;
      }
    }
  }
  return crc;
}


int main(int argc, char**argv){
  int  i = 0;
  int ret_code = 0;

  uint8_t am2320_f3_answ[8];
  uint8_t scl_line, scl_inv_line, sda_line, sda_inv_line;
  uint16_t crc;

/*  printf("%s\n", argv[1]);*/
  if (argc != 2 || (argv[1][0] != '1' && argv[1][0] != '2' && argv[1][0] != 'e')){
    printf("Usage: am2320term {1,2,e}\n");
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
    printf("HUM%c:%0.1f TEMP%c:%0.1f\n", 
           argv[1][0],
           89.1, 
           argv[1][0],
           10.1);
    return 0;
  }

  if (pca9533_open() == 0){

    /* am2320 wakeup:  */
    am2320_wakeup(scl_line, scl_inv_line, sda_line, sda_inv_line);
    
    /* workong: */
    am2320_address(0x00, scl_line, scl_inv_line, sda_line, sda_inv_line); // WR
    am2320_command(0x03, scl_line, scl_inv_line, sda_line, sda_inv_line); // function code
    am2320_command(0x00, scl_line, scl_inv_line, sda_line, sda_inv_line); // staring address 
    am2320_command(0x04, scl_line, scl_inv_line, sda_line, sda_inv_line); // register length
    sda_stop(scl_line, scl_inv_line, sda_line, sda_inv_line);
    am2320_delay(2); //wait for collecting data
    am2320_address(0x01, scl_line, scl_inv_line, sda_line, sda_inv_line); // RD
    am2320_f3_answ[0] = am2320_answer(0, scl_line, scl_inv_line, sda_line, sda_inv_line);
    am2320_f3_answ[1] = am2320_answer(0, scl_line, scl_inv_line, sda_line, sda_inv_line);
    am2320_f3_answ[2] = am2320_answer(0, scl_line, scl_inv_line, sda_line, sda_inv_line);
    am2320_f3_answ[3] = am2320_answer(0, scl_line, scl_inv_line, sda_line, sda_inv_line);
    am2320_f3_answ[4] = am2320_answer(0, scl_line, scl_inv_line, sda_line, sda_inv_line);
    am2320_f3_answ[5] = am2320_answer(0, scl_line, scl_inv_line, sda_line, sda_inv_line);
    am2320_f3_answ[6] = am2320_answer(0, scl_line, scl_inv_line, sda_line, sda_inv_line);
    am2320_f3_answ[7] = am2320_answer(1, scl_line, scl_inv_line, sda_line, sda_inv_line);
    sda_stop(scl_line, scl_inv_line, sda_line, sda_inv_line);
    am2320_delay(6);

    crc = crc16(am2320_f3_answ, 6);

    if ((am2320_f3_answ[6] == (crc & 0xFF)) && (am2320_f3_answ[7] == ((crc >> 8) & 0xFF))){
      printf("HUM%c:%0.1f TEMP%c:%0.1f\n", 
             argv[1][0],
             (am2320_f3_answ[2]*256 + am2320_f3_answ[3])/10.0, 
             argv[1][0],
             (am2320_f3_answ[4]*256 + am2320_f3_answ[5])/10.0);
      ret_code = 0;
    }else{
      ret_code = 1;
    }

    pca9533_close();
  }else{
    printf("pca9533 open error: %s\n", strerror(errno));
    ret_code = 1;
  }
  exit(ret_code);
  return 0;
}
