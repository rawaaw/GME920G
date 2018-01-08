/*
 * NOTE:	RTL8201 ethernet Physical layer
 *
 * Version:	@(#)rtl8201.h	1.0.0	01/10/2001
 *
 * Authors:	ATMEL Rousset
 *
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */


#define	RTL8201_BMCR 		0	/* Basic Mode Control Register */
#define RTL8201_BMSR		1	/* Basic Mode Status Register */
#define RTL8201_PHYID1		2	/* PHY Idendifier Register 1 */
#define RTL8201_PHYID2		3	/* PHY Idendifier Register 2 */
#define RTL8201_ANAR		4	/* Auto_Negotiation Advertisement Register  */
#define RTL8201_ANLPAR		5	/* Auto_negotiation Link Partner Ability Register */
#define RTL8201_ANER		6	/* Auto-negotiation Expansion Register  */
#define RTL8201_TEST		25

/* --Bit definitions: RTL8201_BMCR */
#define RTL8201_RESET  	          (1 << 15)	/* 1= Software Reset; 0=Normal Operation */
#define RTL8201_LOOPBACK          (1 << 14)	/* 1=loopback Enabled; 0=Normal Operation */
#define RTL8201_SPEED_SELECT      (1 << 13)	/* 1=100Mbps; 0=10Mbps */
#define RTL8201_AUTONEG	          (1 << 12)
#define RTL8201_POWER_DOWN        (1 << 11)
#define RTL8201_RESTART_AUTONEG   (1 << 9)
#define RTL8201_DUPLEX_MODE       (1 << 8)

/*--Bit definitions: RTL8201_BMSR */
#define RTL8201_100BASE_T4        (1 << 15)
#define RTL8201_100BASE_TX_FD     (1 << 14)
#define RTL8201_100BASE_T4_HD     (1 << 13)
#define RTL8201_10BASE_T_FD       (1 << 12)
#define RTL8201_10BASE_T_HD       (1 << 11)
#define RTL8201_MF_PREAMB_SUPPR   (1 << 6)
#define RTL8201_AUTONEG_COMP      (1 << 5)
#define RTL8201_REMOTE_FAULT      (1 << 4)
#define RTL8201_AUTONEG_ABILITY   (1 << 3)
#define RTL8201_LINK_STATUS       (1 << 2)
#define RTL8201_JABBER_DETECT     (1 << 1)
#define RTL8201_EXTEND_CAPAB      (1 << 0)

/*--definitions: RTL8201_PHYID1 */
#define RTL8201_PHYID1_OUI	 0x0000
#define RTL8201_PHYID2_OUI	 0x8201

/*--Bit definitions: RTL8201_ANAR, RTL8201_ANLPAR */
#define RTL8201_NP               (1 << 15)
#define RTL8201_ACK              (1 << 14)
#define RTL8201_RF               (1 << 13)
#define RTL8201_FCS              (1 << 10)
#define RTL8201_T4               (1 << 9)
#define RTL8201_TX_FDX           (1 << 8)
#define RTL8201_TX_HDX           (1 << 7)
#define RTL8201_10_FDX           (1 << 6)
#define RTL8201_10_HDX           (1 << 5)
#define RTL8201_AN_IEEE_802_3	0x0001

/*--Bit definitions: RTL8201_ANER */
#define RTL8201_PDF              (1 << 4)
#define RTL8201_LP_NP_ABLE       (1 << 3)
#define RTL8201_NP_ABLE          (1 << 2)
#define RTL8201_PAGE_RX          (1 << 1)
#define RTL8201_LP_AN_ABLE       (1 << 0)


/*--Bit definitions: RTL8201_TEST */
#define RTL8201_LINK10		 (1 << 1)
#define RTL8201_LINK100		 (1 << 0)

#if 0
/*--Bit definitions: RTL8201_DSCR */
#define RTL8201_BP4B5B           (1 << 15)
#define RTL8201_BP_SCR           (1 << 14)
#define RTL8201_BP_ALIGN         (1 << 13)
#define RTL8201_BP_ADPOK         (1 << 12)
#define RTL8201_REPEATER         (1 << 11)
#define RTL8201_TX               (1 << 10)
#define RTL8201_RMII_ENABLE      (1 << 8)
#define RTL8201_F_LINK_100       (1 << 7)
#define RTL8201_SPLED_CTL        (1 << 6)
#define RTL8201_COLLED_CTL       (1 << 5)
#define RTL8201_RPDCTR_EN        (1 << 4)
#define RTL8201_SM_RST           (1 << 3)
#define RTL8201_MFP SC           (1 << 2)
#define RTL8201_SLEEP            (1 << 1)
#define RTL8201_RLOUT            (1 << 0)

/*--Bit definitions: RTL8201_DSCSR */
#define RTL8201_100FDX           (1 << 15)
#define RTL8201_100HDX           (1 << 14)
#define RTL8201_10FDX            (1 << 13)
#define RTL8201_10HDX            (1 << 12)

/*--Bit definitions: RTL8201_10BTCSR */
#define RTL8201_LP_EN           (1 << 14)
#define RTL8201_HBE             (1 << 13)
#define RTL8201_SQUELCH         (1 << 12)
#define RTL8201_JABEN           (1 << 11)
#define RTL8201_10BT_SER        (1 << 10)
#define RTL8201_POLR            (1 << 0)


/*--Bit definitions: RTL8201_MDINTR */
#define RTL8201_INTR_PEND       (1 << 15)
#define RTL8201_FDX_MASK        (1 << 11)
#define RTL8201_SPD_MASK        (1 << 10)
#define RTL8201_LINK_MASK       (1 << 9)
#define RTL8201_INTR_MASK       (1 << 8)
#define RTL8201_FDX_CHANGE      (1 << 4)
#define RTL8201_SPD_CHANGE      (1 << 3)
#define RTL8201_LINK_CHANGE     (1 << 2)
#define RTL8201_INTR_STATUS     (1 << 0)

#endif

/******************  function prototypes **********************/
static unsigned int rtl8201_IsPhyConnected(AT91PS_EMAC p_mac);
static unsigned char rtl8201_GetLinkSpeed(AT91PS_EMAC p_mac);
static unsigned char rtl8201_AutoNegotiate(AT91PS_EMAC p_mac, int *status);
static unsigned char rtl8201_InitPhy(AT91PS_EMAC p_mac);
