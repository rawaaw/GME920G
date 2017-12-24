самое свежее ядро от linux4sam

0. сначала нужно пропатчить загрузчик.
Иначе ядро при старте падает (см. раздел ../u-boot).

1.
wget -c https://releases.linaro.org/components/toolchain/binaries/4.9-2017.01/arm-linux-gnueabi/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabi.tar.xz
tar -xf gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabi.tar.xz 

2.
#git init
git clone -b linux-2.6.39-at91 --single-branch git://github.com/linux4sam/linux-at91.git 
or
git clone -b linux-2.6.39-at91 --single-branch https://github.com/linux4sam/linux-at91.git 


3. . ./set_env.sh

4. патчи, конфиг GME920G

andrew@u1604:~/arm/linux-at91$ git checkout
M       arch/arm/lib/memset.S
M       arch/arm/mach-at91/Makefile
M       arch/arm/mach-at91/board-rm9200dk.c
M       drivers/spi/atmel_spi.c
M       kernel/timeconst.pl
Your branch is up-to-date with 'origin/linux-2.6.39-at91'.


5. 
make ARCH=arm at91rm9200_defconfig
make ARCH=arm menuconfig      !!! <- GME920G
make ARCH=arm
