ядро 2.6.39 и кастомизированный initrd (свежий busybox, wdogd, mdmrst)

прошивка из u-boot:

tftpboot 20008000 uImage
tftpboot 20a00000 initrd.gz

check:
iminfo 0x20008000
iminfo 0x20a00000

erase 10020000 101AFFFF
cp.b 20008000 10020000 180E83

erase 10200000 1041FFFF
cp.b 20a00000 10200000 20B563

====

userfs.ext2 - actual userfs.

INSTALL:

on GME920G box console:

cd /tmp
umount /mnt/userfs/
tftp -r userfs.ext2 -g 192.168.1.252
savefs
