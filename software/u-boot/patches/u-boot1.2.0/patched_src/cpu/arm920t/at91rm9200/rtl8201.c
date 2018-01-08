/*
 *
 * RTL8201 PHY driver
 * (C) Copyright 2005
 * Roman Avramenko <roman@imsystems.ru>
 *
 * (C) Copyright 2003
 * Author : Hamid Ikdoumi (Atmel)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <at91rm9200_net.h>
#include <net.h>
#include <rtl8201.h>

#ifdef CONFIG_DRIVER_ETHER

#if (CONFIG_COMMANDS & CFG_CMD_NET)

/*
 * Name:
 *	rtl8201_IsPhyConnected
 * Description:
 *	Reads the 2 PHY ID registers
 * Arguments:
 *	p_mac - pointer to AT91S_EMAC struct
 * Return value:
 *	TRUE - if id read successfully
 *	FALSE- if error
 */
static unsigned int rtl8201_IsPhyConnected (AT91PS_EMAC p_mac)
{
	unsigned short Id1, Id2;
	printf("%s:%s:%d\n",__FILE__,__func__,__LINE__);
	at91rm9200_EmacEnableMDIO (p_mac);
	at91rm9200_EmacReadPhy (p_mac, RTL8201_PHYID1, &Id1);
	at91rm9200_EmacReadPhy (p_mac, RTL8201_PHYID2, &Id2);
	at91rm9200_EmacDisableMDIO (p_mac);

	if ( Id1 == RTL8201_PHYID1_OUI  && Id2 == RTL8201_PHYID2_OUI ) {
		printf("PHY: RTL8201(B)L detected\n");
		return TRUE;
	}

	return FALSE;

}

/*
 * Name:
 *	rtl8201_GetLinkSpeed
 * Description:
 *	Link parallel detection status of MAC is checked and set in the
 *	MAC configuration registers
 * Arguments:
 *	p_mac - pointer to MAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
static UCHAR rtl8201_GetLinkSpeed (AT91PS_EMAC p_mac)
{
	unsigned short stat1, stat2;

	if (!at91rm9200_EmacReadPhy (p_mac, RTL8201_BMSR, &stat1))
		return FALSE;

	printf("Link: ");

	if (!(stat1 & RTL8201_LINK_STATUS)){	/* link status up? */
		printf("cable disconnected\n");
		return FALSE;
	}

	if (!at91rm9200_EmacReadPhy (p_mac, RTL8201_BMCR, &stat1))
		return FALSE;

	if (!at91rm9200_EmacReadPhy (p_mac, RTL8201_TEST, &stat2))
		return FALSE;

	if ((stat1 & RTL8201_DUPLEX_MODE) && (stat2 & RTL8201_LINK100)) {
		/*set MII for 100BaseTX and Full Duplex  */
		p_mac->EMAC_CFG |= AT91C_EMAC_SPD | AT91C_EMAC_FD;
		printf("100baseTX Full Duplex\n");
		return TRUE;
	}

	if ((stat1 & RTL8201_DUPLEX_MODE) && (stat2 & RTL8201_LINK10)) {
		/*set MII for 10BaseT and Full Duplex  */
		p_mac->EMAC_CFG = (p_mac->EMAC_CFG &
				~(AT91C_EMAC_SPD | AT91C_EMAC_FD))
				| AT91C_EMAC_FD;
		printf("10baseT Full Duplex\n");
		return TRUE;
	}

	if ((!(stat1 & RTL8201_DUPLEX_MODE)) && (stat2 & RTL8201_LINK100)) {
		/*set MII for 100BaseTX and Half Duplex  */
		p_mac->EMAC_CFG = (p_mac->EMAC_CFG &
				~(AT91C_EMAC_SPD | AT91C_EMAC_FD))
				| AT91C_EMAC_SPD;
		printf("100baseTX Half Duplex\n");
		return TRUE;
	}

	if ((!(stat1 & RTL8201_DUPLEX_MODE)) && (stat2 & RTL8201_LINK10)) {
		/*set MII for 10BaseT and Half Duplex  */
		p_mac->EMAC_CFG &= ~(AT91C_EMAC_SPD | AT91C_EMAC_FD);
		printf("10baseT Half Duplex\n");
		return TRUE;
	}
	printf("error\n");
	return FALSE;

}


/*
 * Name:
 *	rtl8201_InitPhy
 * Description:
 *	MAC starts checking its link by using parallel detection and
 *	Autonegotiation and the same is set in the MAC configuration registers
 * Arguments:
 *	p_mac - pointer to struct AT91S_EMAC
 * Return value:
 *	TRUE - if link status set succesfully
 *	FALSE - if link status not set
 */
static UCHAR rtl8201_InitPhy (AT91PS_EMAC p_mac)
{
	UCHAR ret = TRUE;

	at91rm9200_EmacEnableMDIO (p_mac);

	if (!rtl8201_GetLinkSpeed (p_mac)) {
		/* Try another time */
		ret = rtl8201_GetLinkSpeed (p_mac);
	}

	at91rm9200_EmacDisableMDIO (p_mac);

	return (ret);
}


/*
 * Name:
 *	rtl8201_AutoNegotiate
 * Description:
 *	MAC Autonegotiates with the partner status of same is set in the
 *	MAC configuration registers
 * Arguments:
 *	dev - pointer to struct net_device
 * Return value:
 *	TRUE - if link status set successfully
 *	FALSE - if link status not set
 */
static UCHAR rtl8201_AutoNegotiate (AT91PS_EMAC p_mac, int *status)
{
	return FALSE;
}


/*
 * Name:
 *	at91rm9200_GetPhyInterface
 * Description:
 *	Initialise the interface functions to the PHY
 * Arguments:
 *	None
 * Return value:
 *	None
 */
void at91rm9200_GetPhyInterface(AT91PS_PhyOps p_phyops)
{
	p_phyops->Init = rtl8201_InitPhy;
	p_phyops->IsPhyConnected = rtl8201_IsPhyConnected;
	p_phyops->GetLinkSpeed = rtl8201_GetLinkSpeed;
	p_phyops->AutoNegotiate = rtl8201_AutoNegotiate;
}
#endif	/* CONFIG_COMMANDS & CFG_CMD_NET */

#endif	/* CONFIG_DRIVER_ETHER */
