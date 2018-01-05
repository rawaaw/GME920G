#if !defined __I2C_GPIO_H__

#define AM_ADDR 0xB8
#define LM_ADDR 0x90
                    
#define GPIO_LINE1 0x01
#define GPIO_LINE2 0x02
#define GPIO_LINE3 0x04
#define GPIO_LINE4 0x08

#define GPIO_INV_LINE1 0xFE
#define GPIO_INV_LINE2 0xFD
#define GPIO_INV_LINE3 0xFB
#define GPIO_INV_LINE4 0xF7

#define PCA9533_INP_REG  "/sys/bus/i2c/devices/0-0062/input"
#define PCA9533_LS0_REG  "/sys/bus/i2c/devices/0-0062/ls0"
#define I2C_ADAPTER_DEV  "/dev/i2c-0"
#define PCA9533_I2C_ADDR 0x62
#define PCA9533_INPUT_REG 0
#define CA9533_LS0_REG    3

#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0
#define I2C_SMBUS	0x0720	/* SMBus transfer */
#define I2C_SLAVE	0x0703	/* Use this slave address */
#define I2C_SLAVE_FORCE	0x0706	/* Use this slave address, even if it
				   is already in use by a driver! */

#define I2C_SMBUS_BYTE		    1
#define I2C_SMBUS_BYTE_DATA	    2


typedef signed int int32_t;
typedef unsigned int uint32_t;

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;


/* This is the structure as used in the I2C_SMBUS ioctl call */
struct i2c_smbus_ioctl_data {
	uint8_t read_write;
	uint8_t command;
	int32_t size;
	union i2c_smbus_data *data;
};

/* This is the structure as used in the I2C_RDWR ioctl call */
struct i2c_rdwr_ioctl_data {
	struct i2c_msg *msgs;	/* pointers to i2c_msgs */
	int32_t nmsgs;			/* number of i2c_msgs */
};


#define I2C_SMBUS_BLOCK_MAX	32	/* As specified in SMBus standard */
#define I2C_SMBUS_I2C_BLOCK_MAX	32	/* Not specified but we use same structure */
union i2c_smbus_data {
	uint8_t byte;
	uint16_t word;
        uint8_t block[I2C_SMBUS_BLOCK_MAX + 2]; /* block[0] is used for length */
	                                            /* and one more for PEC */
};

int pca9533_open(void);
uint8_t pca9533_read(void);
int pca9533_write(uint8_t line_status);
int pca9533_close(void);

int scl_high(uint8_t line);
int scl_low(uint8_t inv_line);
int sda_high(uint8_t line);
int sda_low(uint8_t inv_line);
uint8_t sda_read(uint8_t line);


#endif /*__I2C_GPIO_H__*/
