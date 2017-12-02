export PATH=/home/andrew/gcc-3.4.6-glibc-2.3.6/arm-linux/bin:$PATH
apt-get install libncurses5-dev

make ARCH=arm CROSS_COMPILE=arm-linux- defconfig
make ARCH=arm CROSS_COMPILE=arm-linux- menuconfig
make ARCH=arm CROSS_COMPILE=arm-linux-
make ARCH=arm CROSS_COMPILE=arm-linux- install CONFIG_PREFIX=/home/andrew/busybox-1.27.2/export/

.config

ash
crontab
less
microcom
tr
vi
