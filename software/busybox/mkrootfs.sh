#!/bin/sh
dd if=/dev/zero bs=1024 count=10000 of=initrd
/sbin/mke2fs -F -m0 initrd

sudo mount -n -o loop initrd131_cust rootfs
sudo mount -n -o loop initrd rootfs_new/
sudo cp -rp rootfs/* rootfs_new/
sudo umount rootfs
sudo umount rootfs_new

gzip -c -v9 initrd >../2.6.39/linux-at91/initrd_cust.gz
mkimage -T ramdisk -A arm -O linux -C gzip -a 0x20a00000 -e 0x20a00000 -d ../2.6.39/linux-at91/initrd_cust.gz ../2.6.39/linux-at91/initrd.gz
