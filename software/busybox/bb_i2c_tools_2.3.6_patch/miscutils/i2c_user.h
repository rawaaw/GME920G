/* ------------------------------------------------------------------------- */
/* 									     */
/* i2c.h - definitions for the i2c-bus interface			     */
/* 									     */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 1995-2000 Simon G. Vogl

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.		     */
/* ------------------------------------------------------------------------- */

/* With some changes from Kyösti Mälkki <kmalkki@cc.hut.fi> and
   Frodo Looijaard <frodol@dds.nl> */

#ifndef _LINUX_I2C_USER_H
#define _LINUX_I2C_USER_H

#include <linux/types.h>
/*
 * I2C Message - used for pure i2c transaction, also from /dev interface
 */
struct i2c_msg {
	__u16 addr;	/* slave address			*/
 	__u16 flags;		
#define I2C_M_TEN	0x10	/* we have a ten bit chip address	*/
#define I2C_M_RD	0x01
#define I2C_M_NOSTART	0x4000
#define I2C_M_REV_DIR_ADDR	0x2000
#define I2C_M_IGNORE_NAK	0x1000
#define I2C_M_NO_RD_ACK		0x0800
 	__u16 len;		/* msg length				*/
 	__u8 *buf;		/* pointer to msg data			*/
};

/* To determine what functionality is present */

#define I2C_FUNC_I2C			0x00000001
#define I2C_FUNC_10BIT_ADDR		0x00000002
#define I2C_FUNC_PROTOCOL_MANGLING	0x00000004 /* I2C_M_{REV_DIR_ADDR,NOSTART,..} */
#define I2C_FUNC_SMBUS_HWPEC_CALC	0x00000008 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_BLOCK_PROC_CALL	0x00008000 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_QUICK		0x00010000 
#define I2C_FUNC_SMBUS_READ_BYTE	0x00020000 
#define I2C_FUNC_SMBUS_WRITE_BYTE	0x00040000 
#define I2C_FUNC_SMBUS_READ_BYTE_DATA	0x00080000 
#define I2C_FUNC_SMBUS_WRITE_BYTE_DATA	0x00100000 
#define I2C_FUNC_SMBUS_READ_WORD_DATA	0x00200000 
#define I2C_FUNC_SMBUS_WRITE_WORD_DATA	0x00400000 
#define I2C_FUNC_SMBUS_PROC_CALL	0x00800000 
#define I2C_FUNC_SMBUS_READ_BLOCK_DATA	0x01000000 
#define I2C_FUNC_SMBUS_WRITE_BLOCK_DATA 0x02000000 
#define I2C_FUNC_SMBUS_READ_I2C_BLOCK	0x04000000 /* I2C-like block xfer  */
#define I2C_FUNC_SMBUS_WRITE_I2C_BLOCK	0x08000000 /* w/ 1-byte reg. addr. */
#define I2C_FUNC_SMBUS_READ_I2C_BLOCK_2	 0x10000000 /* I2C-like block xfer  */
#define I2C_FUNC_SMBUS_WRITE_I2C_BLOCK_2 0x20000000 /* w/ 2-byte reg. addr. */

#define I2C_FUNC_SMBUS_BYTE (I2C_FUNC_SMBUS_READ_BYTE | \
                             I2C_FUNC_SMBUS_WRITE_BYTE)
#define I2C_FUNC_SMBUS_BYTE_DATA (I2C_FUNC_SMBUS_READ_BYTE_DATA | \
                                  I2C_FUNC_SMBUS_WRITE_BYTE_DATA)
#define I2C_FUNC_SMBUS_WORD_DATA (I2C_FUNC_SMBUS_READ_WORD_DATA | \
                                  I2C_FUNC_SMBUS_WRITE_WORD_DATA)
#define I2C_FUNC_SMBUS_BLOCK_DATA (I2C_FUNC_SMBUS_READ_BLOCK_DATA | \
                                   I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)
