/*
 (C) Copyright 2016, TP-Link Inc, konstantin.mauch@tp-link.com

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT any WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 MA 02111-1307 USA
*/

#include <stdio.h>

#if defined(UCSIMM)
#include <asm/MC68EZ328.h>
#endif /* (UCSIMM) */

#include "dport.h"

void PortD_Prepare()
{
#if defined(UCSIMM)

	PDSEL = PD0 | PD1;

	PDDIR = PD0 | PD1;

	printf ("Bits <%08bb> <%08bb> of Port D initialized as OUT\n", (unsigned char)PD0, (unsigned char)PD1);

#endif /* (UCSIMM) */
}

void PortD_Toggle(unsigned char uchBit)
{
#if defined(UCSIMM)
	PDDATA ^= uchBit;
#endif /* (UCSIMM) */
}

void PortD_Down(unsigned char uchBit)
{
#if defined(UCSIMM)
	PDDATA &= ~uchBit;
#endif /* (UCSIMM) */
}

void PortD_Up(unsigned char uchBit)
{
#if defined(UCSIMM)
	PDDATA |= uchBit;
#endif /* (UCSIMM) */
}

/* Port D probe routine, two bits are enough for current task */
void PortD_Probe( )
{
#if defined(UCSIMM)
	PortD_Prepare( );

	PortD_Toggle(  PD0 );

	while (1)
	{
		PortD_Toggle(  PD1 | PD0 );

		/* Dubious */
		usleep (10);
	}
#endif /* (UCSIMM) */
}
