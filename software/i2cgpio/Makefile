CC=arm-linux-gcc
CFLAGS=-DDEBUG_ON=1 -O2 -D_GNU_SOURCE
#CFLAGS= -O2

#.c.o:
#	$(CC) -c $<

#i2c_gpio.o:i2c_gpio.c i2c_gpio.h
#	$(CC) -c $(CFLAGS) i2c_gpio.c -o i2c_gpio.o

i2c_gpio_sysctl.o:i2c_gpio_sysctl.c i2c_gpio.h
	$(CC) -c $(CFLAGS) i2c_gpio_sysctl.c -o i2c_gpio_sysctl.o

i2c_gpio_example.o:i2c_gpio_example.c i2c_gpio.h
	$(CC) -c $(CFLAGS) i2c_gpio_example.c -o i2c_gpio_example.o

i2c_gpio_term.o:i2c_gpio_term.c i2c_gpio.h
	$(CC) -c $(CFLAGS) i2c_gpio_term.c -o i2c_gpio_term.o

i2c_lite_gpio_example.o:i2c_lite_gpio_example.c i2c_gpio.h
	$(CC) -c $(CFLAGS) i2c_lite_gpio_example.c -o i2c_lite_gpio_example.o

#i2cgpio:i2c_gpio_sysctl.o i2c_lite_gpio_example.o
#	$(CC) -o i2cgpio i2c_gpio_sysctl.o i2c_lite_gpio_example.o

i2cgpio:i2c_gpio_sysctl.o i2c_gpio_example.o
	$(CC) -o i2cgpio i2c_gpio_sysctl.o i2c_gpio_example.o

am2320term:i2c_gpio_sysctl.o i2c_gpio_term.o
	$(CC) -o am2320term i2c_gpio_sysctl.o i2c_gpio_term.o

#i2cgpio:i2c_gpio.o i2c_gpio_example.o
#	$(CC) -o i2cgpio i2c_gpio.o i2c_gpio_example.o

#all:i2cgpio
all:am2320term

clean:
	rm -f *.o
	rm -f i2cgpio
	rm -f am2320term


	