#define I2C_FUNC_SMBUS_I2C_BLOCK (I2C_FUNC_SMBUS_READ_I2C_BLOCK | \
                                  I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)
#define I2C_FUNC_SMBUS_I2C_BLOCK_2 (I2C_FUNC_SMBUS_READ_I2C_BLOCK_2 | \
                                    I2C_FUNC_SMBUS_WRITE_I2C_BLOCK_2)

#define I2C_FUNC_SMBUS_EMUL (I2C_FUNC_SMBUS_QUICK | \
                             I2C_FUNC_SMBUS_BYTE | \
                             I2C_FUNC_SMBUS_BYTE_DATA | \
                             I2C_FUNC_SMBUS_WORD_DATA | \
                             I2C_FUNC_SMBUS_PROC_CALL | \
                             I2C_FUNC_SMBUS_WRITE_BLOCK_DATA | \
                             I2C_FUNC_SMBUS_I2C_BLOCK)

/* 
 * Data for SMBus Messages 
 */
#define I2C_SMBUS_BLOCK_MAX	32	/* As specified in SMBus standard */	
union i2c_smbus_data {
	__u8 byte;
	__u16 word;
	__u8 block[I2C_SMBUS_BLOCK_MAX + 2]; /* block[0] is used for length */
	                       /* and one more for user-space compatibility */
};

/* smbus_access read or write markers */
#define I2C_SMBUS_READ	1
#define I2C_SMBUS_WRITE	0

/* SMBus transaction types (size parameter in the above functions) 
   Note: these no longer correspond to the (arbitrary) PIIX4 internal codes! */
#define I2C_SMBUS_QUICK		    0
#define I2C_SMBUS_BYTE		    1
#define I2C_SMBUS_BYTE_DATA	    2 
#define I2C_SMBUS_WORD_DATA	    3
#define I2C_SMBUS_PROC_CALL	    4
#define I2C_SMBUS_BLOCK_DATA	    5
#define I2C_SMBUS_I2C_BLOCK_DATA    6
#define I2C_SMBUS_BLOCK_PROC_CALL   7		/* SMBus 2.0 */


/* ----- commands for the ioctl like i2c_command call:
 * note that additional calls are defined in the algorithm and hw 
 *	dependent layers - these can be listed here, or see the 
 *	corresponding header files.
 */
				/* -> bit-adapter specific ioctls	*/
#define I2C_RETRIES	0x0701	/* number of times a device address      */
				/* should be polled when not            */
                                /* acknowledging 			*/
#define I2C_TIMEOUT	0x0702	/* set timeout - call with int 		*/


/* this is for i2c-dev.c	*/
#define I2C_SLAVE	0x0703	/* Change slave address			*/
				/* Attn.: Slave address is 7 or 10 bits */
#define I2C_SLAVE_FORCE	0x0706	/* Change slave address			*/
				/* Attn.: Slave address is 7 or 10 bits */
				/* This changes the address, even if it */
				/* is already taken!			*/
#define I2C_TENBIT	0x0704	/* 0 for 7 bit addrs, != 0 for 10 bit	*/

#define I2C_FUNCS	0x0705	/* Get the adapter functionality */
#define I2C_RDWR	0x0707	/* Combined R/W transfer (one stop only)*/
#define I2C_PEC		0x0708	/* != 0 for SMBus PEC                   */

#define I2C_SMBUS	0x0720	/* SMBus-level access */

/* ----- I2C-DEV: char device interface stuff ------------------------- */

#define I2C_MAJOR	89		/* Device major number		*/

/* These defines are used for probing i2c client addresses */
/* The length of the option lists */
#define I2C_CLIENT_MAX_OPTS 48

/* Default fill of many variables */
#define I2C_CLIENT_DEFAULTS {I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END, \
                          I2C_CLIENT_END, I2C_CLIENT_END, I2C_CLIENT_END}

/* I2C_CLIENT_MODULE_PARM creates a module parameter, and puts it in the
   module header */

