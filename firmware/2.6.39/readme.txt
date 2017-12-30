ядро 2.6.39 и кастомизированный initrd (свежий busybox, wdogd)

прошивка из u-boot:

tftpboot 21008000 uImage
tftpboot 21a00000 initrd.gz

erase 10020000 101AFFFF
cp.b 20008000 10020000 180E83

erase 10200000 1041FFFF
cp.b 20a00000 10200000 20B0BA

