export PATH=/home/andrew/gcc-3.4.6-glibc-2.3.6/arm-linux/bin:$PATH
apt-get install libncurses5-dev

. ./set_env.sh
make ARCH=arm CROSS_COMPILE=arm-linux- defconfig
make ARCH=arm CROSS_COMPILE=arm-linux- menuconfig
  -- use gme920g_full --
make ARCH=arm CROSS_COMPILE=arm-linux-
make ARCH=arm CROSS_COMPILE=arm-linux- install CONFIG_PREFIX=/home/andrew/busybox-1.27.2/export/

mkrootfs.sh - make rootfs image
mkuserfs.sh - make userfs image

gme920g_full:

[
[[
addgroup
adduser
ash
cat
chat
chmod
chown
cp
cpio
crond
crontab
date
dd
delgroup
deluser
df
dumpleases
echo
false
flock
fsck
grep
gunzip
gzip
hostname
hwclock
i2cdetect
i2cdump
i2cget
i2cset
ifconfig
inetd
init
insmod
iplink
kill
klogd
less
ln
login
ls
lsmod
md5sum
microcom
mkdir
mkfs.ext2
mknod
modinfo
modprobe
more
mount
mv
netstat
nohup
nslookup
passwd
pidof
ping
printf
ps
pwd
reboot
reset
rm
rmdir
rmmod
route
sed
sh
sha1sum
sleep
stat
stty
sum
sync
sysctl
syslogd
tail
tar
tee
telnet
telnetd
test
tftp
touch
tr
traceroute
true
udhcpd
umount
uptime
usleep
uudecode
uuencode
vi
watchdog
wc
wget
who
whoami
xargs
zcat