#define I2C_CLIENT_MODULE_PARM(var,desc) \
  static unsigned short var[I2C_CLIENT_MAX_OPTS] = I2C_CLIENT_DEFAULTS; \
  static unsigned int var##_num; \
  module_param_array(var, short, &var##_num, 0); \
  MODULE_PARM_DESC(var,desc)

#define I2C_CLIENT_MODULE_PARM_FORCE(name)				\
I2C_CLIENT_MODULE_PARM(force_##name,					\
		       "List of adapter,address pairs which are "	\
		       "unquestionably assumed to contain a `"		\
		       # name "' chip")


#define I2C_CLIENT_INSMOD_COMMON					\
I2C_CLIENT_MODULE_PARM(probe, "List of adapter,address pairs to scan "	\
		       "additionally");					\
I2C_CLIENT_MODULE_PARM(ignore, "List of adapter,address pairs not to "	\
		       "scan");						\
static struct i2c_client_address_data addr_data = {			\
	.normal_i2c	= normal_i2c,					\
	.probe		= probe,					\
	.ignore		= ignore,					\
	.forces		= forces,					\
}

/* These are the ones you want to use in your own drivers. Pick the one
   which matches the number of devices the driver differenciates between. */
#define I2C_CLIENT_INSMOD \
  I2C_CLIENT_MODULE_PARM(force, \
                      "List of adapter,address pairs to boldly assume " \
                      "to be present"); \
	static unsigned short *forces[] = {				\
			force,						\
			NULL						\
		};							\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_1(chip1)					\
enum chips { any_chip, chip1 };						\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
static unsigned short *forces[] = { force, force_##chip1, NULL };	\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_2(chip1, chip2)				\
enum chips { any_chip, chip1, chip2 };					\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, NULL };		\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_3(chip1, chip2, chip3)			\
enum chips { any_chip, chip1, chip2, chip3 };				\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    NULL };				\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_4(chip1, chip2, chip3, chip4)			\
enum chips { any_chip, chip1, chip2, chip3, chip4 };			\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip4);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    force_##chip4, NULL};		\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_5(chip1, chip2, chip3, chip4, chip5)		\
enum chips { any_chip, chip1, chip2, chip3, chip4, chip5 };		\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip4);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip5);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    force_##chip4, force_##chip5,	\
				    NULL };				\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_6(chip1, chip2, chip3, chip4, chip5, chip6)	\
enum chips { any_chip, chip1, chip2, chip3, chip4, chip5, chip6 };	\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip4);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip5);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip6);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    force_##chip4, force_##chip5,	\
				    force_##chip6, NULL };		\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_7(chip1, chip2, chip3, chip4, chip5, chip6, chip7) \
enum chips { any_chip, chip1, chip2, chip3, chip4, chip5, chip6,	\
	     chip7 };							\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip4);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip5);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip6);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip7);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    force_##chip4, force_##chip5,	\
				    force_##chip6, force_##chip7,	\
				    NULL };				\
I2C_CLIENT_INSMOD_COMMON

#define I2C_CLIENT_INSMOD_8(chip1, chip2, chip3, chip4, chip5, chip6, chip7, chip8) \
enum chips { any_chip, chip1, chip2, chip3, chip4, chip5, chip6,	\
	     chip7, chip8 };						\
I2C_CLIENT_MODULE_PARM(force, "List of adapter,address pairs to "	\
		       "boldly assume to be present");			\
I2C_CLIENT_MODULE_PARM_FORCE(chip1);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip2);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip3);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip4);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip5);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip6);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip7);					\
I2C_CLIENT_MODULE_PARM_FORCE(chip8);					\
static unsigned short *forces[] = { force, force_##chip1,		\
				    force_##chip2, force_##chip3,	\
				    force_##chip4, force_##chip5,	\
				    force_##chip6, force_##chip7,	\
				    force_##chip8, NULL };		\
I2C_CLIENT_INSMOD_COMMON

#endif /* _LINUX_I2C_USER_H */
