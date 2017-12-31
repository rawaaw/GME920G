lua - with io.popen support

Makefile
luaconf.h  - patches for gcc-3.4.6-glibc-2.3.6; lua root in /usr/

luaposix-34.0.1
CC=arm-linux-gcc

luke configured for my usage
unistd.c - commented out linkat

====
crontab.tmpl - actual crontab

get_temp.lua  - read sensors //executed by crond
send_temp.lua - control lua script for send SMS //executed by crond
send_temp1.sh - invokes by send_temp.lua (uses custom chat)
read_sms.lua  - read ad drop incoming sms (send temperature if message contains magic word GETT) //executed by crond

send_temp.sh  - not used because bug in busybox chat (TODO: fix that bug)
lock_test.lua - test script for checking fcntl lock works
