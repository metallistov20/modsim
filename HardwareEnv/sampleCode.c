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
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h> 
#include <sys/time.h>

#include<asm/MC68EZ328.h>

#if 0
int main( )
{
	PDSEL = 0xF0;
	PDDIR = 0xF0;
	printf ("============\n");
	while (1)
	{
		PDDATA ^= 0xF0;
		usleep (10);
	}
	return 0;
}
#endif

int main( )
{
#if 0
    PESEL &= 0xf8;// PESEL &= ~(PE_SPMTXD | PE_SPMRXD | PE_SPMCLK);
#else
    PESEL &= ~(PE_SPMTXD | PE_SPMRXD | PE_SPMCLK);
#endif 
    
#if 0
    SPIMCONT = 0x0277;  // SPIMCONT = PUT_FIELD(SPIMCONT_DATA_RATE, 2) | SPIMCONT_PHA | PUTFIELD(SPIMCONT_BIT_COUNT, 15);
    SPIMCONT = PUT_FIELD(SPIMCONT_DATA_RATE, 2) | SPIMCONT_PHA | PUTFIELD(SPIMCONT_BIT_COUNT, 15);
#else
    SPIMCONT = 0x402f;
    SPIMCONT |= SPIMCONT_ENABLE;    
#endif 

	SPIMDATA = 0x52;
	SPIMCONT |= SPIMCONT_XCH;	
	while (!(SPIMCONT & SPIMCONT_IRQ)); 

	printf ("============\n") ;		

	while (1)
	{
		SPIMDATA = 0xAA;

		/* printf("%x\n", SPIMDATA); */

		SPIMCONT |= SPIMCONT_XCH;	

		while (!(SPIMCONT & SPIMCONT_IRQ)); 

			usleep (10) ;

		SPIMDATA = 0x55;

		/* printf("%x\n", SPIMDATA); */

		SPIMCONT |= SPIMCONT_XCH;	

		while (!(SPIMCONT & SPIMCONT_IRQ)); 

			usleep (10) ;

		SPIMDATA = 0xCC;

		/* printf("%x\n", SPIMDATA); */

		SPIMCONT |= SPIMCONT_XCH;	

		while (!(SPIMCONT & SPIMCONT_IRQ)); 

			usleep (10) ;

		SPIMDATA = 0x33;

		/* printf("%x\n", SPIMDATA); */

		SPIMCONT |= SPIMCONT_XCH;	

		while (!(SPIMCONT & SPIMCONT_IRQ)); 

			usleep (10) ;

	}

	return 0;	 

}
