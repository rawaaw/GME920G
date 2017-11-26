/* 
* i2c over pca9533 
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "i2c_gpio.h"

static uint8_t s_gpio_line_state = 0;
static int s_i2c = -1;

/* got from i2c-tools_3.1.1.orig.tar */
static inline int i2c_smbus_access(int file, char read_write, uint8_t command,
                                     int size, union i2c_smbus_data *data){
  struct i2c_smbus_ioctl_data args;

  args.read_write = read_write;
  args.command = command;
  args.size = size;
  args.data = data;
  return ioctl(file,I2C_SMBUS,&args);
}


static inline int i2c_smbus_read_byte(int file){
  union i2c_smbus_data data;
  if (i2c_smbus_access(file,I2C_SMBUS_READ,0x00 /* INPUT */,I2C_SMBUS_BYTE_DATA,&data))
#if defined BEBUG_ON && DEBUG_ON >=3
    printf("i2c_smbus_read_byte() error: %d\n", errno);
#endif
    return -1;
  else
    return 0x0FF & data.byte;
}

static inline int i2c_smbus_write_byte(int file, uint8_t value){
  union i2c_smbus_data data;
  data.byte = value;
  return i2c_smbus_access(file,I2C_SMBUS_WRITE,0x05 /* LS0 */,
                          I2C_SMBUS_BYTE_DATA, &data);
}



int pca9533_open(void){
  ssize_t rd;
  uint8_t io_buf[3];

  s_gpio_line_state = 0;

  s_i2c = open(I2C_ADAPTER_DEV, O_RDWR);

  if (s_i2c == -1){
#if defined DEBUG_ON && DEBUG_ON >=3
    printf("->pca9533_open(open): errno=%d\n", errno);
#endif
    return -1;
  }

  if (ioctl(s_i2c, I2C_SLAVE, PCA9533_I2C_ADDR) < 0) {
    if (ioctl(s_i2c, I2C_SLAVE_FORCE, PCA9533_I2C_ADDR) < 0) {
#if defined DEBUG_ON && DEBUG_ON >=3
      printf("->pca9533_open(ioctl): errno=%d\n", errno);
#endif
      close(s_i2c);
      s_i2c = -1;
      return -1;
    }
  }

  s_gpio_line_state = i2c_smbus_read_byte(s_i2c);
  return 0;
}

uint8_t pca9533_read(void){
  ssize_t rd;
  uint8_t io_buf[3];

  s_gpio_line_state = i2c_smbus_read_byte(s_i2c);
  return s_gpio_line_state;
}

int pca9533_write(uint8_t line_state){
  int ret = 0;

  uint8_t ls0_reg = (~line_state & GPIO_LINE1) | ((~line_state & GPIO_LINE2) << 1) | ((~line_state & GPIO_LINE3) << 2);

  s_gpio_line_state = line_state;

  ret = i2c_smbus_write_byte(s_i2c, ls0_reg);
  return ret;
}

int pca9533_close(void){
  if (s_i2c != -1)
    close(s_i2c);
  s_i2c = -1;
  s_gpio_line_state = 0;
  return 0;
}

/* SCL - digital i/o: 1*/
int scl_high(void){
  int ret;
  ret = pca9533_write((s_gpio_line_state | GPIO_LINE1));
#if defined DEBUG_ON && DEBUG_ON >=2
  printf("->>>>scl_high:SCL=1 SDA1=%d SDA2=%d\n", ((s_gpio_line_state & GPIO_LINE2) >> 1), ((s_gpio_line_state & GPIO_LINE3) >> 2));
#endif
  return ret;
}

int scl_low(void){
  int ret;
  ret = pca9533_write((s_gpio_line_state & GPIO_INV_LINE1));
#if defined DEBUG_ON && DEBUG_ON >=2
  printf("->>>>scl_low: SCL=0 SDA1=%d SDA2=%d\n", ((s_gpio_line_state & GPIO_LINE2) >> 1), ((s_gpio_line_state & GPIO_LINE3) >> 2));
#endif
  return ret;
}

/* SDA - digital i/o: 2*/
/* line: GPIO_LINE2 GPIO_LINE3 */
int sda_high(uint8_t line){
  int ret;
  ret = pca9533_write((s_gpio_line_state | line/*GPIO_LINE2*/));
#if defined DEBUG_ON && DEBUG_ON >=2
  printf("->>>>sda_high:SCL=%d SDA1=%d SDA2=%d\n", (s_gpio_line_state & GPIO_LINE1), ((s_gpio_line_state & GPIO_LINE2) >> 1), ((s_gpio_line_state & GPIO_LINE3) >> 2));
#endif
  return ret;
}

/* line: GPIO_INV_LINE2 GPIO_INV_LINE3 */
int sda_low(uint8_t inv_line){
  int ret;
  ret = pca9533_write((s_gpio_line_state & inv_line/*GPIO_INV_LINE2*/));
#if defined DEBUG_ON && DEBUG_ON >=2
  printf("->>>>sda_low: SCL=%d SDA1=%d SDA2=%d\n", (s_gpio_line_state & GPIO_LINE1), ((s_gpio_line_state & GPIO_LINE2) >> 1), ((s_gpio_line_state & GPIO_LINE3) >> 2));
#endif
  return ret;
}

/* line: GPIO_LINE2 GPIO_LINE3 */
uint8_t sda_read(uint8_t line){
  int ret;
  uint8_t rd = pca9533_read();
#if defined DEBUG_ON && DEBUG_ON >=2
  printf("->>>>>>sda_read:rd=%.2X SCL=%d SDA1=%d SDA2=%d\n", rd, (rd & GPIO_LINE1), ((rd & GPIO_LINE2) >> 1), ((rd & GPIO_LINE3) >> 2));
#endif
  if (rd == 0xFF)
    return 0xFF;
  if (line & GPIO_LINE2)
    return (((rd & GPIO_LINE2) >> 1) & 0x01);
  else if (line & GPIO_LINE3)
    return (((rd & GPIO_LINE3) >> 2) & 0x01);
  else
    return 0xFF;
}

