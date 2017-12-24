#!/bin/sh

. ./set_env.sh

#$OBJCOPY -O binary -S linux-at91/vmlinux linux.bin
#gzip -f -v9 linux.bin
#mkimage -A arm -O linux -C gzip -a 0x20008000 -e 0x20008000 -d linux.bin.gz uImage


gzip -c -v9 ../busybox/initrd131_cust >initrd131_cust.gz
mkimage -T ramdisk -A arm -O linux -C gzip -a 0x20a00000 -e 0x20a00000 -d initrd131_cust.gz linux-at91/initrd.gz

exit 0

