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
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "modsim.h"
#include "dtastr.h"

#include <stdio.h>

#include <errno.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/time.h>
#include<asm/MC68EZ328.h>

#if 1

#define PE0 0x80
#define PE1 0x40

#define PE0orPE1 (0x80 | 0x40)

void PortE_Prepare()
{
	PDSEL = 0xF0;

	PDDIR = 0xF0;
	printf ("============ ============ \n");
}

void PortE_Toggle(unsigned char ucBit)
{
	PDDATA ^= ucBit;
}

int main( )
{
	PortE_Prepare( );

	PortE_Toggle(  PE0 );

	while (1)
	{
		PortE_Toggle(  PE1 | PE0 );

		//PDDATA ^= 0xF0;//mF0
		//PDDATA ^= 0x80;//m80
		//PDDATA ^= 0x40;//m40
		//PDDATA ^= 0x20;//m20
		//PDDATA ^= 0x10;//m10

		//PDDATA ^= 0x01;//m01
		//PDDATA ^= 0x02;//m02
		//PDDATA ^= 0x04;//m04
		//PDDATA ^= 0x08;//m08

		// PDDATA ^= PE0orPE1;//PE0orPE1

		usleep (10);
	}
	return 0;
}
#endif

int _main ()
{
FILE *fp = NULL;

char cBuf [LARGE_BUF_SZ];

char cBf0 [SMALL_BUF_SZ], cBf1 [SMALL_BUF_SZ], cBf2 [SMALL_BUF_SZ];

float fltTM, fltDIn, fltDOut;

pTimepointType pTimeChain;

	if ( NULL == (fp = fopen (FILE_NAME, "r") ) )
	{
		printf("can't open \n");

		return P_ERROR;
	}

/*

Function	Module Pin
LD0	30
LD1	29
LD2	28
LD3	27
LFLM	26
LLAP	25
LCLK	24
LACD	23 
*/

/*
TTL уровни "0" 0,4В и "1" 2,4В
Точнее - <0.4 и >2.4 сответственно

Как то раз посмотрел я документацию.
Питание действительно - 5 V
А вот данные - не более 3.6 V
Я и сам удивился, столь не удобной (на мой взгляд) конфигурацции.
Но это так.
Вот документация на один из жуков:
http://www.terraelectronica.ru/pdf/NSC/USBN9603-28.pdf

*/
	while ( ! (feof (fp) ) ) 
	{
		if (0 > fscanf (fp, "%s", cBuf ) )
		{
			// eof reached
		}
		else
		{
		char * cpTmp = cBuf;

			//printf("scanned: >> %s\n", cBuf);

			fltTM = fltDIn = fltDOut = 0.0f;

			/* memset (cBf0, 0, SMALL_BUF_SZ);
			memset (cBf1, 0, SMALL_BUF_SZ);
			memset (cBf2, 0, SMALL_BUF_SZ); */

			while (*cpTmp) {  if (',' == *cpTmp) *cpTmp = ' '; cpTmp++; }
			//printf("changed: >> %s\n", cBuf);

			sscanf(cBuf, "%f %f %f,", &fltTM,     &fltDIn,   &fltDOut );
			//printf(" parced : >>  <%f> <%f> <%f>\n", fltTM, fltDIn, fltDOut );

			//_EnrollPoint("_AppendPoint", &pTimeChain, &fltTM, &fltDIn, &fltDOut, "emty");
			EnrollPoint(&pTimeChain, &fltTM, &fltDIn, &fltDOut, "emty");
			// usleep(500000);
		}
	}

	fclose(fp);

	printf("success 0 \n");

	//_ProcessPoints("_ProcessPoints", pTimeChain);
	ProcessPoints(pTimeChain);

	printf("success 1 \n");

	//_DeletePoints("_DeletePoints", &pTimeChain);
	DeletePoints(&pTimeChain);

	printf("success 2 \n");

	return P_SUCCESS;
}
