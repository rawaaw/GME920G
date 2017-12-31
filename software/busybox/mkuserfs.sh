#!/bin/sh
dd if=/dev/zero bs=1024 count=2048 of=userfs_new.ext2
/sbin/mke2fs -F -m0 userfs_new.ext2

sudo mount -n -o loop userfs.ext2 userfs
sudo mount -n -o loop userfs_new.ext2 userfs_new/
sudo cp -rp userfs/* userfs_new/
sudo umount userfs
sudo umount userfs_new

