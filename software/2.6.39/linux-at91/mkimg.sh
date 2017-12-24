#!/bin/sh

.  ../set_env.sh

$OBJCOPY -O binary -S vmlinux linux.bin
gzip -f -v9 linux.bin
mkimage -A arm -O linux -C gzip -a 0x20008000 -e 0x20008000 -d linux.bin.gz uImage


#gzip -c -v9 initrd >initrd.gz
#mkimage -T ramdisk -A arm -O linux -C gzip -a 0x20a00000 -e 0x20a00000 -d initrd.gz InitrdImage.gz

exit 0

