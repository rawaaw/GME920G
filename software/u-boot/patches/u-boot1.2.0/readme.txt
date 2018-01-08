make u-boot:

u-boot sources:
ftp://ftp.denx.de/pub/u-boot/u-boot-1.2.0.tar.bz2

1.
bzip2 -dc u-boot-1.2.0.tar.bz2 | tar -xf -andrew@u1604:~/arm/uboot/temp$ patch -p0 <./diff.txt 

2.
patch -p0 <./u-boot-1.2.0-rtl8201.patch

patching file u-boot-1.2.0/board/at91rm9200dk/at91rm9200dk.c
patching file u-boot-1.2.0/config.mk
patching file u-boot-1.2.0/cpu/arm920t/at91rm9200/ether.c
patching file u-boot-1.2.0/cpu/arm920t/at91rm9200/Makefile
patching file u-boot-1.2.0/cpu/arm920t/at91rm9200/rtl8201.c
patching file u-boot-1.2.0/include/configs/at91rm9200dk.h
patching file u-boot-1.2.0/include/flash.h
patching file u-boot-1.2.0/include/rtl8201.h
patching file u-boot-1.2.0/lib_arm/armlinux.c

3. (gcc-3.4.6)
. ./set_env.sh  

4.

cd u-boot-1.2.0/
make distclean 
make at91rm9200dk_config
make


5. load on board for testing:
reset
CCCCCC (xmodem)
loader.bin
xmodem
u_boot.bin


==========================
NOTES:


at91_ether: Your bootloader did not configure a MAC address
http://www.at91.com/viewtopic.php?f=4&t=2658

at91_ether: Your bootloader did not configure a MAC address
Fri Mar 16, 2007 5:17 pm

When booting kernel I've got the message 

"at91_ether: Your bootloader did not configure a MAC address".

Due to that I can not mount root file system throgh NFS.

On other hand , after downloading uImage by tftp and kommand 'bootm' all working well. MAC address in u-boot is configured and written into flash. U-boot sees it too.(printenv shows it). 

On the site 
http://www.gelato.unsw.edu.au/lxr/sourc ... 91_ether.c
I read that 
" - It must always set the MAC address after reset, even if it doesn't
365 * happen to access the Ethernet while it's booting. Some versions of
366 * U-Boot on the AT91RM9200-DK do not do this."

How to workaround all that? Thank you! 
Andrew


in uboot add 
setenv ethaddr 04:25:fe:ed:00:18
before booting linux.

don't forget a saveenv

This mac-address above is the sample mac from atmel
Mw
mwolfram
Contact: Contact mwolfram
Location: Erfurt/Germany
Posts: 87
Joined: Thu Feb 03, 2005 11:41 am
Post
Sat Mar 17, 2007 2:41 pm

Hi,

it's already explained here: phpbb/viewtopic.php?t=2646.

There is also a long discussion about that on the arm-linux mailing list.

So please read the u-boot docs and FAQ.

--
Mike
Senior IT Consultant
Germany
An
AndrewMik
Posts: 22
Joined: Thu Feb 15, 2007 4:42 pm
Post
Sat Mar 17, 2007 4:17 pm

Thank you all! I have fixed the issue.

I have added --->

in at91rm9200dk.c : 

int board_late_init(void)
{
/
/* Fix Ethernet Initialization Bug when starting Linux from U-Boot */
#if (CONFIG_COMMANDS & CFG_CMD_NET)
eth_init(gd->bd);
#endif

return 0;
}

in at91rm9200dk.h :

define CONFIG_COMMANDS	\
((CONFIG_CMD_DFL | CFG_CMD_MII | CFG_CMD_NET | \
CFG_CMD_DHCP ) & \
~(CFG_CMD_BDI | \
CFG_CMD_IMI | \
CFG_CMD_AUTOSCRIPT | \
CFG_CMD_FPGA | \
CFG_CMD_MISC | \
CFG_CMD_LOADS ))


#define CONFIG_DRIVER_ETHER	1

#define BOARD_LATE_INIT	1
