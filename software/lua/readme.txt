lua - with io.popen support

Makefile
luaconf.h  - patches for gcc-3.4.6-glibc-2.3.6; lua root in /usr/

get_temp.lua - execute cron
send_temp.lua - execute by cron

send_temp.sh - invokes by send_temp.lua

luaposix-34.0.1
CC=arm-linux-gcc

luke configured for my usage
unistd.c - commented out linkat
