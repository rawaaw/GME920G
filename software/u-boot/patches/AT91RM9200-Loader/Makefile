TOPDIR	:= $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

#CFLAGS = -Os -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fno-inline -fno-common -mshort-load-bytes -msoft-float -D__arm__ -D__FLASHED__ -I$(TOPDIR)/include
CFLAGS = -Os -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fno-inline -fno-common -malignment-traps -msoft-float -D__arm__ -D__FLASHED__ -I$(TOPDIR)/include
LDFLAGS = -nostdlib -Tld.script

CROSS_COMPILE=arm-linux-
CC=$(CROSS_COMPILE)gcc

OBJ =  entry.o div0.o _udivsi3.o _umodsi3.o init.o crt0.o main.o asm_isr.o jump.o \
       lib_AT91RM9200.o

all:	loader.bin

loader.bin:	loader.text loader.data
	cat loader.text loader.data > $@

loader.text:	loader
	$(CROSS_COMPILE)objcopy -O binary -j .text $< $@

loader.data:	loader
	$(CROSS_COMPILE)objcopy -O binary -j .data $< $@

loader:	$(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

crt0.o:	crt0.S
	$(CC) $(CFLAGS) -c $<

entry.o:	entry.S
	$(CC) $(CFLAGS) -c $<

_udivsi3.o:	_udivsi3.S
	$(CC) $(CFLAGS) -c $<

div0.o:	div0.c
	$(CC) $(CFLAGS) -c $<

_umodsi3.o:	_umodsi3.S
	$(CC) $(CFLAGS) -c $<

init.o:	init.c
	$(CC) $(CFLAGS) -c $<

main.o:	main.c
	$(CC) $(CFLAGS) -c $<

lib_AT91RM9200.o:	lib_AT91RM9200.c
	$(CC) $(CFLAGS) -c $<

asm_isr.o: 	asm_isr.S
	$(CC) $(CFLAGS) -c $<
	
jump.o: 	jump.S
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f loader loader.text loader.data *.o *~
